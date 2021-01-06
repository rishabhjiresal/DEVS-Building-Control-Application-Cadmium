#ifndef BOOST_SIMULATION_TEMPHUMIDITY_CONTROLLER_HPP
#define BOOST_SIMULATION_TEMPHUMIDITY_CONTROLLER_HPP

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

struct TempHumidityController_defs
{
        struct TempInput : public in_port<int> { };
        struct HumInput : public in_port<int> { };
        struct Temperature : public out_port<int> { };
        struct Humidity : public out_port<int> { };
};

template<typename TIME>
class TempHumidityController
{
  using defs=TempHumidityController_defs;
  	public:
      TempHumidityController() noexcept {
        state.temperature = 0;
        state.humidity = 0;
        state.active = false;
      }

      struct state_type {
        int temperature;
        int humidity;
        bool active;
        }; state_type state;

  using input_ports=std::tuple<typename defs::TempInput, typename defs::HumInput>;
	using output_ports=std::tuple<typename defs::Temperature, typename defs::Humidity>;

  void internal_transition (){
    state.active = false;
  }

  void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
		for(const auto &x : get_messages<typename defs::TempInput>(mbs)) {
			state.temperature = x;
		}

    for(const auto &x : get_messages<typename defs::HumInput>(mbs)) {
      state.humidity = x;
    }
    //If the values are not up to the mark, we can discard them here if that can be done.
		state.active = true;
	}

  void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
  internal_transition();
  external_transition(TIME(), std::move(mbs));
}

typename make_message_bags<output_ports>::type output() const {
  typename make_message_bags<output_ports>::type bags;
  if(state.temperature>-25 && state.temperature<50) {
  int OUTtemp = state.temperature;
  get_messages<typename defs::Temperature>(bags).push_back(OUTtemp);
  }
  if(state.humidity>0){
  int OUTHum = state.humidity;
  get_messages<typename defs::Humidity>(bags).push_back(OUTHum);
  }
  return bags;
}

TIME time_advance() const {
  if(state.active) {
    return TIME("00:00:00");
  }
  return std::numeric_limits<TIME>::infinity();
}

friend std::ostringstream& operator<<(std::ostringstream& os, const typename TempHumidityController<TIME>::state_type& i) {
           os << "Sent Data by TempHumController: " << i.temperature << "\t" << i.humidity;
           return os;
         }
};
#endif
