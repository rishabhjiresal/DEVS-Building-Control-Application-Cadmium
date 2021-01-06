#ifndef BOOST_SIMULATION_AC_ACTUATION_CONTROLLER_HPP
#define BOOST_SIMULATION_AC_ACTUATION_CONTROLLER_HPP

#include <stdio.h>
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>
#include <limits>
#include <math.h>
#include <assert.h>
#include <memory>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <algorithm>
#include <limits>
#include <random>


using namespace cadmium;
using namespace std;

struct ACActuationController_defs
{
        struct TempFusedInput : public in_port<int> { };
        struct HumFusedInput : public in_port<int> { };
    //    struct CO2FusedInput : public in_port<float> { };
        struct ConditioningLevel : public out_port<float> { };
};

template<typename TIME>
class ACActuationController {
  using defs=ACActuationController_defs;
public:
  ACActuationController () noexcept{
    state.active = false;
    state.tempLevel = 0;
    state.humLevel = 0;
    state.HotORCold = 0;
    state.ACLevel = 0;
  }

  struct state_type {
    bool active;
    int tempLevel;
    int humLevel;
    int ACLevel; //let's say the level is from 0 to 255
    bool HotORCold; //1 for Heater and 0 for cooler
    }; state_type state;

    using input_ports=std::tuple<typename defs::TempFusedInput, typename defs::HumFusedInput>;
    using output_ports=std::tuple<typename defs::ConditioningLevel>;

    void internal_transition (){
      state.active = false;
    }

  void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
    for(const auto &x : get_messages<typename defs::TempFusedInput>(mbs)) {
      state.tempLevel = x;
    }
    for(const auto &x : get_messages<typename defs::HumFusedInput>(mbs)) {
      state.humLevel = x;
    }

    if(state.tempLevel < 0) {
      state.HotORCold = 1;
    }
    else {
      state.HotORCold = 0;
    }

    if(state.HotORCold && ((state.tempLevel<0 && state.tempLevel>-20))) {
      state.ACLevel = -200;
    }
    else if(state.HotORCold && ((state.tempLevel<0 && state.tempLevel>-10))) {
        state.ACLevel = -100;
    }
    else if(!state.HotORCold && ((state.tempLevel>0 && state.tempLevel<10))) {
        state.ACLevel = 50;
    }
    else if(!state.HotORCold && ((state.tempLevel>10 && state.tempLevel<=20))) {
        state.ACLevel = 100;
    }
    else if(!state.HotORCold && ((state.tempLevel>20 && state.tempLevel<30))) {
        state.ACLevel = 150;
    }
    else if(!state.HotORCold && ((state.tempLevel>30 && state.tempLevel<40))) {
        state.ACLevel = 200;
    }
    else if(!state.HotORCold && ((state.tempLevel>40 && state.tempLevel<55))) {
        state.ACLevel = 255;
    }

    state.active = true;
  }

  void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
  internal_transition();
  external_transition(TIME(), std::move(mbs));
}

typename make_message_bags<output_ports>::type output() const {
  typename make_message_bags<output_ports>::type bags;
  float AcOut = state.ACLevel;
  get_messages<typename defs::ConditioningLevel>(bags).push_back(AcOut);

  return bags;
}

TIME time_advance() const {
  if(state.active) {
    return TIME("00:00:00");
  }
  return std::numeric_limits<TIME>::infinity();
}

friend std::ostringstream& operator<<(std::ostringstream& os, const typename ACActuationController<TIME>::state_type& i) {
           os << "Sent Data by ACActuationController: " << i.ACLevel;
           return os;
         }
};
#endif
