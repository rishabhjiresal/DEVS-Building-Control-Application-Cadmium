#ifndef BOOST_SIMULATION_TEMPHUMIDITY_SENSOR_HPP
#define BOOST_SIMULATION_TEMPHUMIDITY_SENSOR_HPP

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

#ifdef RT_ARM_MBED
#include <cadmium/real_time/arm_mbed/embedded_error.hpp>
#include "../mbed-os/mbed.h"
#include "../drivers/TempHumidity_Driver.hpp"

struct TempHumiditySensor_defs
{
        struct Tempout : public out_port<int> { };
        struct Humout : public out_port<int> { };
};

template<typename TIME>
class TempHumiditySensor
{
using defs=TempHumiditySensor_defs;
	public:
		TIME pollingRate;
    TempHumiditySensor() noexcept {
      MBED_ASSERT(false);
    }

		TempHumiditySensor(PinName TempPin) noexcept {
      new (this) TempHumiditySensor(TempPin, TIME("00:01:00:000"));
    		}

        TempHumiditySensor(PinName TempPin, TIME rate) noexcept {
          pollingRate = rate;
          state.TempHum = new drivers::TEMPHUMIDITY_DRIVER(TempPin);
        }

    		struct state_type {
    			int lastTemp;
          int lastHum;
    			int outputTemp;
          int outputHum;
          drivers::TEMPHUMIDITY_DRIVER* TempHum;
    			}; state_type state;

    		using input_ports=std::tuple<>;
    		using output_ports=std::tuple<typename defs::Tempout, typename defs::Humout>;

    		void internal_transition() {
    			//state.lastTemp = state.outputTemp;
          //state.lastHum = state.outputHum;
          state.TempHum->TempHumidity(state.outputTemp, state.outputHum);
         // printf("Temp = %d \n", state.outputTemp);
         // printf("Hum = %d \n", state.outputHum);
    			}

    		void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
      			MBED_ASSERT(false);
      //throw std::logic_error("External transition called in a model with no input ports");
    }

    		void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
      			internal_transition();
      			external_transition(TIME(), std::move(mbs));
    }

    		typename make_message_bags<output_ports>::type output() const {
    			typename make_message_bags<output_ports>::type bags;
    			//if(state.lastTemp != state.outputTemp) {
    				get_messages<typename defs::Tempout>(bags).push_back(state.outputTemp);
    				//}
            //if(state.lastHum != state.outputHum) {
             
              get_messages<typename defs::Humout>(bags).push_back(state.outputHum);
            //}
            return bags;
          }

    		TIME time_advance() const {
    			  return TIME("00:01:00");
   		 }

   		 friend std::ostringstream& operator<<(std::ostringstream& os, const typename TempHumiditySensor<TIME>::state_type& i) {
      os << "Temperature Output " << i.outputTemp << "Humidity Output" <<i.outputHum;
      return os;
    }
  };

  #else
    #include <cadmium/io/iestream.hpp>
    using namespace cadmium;
    using namespace std;

    //Port definition
    struct TempSensor_defs{
      struct out : public out_port<int> {};
    };

    struct HumiditySensor_defs{
      struct out : public out_port<int> {};
    };

    template<typename TIME>
    class TempSensor : public iestream_input<int,TIME, TempSensor_defs>{
      public:
        TempSensor() = default;
        TempSensor(const char* file_path) : iestream_input<int,TIME, TempSensor_defs>(file_path) {}
        TempSensor(const char* file_path, TIME t) : iestream_input<int,TIME, TempSensor_defs>(file_path) {}
    };

    template<typename TIME>
    class HumiditySensor : public iestream_input<int, TIME, HumiditySensor_defs>{
    public:
      HumiditySensor() = default;
      HumiditySensor(const char* file_path) : iestream_input<int, TIME, HumiditySensor_defs>(file_path) {}
      HumiditySensor(const char* file_path, TIME t) : iestream_input<int, TIME, HumiditySensor_defs>(file_path) {}
    };

  #endif
  #endif
