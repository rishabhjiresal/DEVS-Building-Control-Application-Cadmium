#ifndef BOOST_SIMULATION_TEMPHUMIDITY_FUSION_CONTROL_HPP
#define BOOST_SIMULATION_TEMPHUMIDITY_FUSION_CONTROL_HPP

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

struct TempHumidityFusionControl_defs
{
  struct s1T : public in_port<float>{};
  struct s2T : public in_port<float>{};
  struct s3T : public in_port<float>{};
  struct s4T : public in_port<float>{};
  struct s5T : public in_port<float>{};
  struct s6T : public in_port<float>{};
  struct s7T : public in_port<float>{};
  struct s8T : public in_port<float>{};
  struct s9T : public in_port<float>{};
  struct s10T: public in_port<float>{};
  struct s1H : public in_port<float>{};
  struct s2H : public in_port<float>{};
  struct s3H : public in_port<float>{};
  struct s4H : public in_port<float>{};
  struct s5H : public in_port<float>{};
  struct s6H : public in_port<float>{};
  struct s7H : public in_port<float>{};
  struct s8H : public in_port<float>{};
  struct s9H : public in_port<float>{};
  struct s10H: public in_port<float>{};
  struct outT : public out_port<float> {};
  struct outH : public out_port<float> {};
};

template<typename TIME>
class TempHumidityFusionControl
{
  using defs=TempHumidityFusionControl_defs;
  	public:
      TempHumidityFusionControl() noexcept {
        for(int i=0;i<10;i++) {
          state.sT[i] = 0;
          state.sH[i] = 0;
        }
        state.FusedT = 0;
        state.FusedH = 0;
        state.LastH = 0;
        state.LastT = 0;
        state.active = false;
      }

      struct state_type {
        float sT [10];
        float sH [10];
        float FusedT;
        float FusedH;
        float LastT;
        float LastH;
        bool active;
        }; state_type state;

        using input_ports=std::tuple<typename defs::s1T, typename defs::s2T, typename defs::s3T, typename defs::s4T, typename defs::s5T, typename defs::s6T, typename defs::s7T, typename defs::s8T, typename defs::s9T, typename defs::s10T,
        typename defs::s1H, typename defs::s2H, typename defs::s3H, typename defs::s4H, typename defs::s5H, typename defs::s6H, typename defs::s7H, typename defs::s8H, typename defs::s9H, typename defs::s10H>;
      	using output_ports=std::tuple<typename defs::outT, typename defs::outH>;


        void internal_transition (){
          state.LastT = state.FusedT;
          state.LastH = state.FusedH;
          state.active = false;
        }

        void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
          for(const auto &x : get_messages<typename defs::s1T>(mbs)) {
            state.sT[0] = x;
          }
          for(const auto &x : get_messages<typename defs::s2T>(mbs)) {
            state.sT[1] = x;
          }
          for(const auto &x : get_messages<typename defs::s3T>(mbs)) {
            state.sT[2] = x;
          }
          for(const auto &x : get_messages<typename defs::s4T>(mbs)) {
            state.sT[3] = x;
          }
          for(const auto &x : get_messages<typename defs::s5T>(mbs)) {
            state.sT[4] = x;
          }
          for(const auto &x : get_messages<typename defs::s6T>(mbs)) {
            state.sT[5] = x;
          }
          for(const auto &x : get_messages<typename defs::s7T>(mbs)) {
            state.sT[6] = x;
          }
          for(const auto &x : get_messages<typename defs::s8T>(mbs)) {
            state.sT[7] = x;
          }
          for(const auto &x : get_messages<typename defs::s9T>(mbs)) {
            state.sT[8] = x;
          }
          for(const auto &x : get_messages<typename defs::s10T>(mbs)) {
            state.sT[9] = x;
          }


          for(const auto &x : get_messages<typename defs::s1H>(mbs)) {
            state.sH[0] = x;
          }
          for(const auto &x : get_messages<typename defs::s2H>(mbs)) {
            state.sH[1] = x;
          }
          for(const auto &x : get_messages<typename defs::s3H>(mbs)) {
            state.sH[2] = x;
          }
          for(const auto &x : get_messages<typename defs::s4H>(mbs)) {
            state.sH[3] = x;
          }
          for(const auto &x : get_messages<typename defs::s5H>(mbs)) {
            state.sH[4] = x;
          }
          for(const auto &x : get_messages<typename defs::s6H>(mbs)) {
            state.sH[5] = x;
          }
          for(const auto &x : get_messages<typename defs::s7H>(mbs)) {
            state.sH[6] = x;
          }
          for(const auto &x : get_messages<typename defs::s8H>(mbs)) {
            state.sH[7] = x;
          }
          for(const auto &x : get_messages<typename defs::s9H>(mbs)) {
            state.sH[8] = x;
          }
          for(const auto &x : get_messages<typename defs::s10H>(mbs)) {
            state.sH[9] = x;
          }

          state.FusedT = 0;
          state.FusedH = 0;

          for(int i=0;i<10;i++) {
            state.FusedT = state.FusedT + state.sT[i];
          }

          state.FusedT = state.FusedT / 10;

          for(int i=0;i<10;i++) {
            state.FusedH = state.FusedH + state.sH[i];
          }

          state.FusedH = state.FusedH / 10;

          //If the values are not up to the mark, we can discard them here if that can be done.
      		state.active = true;
      	}

        void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
        internal_transition();
        external_transition(TIME(), std::move(mbs));
      }

      typename make_message_bags<output_ports>::type output() const {
        typename make_message_bags<output_ports>::type bags;
        if(state.LastT != state.FusedT) {
          get_messages<typename defs::outT>(bags).push_back(state.FusedT);
        }
        if(state.LastH != state.FusedH) {
          get_messages<typename defs::outH>(bags).push_back(state.FusedH);
        }

        return bags;
      }

      TIME time_advance() const {
        if(state.active) {
          return TIME("00:00:00");
        }
        return std::numeric_limits<TIME>::infinity();
      }

      friend std::ostringstream& operator<<(std::ostringstream& os, const typename TempHumidityFusionControl<TIME>::state_type& i) {
                 os << "Sent Data by TempHumidityFusionControl: " << i.FusedT << "\t" << i.FusedH;
                 return os;
               }
      };
      #endif
