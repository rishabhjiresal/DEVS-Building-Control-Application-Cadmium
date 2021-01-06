#ifndef BOOST_SIMULATION_LCD_ACTUATION_CONTROLLER_HPP
#define BOOST_SIMULATION_LCD_ACTUATION_CONTROLLER_HPP

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
#include "../drivers/TextLCD.h"

struct LCDActuationController_defs {
    struct FusionInputT : public in_port<int> {};
    struct FusionInputH : public in_port<int> {};
};

TextLCD lcd(PA_10, PB_3, PB_5, PB_4, PB_10, PA_8);

template<typename TIME>
class LCDActuationController {
    using defs=LCDActuationController_defs;

    public:
       /* LCDActuationController() noexcept {
            MBED_ASSERT(false);
        }*/

        LCDActuationController() noexcept {
            state.receivedtoPrintfromFusionT = 0;
            state.receivedtoPrintfromFusionH = 0;

        }

        struct state_type {
            int receivedtoPrintfromFusionT;
            int receivedtoPrintfromFusionH;
           // int receivedtoPrintfromAC;
            bool printing;
           // drivers::LCD_DRIVER* lcd;
        }; state_type state;

        using input_ports=std::tuple<typename defs::FusionInputT, typename defs::FusionInputH>;
        using output_ports=std::tuple<>;

        void internal_transition() {
            state.printing = false;
        }

        void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
            for(const auto &x : get_messages<typename defs::FusionInputT>(mbs)) {
                state.receivedtoPrintfromFusionT = x;
               
            }
            for(const auto &x : get_messages<typename defs::FusionInputH>(mbs)) {
                state.receivedtoPrintfromFusionH = x;
            }
            state.printing = true;
        }
        void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
      			internal_transition();
      			external_transition(TIME(), std::move(mbs));
    }
        typename make_message_bags<output_ports>::type output() const {
    			typename make_message_bags<output_ports>::type bags;
                if(state.receivedtoPrintfromFusionT<100 && state.receivedtoPrintfromFusionH <100) {
    			lcd.printf("Temperature: %d\n",state.receivedtoPrintfromFusionT);
                lcd.printf("Humidity:  %d\n",state.receivedtoPrintfromFusionH);
                }
            return bags;
          }
    TIME time_advance() const {
            if(state.printing) {
			return TIME("00:01:00");
		}
		return std::numeric_limits<TIME>::infinity();
    }

friend std::ostringstream& operator<<(std::ostringstream& os, const typename LCDActuationController<TIME>::state_type& i) {
      os << "Temperature Output " << i.receivedtoPrintfromFusionT << "Humidity Output" <<i.receivedtoPrintfromFusionH;
      return os;
    }
};
#else
struct LCDActuationController_defs {
    struct FusionInputT : public in_port<int> {};
    struct FusionInputH : public in_port<int> {};
    struct ACStatus : public in_port<float> {};
    struct outTemp : public out_port<int> {};
        struct outHum : public out_port<int> {};

};

template<typename TIME>
class LCDActuationController {
    using defs=LCDActuationController_defs;
    public:
        LCDActuationController() noexcept {
            //MBED_ASSERT(false);
        }

        LCDActuationController(const char* rs,const char* en, const char* d4, const char* d5, const char*  d6, const char* d7) noexcept {
            
        }

        struct state_type {
            int receivedtoPrintfromFusionT;
            int receivedtoPrintfromFusionH;
            int receivedtoPrintfromAC;
            bool printing;
        }; state_type state;

        using input_ports=std::tuple<typename defs::FusionInputT, typename defs::FusionInputH, typename defs::ACStatus>;
        using output_ports=std::tuple<typename defs::outTemp, typename defs::outHum>;

        void internal_transition() {
            state.printing = false;
        }

        void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
            for(const auto &x : get_messages<typename defs::FusionInputT>(mbs)) {
                state.receivedtoPrintfromFusionT = x;
            }
            for(const auto &x : get_messages<typename defs::FusionInputH>(mbs)) {
                state.receivedtoPrintfromFusionH = x;
            }
       //     for(const auto &x : get_messages<typename defs::ACStatus>(mbs)) {
        //        state.receivedtoPrintfromAC = x;
         //   }
            state.printing = true;
        }
        void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
      			internal_transition();
      			external_transition(TIME(), std::move(mbs));
    }
        typename make_message_bags<output_ports>::type output() const {
    			typename make_message_bags<output_ports>::type bags;
        int OUT = state.receivedtoPrintfromFusionT;
        int OUT2 = state.receivedtoPrintfromFusionH;
		get_messages<typename defs::outTemp>(bags).push_back(OUT);
   		get_messages<typename defs::outHum>(bags).push_back(OUT2);
            return bags;
          }
    TIME time_advance() const {
            if(state.printing) {
			return TIME("00:00:00");
		}
		return std::numeric_limits<TIME>::infinity();
    }

friend std::ostringstream& operator<<(std::ostringstream& os, const typename LCDActuationController<TIME>::state_type& i) {
      os << "Temperature Output " << i.receivedtoPrintfromFusionT << "Humidity Output" <<i.receivedtoPrintfromFusionH;
      return os;
    }
};
#endif
#endif