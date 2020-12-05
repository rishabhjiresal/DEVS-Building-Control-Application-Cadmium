#include <iostream>
#include <chrono>
#include <algorithm>
#include <string>

#include <cadmium/modeling/coupling.hpp>
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/concept/coupled_model_assert.hpp>
#include <cadmium/modeling/dynamic_coupled.hpp>
#include <cadmium/modeling/dynamic_atomic.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/tuple_to_ostream.hpp>
#include <cadmium/logger/common_loggers.hpp>
#include <cadmium/io/iestream.hpp>


#include <NDTime.hpp>

#include <cadmium/real_time/arm_mbed/io/digitalInput.hpp>
#include <cadmium/real_time/arm_mbed/io/analogInput.hpp>
#include <cadmium/real_time/arm_mbed/io/pwmOutput.hpp>
#include <cadmium/real_time/arm_mbed/io/digitalOutput.hpp>

#include "../atomics/TempHumiditySensor.hpp"
#include "../atomics/TempHumidityController.hpp"
#include "../atomics/ACActuationController.hpp"
#include "../atomics/TempHumidityFusionControl.hpp"

#ifdef RT_ARM_MBED
  #include "../mbed.h"
#else
const char* t1_IN = "./inputs/Temperature_Sensor_Values1.txt";
const char* t2_IN = "./inputs/Temperature_Sensor_Values2.txt";
const char* t3_IN = "./inputs/Temperature_Sensor_Values3.txt";
const char* t4_IN = "./inputs/Temperature_Sensor_Values4.txt";
const char* t5_IN = "./inputs/Temperature_Sensor_Values5.txt";
const char* t6_IN = "./inputs/Temperature_Sensor_Values6.txt";
const char* t7_IN = "./inputs/Temperature_Sensor_Values7.txt";
const char* t8_IN = "./inputs/Temperature_Sensor_Values8.txt";
const char* t9_IN = "./inputs/Temperature_Sensor_Values9.txt";
const char* t10_IN = "./inputs/Temperature_Sensor_Values10.txt";
const char* h1_IN = "./inputs/Humidity_Sensor_Values1.txt";
const char* h2_IN = "./inputs/Humidity_Sensor_Values2.txt";
const char* h3_IN = "./inputs/Humidity_Sensor_Values3.txt";
const char* h4_IN = "./inputs/Humidity_Sensor_Values4.txt";
const char* h5_IN = "./inputs/Humidity_Sensor_Values5.txt";
const char* h6_IN = "./inputs/Humidity_Sensor_Values6.txt";
const char* h7_IN = "./inputs/Humidity_Sensor_Values7.txt";
const char* h8_IN = "./inputs/Humidity_Sensor_Values8.txt";
const char* h9_IN = "./inputs/Humidity_Sensor_Values9.txt";
const char* h10_IN = "./inputs/Humidity_Sensor_Values10.txt";
const char* PA_5 = "./outputs/AC_Values.txt";

#endif

using namespace std;

using hclock=chrono::high_resolution_clock;
using TIME = NDTime;

int main(int argc, char ** argv) {

  //This will end the main thread and create a new one with more stack.
  #ifdef RT_ARM_MBED
    //Logging is done over cout in RT_ARM_MBED
    struct oss_sink_provider{
      static std::ostream& sink(){
        return cout;
      }
    };
  #else
    // all simulation timing and I/O streams are ommited when running embedded

    auto start = hclock::now(); //to measure simulation execution time

    /*************** Loggers *******************/

    static std::ofstream out_data("DEVS_BCA_Cadmium_output.txt");
    struct oss_sink_provider{
      static std::ostream& sink(){
        return out_data;
      }
    };
  #endif


  using info=cadmium::logger::logger<cadmium::logger::logger_info, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
  using debug=cadmium::logger::logger<cadmium::logger::logger_debug, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
  using state=cadmium::logger::logger<cadmium::logger::logger_state, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
  using log_messages=cadmium::logger::logger<cadmium::logger::logger_messages, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
  using routing=cadmium::logger::logger<cadmium::logger::logger_message_routing, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
  using global_time=cadmium::logger::logger<cadmium::logger::logger_global_time, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
  using local_time=cadmium::logger::logger<cadmium::logger::logger_local_time, cadmium::dynamic::logger::formatter<TIME>, oss_sink_provider>;
  using log_all=cadmium::logger::multilogger<info, debug, state, log_messages, routing, global_time, local_time>;
  using logger_top=cadmium::logger::multilogger<log_messages, global_time>;

  using AtomicModelPtr=std::shared_ptr<cadmium::dynamic::modeling::model>;
  using CoupledModelPtr=std::shared_ptr<cadmium::dynamic::modeling::coupled<TIME>>;

  #ifdef RT_ARM_MBED
  AtomicModelPtr TempHumiditySensor1 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempHumiditySensor, TIME>("TempHumiditySensor1", A0);
  AtomicModelPtr TempHumiditySensor2 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempHumiditySensor, TIME>("TempHumiditySensor2", A1);
  AtomicModelPtr TempHumiditySensor3 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempHumiditySensor, TIME>("TempHumiditySensor3", A2);
  AtomicModelPtr TempHumiditySensor4 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempHumiditySensor, TIME>("TempHumiditySensor4", A3);
  AtomicModelPtr TempHumiditySensor5 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempHumiditySensor, TIME>("TempHumiditySensor5", A4);
  AtomicModelPtr TempHumiditySensor6 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempHumiditySensor, TIME>("TempHumiditySensor6", A5);
  AtomicModelPtr TempHumiditySensor7 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempHumiditySensor, TIME>("TempHumiditySensor7", PA_13);
  AtomicModelPtr TempHumiditySensor8 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempHumiditySensor, TIME>("TempHumiditySensor8", PA_14);
  AtomicModelPtr TempHumiditySensor9 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempHumiditySensor, TIME>("TempHumiditySensor9", PA_15);
  AtomicModelPtr TempHumiditySensor10 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempHumiditySensor, TIME>("TempHumiditySensor10", PB_7);
  #else
  AtomicModelPtr TempSensor1 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempSensor, TIME>("TempSensor1", t1_IN);
  AtomicModelPtr TempSensor2 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempSensor, TIME>("TempSensor2", t2_IN);
  AtomicModelPtr TempSensor3 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempSensor, TIME>("TempSensor3", t3_IN);
  AtomicModelPtr TempSensor4 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempSensor, TIME>("TempSensor4", t4_IN);
  AtomicModelPtr TempSensor5 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempSensor, TIME>("TempSensor5", t5_IN);
  AtomicModelPtr TempSensor6 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempSensor, TIME>("TempSensor6", t6_IN);
  AtomicModelPtr TempSensor7 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempSensor, TIME>("TempSensor7", t7_IN);
  AtomicModelPtr TempSensor8 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempSensor, TIME>("TempSensor8", t8_IN);
  AtomicModelPtr TempSensor9 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempSensor, TIME>("TempSensor9", t9_IN);
  AtomicModelPtr TempSensor10 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempSensor, TIME>("TempSensor10", t10_IN);
  AtomicModelPtr HumiditySensor1 = cadmium::dynamic::translate::make_dynamic_atomic_model<HumiditySensor, TIME>("HumiditySensor1", h1_IN);
  AtomicModelPtr HumiditySensor2 = cadmium::dynamic::translate::make_dynamic_atomic_model<HumiditySensor, TIME>("HumiditySensor2", h2_IN);
  AtomicModelPtr HumiditySensor3 = cadmium::dynamic::translate::make_dynamic_atomic_model<HumiditySensor, TIME>("HumiditySensor3", h3_IN);
  AtomicModelPtr HumiditySensor4 = cadmium::dynamic::translate::make_dynamic_atomic_model<HumiditySensor, TIME>("HumiditySensor4", h4_IN);
  AtomicModelPtr HumiditySensor5 = cadmium::dynamic::translate::make_dynamic_atomic_model<HumiditySensor, TIME>("HumiditySensor5", h5_IN);
  AtomicModelPtr HumiditySensor6 = cadmium::dynamic::translate::make_dynamic_atomic_model<HumiditySensor, TIME>("HumiditySensor6", h6_IN);
  AtomicModelPtr HumiditySensor7 = cadmium::dynamic::translate::make_dynamic_atomic_model<HumiditySensor, TIME>("HumiditySensor7", h7_IN);
  AtomicModelPtr HumiditySensor8 = cadmium::dynamic::translate::make_dynamic_atomic_model<HumiditySensor, TIME>("HumiditySensor8", h8_IN);
  AtomicModelPtr HumiditySensor9 = cadmium::dynamic::translate::make_dynamic_atomic_model<HumiditySensor, TIME>("HumiditySensor9", h9_IN);
  AtomicModelPtr HumiditySensor10 = cadmium::dynamic::translate::make_dynamic_atomic_model<HumiditySensor, TIME>("HumiditySensor10", h10_IN);
  #endif
  AtomicModelPtr TempHumidityController1 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempHumidityController, TIME>("TempHumidityController1");
  AtomicModelPtr TempHumidityController2 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempHumidityController, TIME>("TempHumidityController2");
  AtomicModelPtr TempHumidityController3 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempHumidityController, TIME>("TempHumidityController3");
  AtomicModelPtr TempHumidityController4 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempHumidityController, TIME>("TempHumidityController4");
  AtomicModelPtr TempHumidityController5 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempHumidityController, TIME>("TempHumidityController5");
  AtomicModelPtr TempHumidityController6 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempHumidityController, TIME>("TempHumidityController6");
  AtomicModelPtr TempHumidityController7 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempHumidityController, TIME>("TempHumidityController7");
  AtomicModelPtr TempHumidityController8 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempHumidityController, TIME>("TempHumidityController8");
  AtomicModelPtr TempHumidityController9 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempHumidityController, TIME>("TempHumidityController9");
  AtomicModelPtr TempHumidityController10 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempHumidityController, TIME>("TempHumidityController10");
  AtomicModelPtr TempHumidityFusionControl1 = cadmium::dynamic::translate::make_dynamic_atomic_model<TempHumidityFusionControl, TIME>("TempHumidityFusionControl1");
  AtomicModelPtr ACActuationController1 = cadmium::dynamic::translate::make_dynamic_atomic_model<ACActuationController, TIME>("ACActuationController1");
  AtomicModelPtr pwmOutput1 = cadmium::dynamic::translate::make_dynamic_atomic_model<PwmOutput, TIME>("pwmOutput1", PA_5);

  cadmium::dynamic::modeling::Ports iports_TOP = {};
  cadmium::dynamic::modeling::Ports oports_TOP = {};
  #ifdef RT_ARM_MBED
  cadmium::dynamic::modeling::Models submodels_TOP = {TempHumiditySensor1, TempHumiditySensor2, TempHumiditySensor3, TempHumiditySensor4, TempHumiditySensor5, TempHumiditySensor6,
  TempHumiditySensor7, TempHumiditySensor8, TempHumiditySensor9, TempHumiditySensor10, TempHumidityController1, TempHumidityController2,
  TempHumidityController3, TempHumidityController4, TempHumidityController5, TempHumidityController6, TempHumidityController7, TempHumidityController8, TempHumidityController9, TempHumidityController10, TempHumidityFusionControl1, ACActuationController1, pwmOutput1};
#else
  cadmium::dynamic::modeling::Models submodels_TOP = {TempSensor1, TempSensor2, TempSensor3, TempSensor4, TempSensor5, TempSensor6, TempSensor7, TempSensor8, TempSensor9, TempSensor10,
HumiditySensor1, HumiditySensor2, HumiditySensor3, HumiditySensor4, HumiditySensor5, HumiditySensor6, HumiditySensor7, HumiditySensor8, HumiditySensor9, HumiditySensor10, TempHumidityController1, TempHumidityController2,
TempHumidityController3, TempHumidityController4, TempHumidityController5, TempHumidityController6, TempHumidityController7, TempHumidityController8, TempHumidityController9, TempHumidityController10,
TempHumidityFusionControl1, ACActuationController1, pwmOutput1};
#endif

cadmium::dynamic::modeling::EICs eics_TOP = {};
cadmium::dynamic::modeling::EOCs eocs_TOP = {};

#ifdef RT_ARM_MBED
cadmium::dynamic::modeling::ICs ics_TOP = {
  cadmium::dynamic::translate::make_IC<TempHumiditySensor_defs::Tempout, TempHumidityController_defs::TempInput>("TempHumiditySensor1","TempHumidityController1"),
  cadmium::dynamic::translate::make_IC<TempHumiditySensor_defs::Tempout, TempHumidityController_defs::TempInput>("TempHumiditySensor2","TempHumidityController2"),
  cadmium::dynamic::translate::make_IC<TempHumiditySensor_defs::Tempout, TempHumidityController_defs::TempInput>("TempHumiditySensor3","TempHumidityController3"),
  cadmium::dynamic::translate::make_IC<TempHumiditySensor_defs::Tempout, TempHumidityController_defs::TempInput>("TempHumiditySensor4","TempHumidityController4"),
  cadmium::dynamic::translate::make_IC<TempHumiditySensor_defs::Tempout, TempHumidityController_defs::TempInput>("TempHumiditySensor5","TempHumidityController5"),
  cadmium::dynamic::translate::make_IC<TempHumiditySensor_defs::Tempout, TempHumidityController_defs::TempInput>("TempHumiditySensor6","TempHumidityController6"),
  cadmium::dynamic::translate::make_IC<TempHumiditySensor_defs::Tempout, TempHumidityController_defs::TempInput>("TempHumiditySensor7","TempHumidityController7"),
  cadmium::dynamic::translate::make_IC<TempHumiditySensor_defs::Tempout, TempHumidityController_defs::TempInput>("TempHumiditySensor8","TempHumidityController8"),
  cadmium::dynamic::translate::make_IC<TempHumiditySensor_defs::Tempout, TempHumidityController_defs::TempInput>("TempHumiditySensor9","TempHumidityController9"),
  cadmium::dynamic::translate::make_IC<TempHumiditySensor_defs::Tempout, TempHumidityController_defs::TempInput>("TempHumiditySensor10","TempHumidityController10"),

  cadmium::dynamic::translate::make_IC<TempHumiditySensor_defs::Humout, TempHumidityController_defs::HumInput>("TempHumiditySensor1","TempHumidityController1"),
  cadmium::dynamic::translate::make_IC<TempHumiditySensor_defs::Humout, TempHumidityController_defs::HumInput>("TempHumiditySensor2","TempHumidityController2"),
  cadmium::dynamic::translate::make_IC<TempHumiditySensor_defs::Humout, TempHumidityController_defs::HumInput>("TempHumiditySensor3","TempHumidityController3"),
  cadmium::dynamic::translate::make_IC<TempHumiditySensor_defs::Humout, TempHumidityController_defs::HumInput>("TempHumiditySensor4","TempHumidityController4"),
  cadmium::dynamic::translate::make_IC<TempHumiditySensor_defs::Humout, TempHumidityController_defs::HumInput>("TempHumiditySensor5","TempHumidityController5"),
  cadmium::dynamic::translate::make_IC<TempHumiditySensor_defs::Humout, TempHumidityController_defs::HumInput>("TempHumiditySensor6","TempHumidityController6"),
  cadmium::dynamic::translate::make_IC<TempHumiditySensor_defs::Humout, TempHumidityController_defs::HumInput>("TempHumiditySensor7","TempHumidityController7"),
  cadmium::dynamic::translate::make_IC<TempHumiditySensor_defs::Humout, TempHumidityController_defs::HumInput>("TempHumiditySensor8","TempHumidityController8"),
  cadmium::dynamic::translate::make_IC<TempHumiditySensor_defs::Humout, TempHumidityController_defs::HumInput>("TempHumiditySensor9","TempHumidityController9"),
  cadmium::dynamic::translate::make_IC<TempHumiditySensor_defs::Humout, TempHumidityController_defs::HumInput>("TempHumiditySensor10","TempHumidityController10"),

  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Temperature, TempHumidityFusionControl_defs::s1T>("TempHumidityController1", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Temperature, TempHumidityFusionControl_defs::s2T>("TempHumidityController2", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Temperature, TempHumidityFusionControl_defs::s3T>("TempHumidityController3", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Temperature, TempHumidityFusionControl_defs::s4T>("TempHumidityController4", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Temperature, TempHumidityFusionControl_defs::s5T>("TempHumidityController5", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Temperature, TempHumidityFusionControl_defs::s6T>("TempHumidityController6", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Temperature, TempHumidityFusionControl_defs::s7T>("TempHumidityController7", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Temperature, TempHumidityFusionControl_defs::s8T>("TempHumidityController8", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Temperature, TempHumidityFusionControl_defs::s9T>("TempHumidityController9", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Temperature, TempHumidityFusionControl_defs::s10T>("TempHumidityController10", "TempHumidityFusionControl1"),

  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Humidity, TempHumidityFusionControl_defs::s1H>("TempHumidityController1", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Humidity, TempHumidityFusionControl_defs::s2H>("TempHumidityController2", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Humidity, TempHumidityFusionControl_defs::s3H>("TempHumidityController3", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Humidity, TempHumidityFusionControl_defs::s4H>("TempHumidityController4", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Humidity, TempHumidityFusionControl_defs::s5H>("TempHumidityController5", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Humidity, TempHumidityFusionControl_defs::s6H>("TempHumidityController6", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Humidity, TempHumidityFusionControl_defs::s7H>("TempHumidityController7", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Humidity, TempHumidityFusionControl_defs::s8H>("TempHumidityController8", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Humidity, TempHumidityFusionControl_defs::s9H>("TempHumidityController9", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Humidity, TempHumidityFusionControl_defs::s10H>("TempHumidityController10", "TempHumidityFusionControl1"),

  cadmium::dynamic::translate::make_IC<TempHumidityFusionControl_defs::outT, ACActuationController_defs::TempFusedInput>("TempHumidityFusionControl1", "ACActuationController1"),
  cadmium::dynamic::translate::make_IC<TempHumidityFusionControl_defs::outH, ACActuationController_defs::HumFusedInput>("TempHumidityFusionControl1", "ACActuationController1"),
  cadmium::dynamic::translate::make_IC<ACActuationController_defs::ConditioningLevel, pwmOutput_defs::in>("ACActuationController1", "pwmOutput1")



};
#else
cadmium::dynamic::modeling::ICs ics_TOP = {
  cadmium::dynamic::translate::make_IC<TempSensor_defs::out, TempHumidityController_defs::TempInput>("TempSensor1","TempHumidityController1"),
  cadmium::dynamic::translate::make_IC<HumiditySensor_defs::out, TempHumidityController_defs::HumInput>("HumiditySensor1","TempHumidityController1"),

  cadmium::dynamic::translate::make_IC<TempSensor_defs::out, TempHumidityController_defs::TempInput>("TempSensor2","TempHumidityController2"),
  cadmium::dynamic::translate::make_IC<HumiditySensor_defs::out, TempHumidityController_defs::HumInput>("HumiditySensor2","TempHumidityController2"),

  cadmium::dynamic::translate::make_IC<TempSensor_defs::out, TempHumidityController_defs::TempInput>("TempSensor3","TempHumidityController3"),
  cadmium::dynamic::translate::make_IC<HumiditySensor_defs::out, TempHumidityController_defs::HumInput>("HumiditySensor3","TempHumidityController3"),

  cadmium::dynamic::translate::make_IC<TempSensor_defs::out, TempHumidityController_defs::TempInput>("TempSensor4","TempHumidityController4"),
  cadmium::dynamic::translate::make_IC<HumiditySensor_defs::out, TempHumidityController_defs::HumInput>("HumiditySensor4","TempHumidityController4"),

  cadmium::dynamic::translate::make_IC<TempSensor_defs::out, TempHumidityController_defs::TempInput>("TempSensor5","TempHumidityController5"),
  cadmium::dynamic::translate::make_IC<HumiditySensor_defs::out, TempHumidityController_defs::HumInput>("HumiditySensor5","TempHumidityController5"),

  cadmium::dynamic::translate::make_IC<TempSensor_defs::out, TempHumidityController_defs::TempInput>("TempSensor6","TempHumidityController6"),
  cadmium::dynamic::translate::make_IC<HumiditySensor_defs::out, TempHumidityController_defs::HumInput>("HumiditySensor6","TempHumidityController6"),

  cadmium::dynamic::translate::make_IC<TempSensor_defs::out, TempHumidityController_defs::TempInput>("TempSensor7","TempHumidityController7"),
  cadmium::dynamic::translate::make_IC<HumiditySensor_defs::out, TempHumidityController_defs::HumInput>("HumiditySensor7","TempHumidityController7"),

  cadmium::dynamic::translate::make_IC<TempSensor_defs::out, TempHumidityController_defs::TempInput>("TempSensor8","TempHumidityController8"),
  cadmium::dynamic::translate::make_IC<HumiditySensor_defs::out, TempHumidityController_defs::HumInput>("HumiditySensor8","TempHumidityController8"),

  cadmium::dynamic::translate::make_IC<TempSensor_defs::out, TempHumidityController_defs::TempInput>("TempSensor9","TempHumidityController9"),
  cadmium::dynamic::translate::make_IC<HumiditySensor_defs::out, TempHumidityController_defs::HumInput>("HumiditySensor9","TempHumidityController9"),

  cadmium::dynamic::translate::make_IC<TempSensor_defs::out, TempHumidityController_defs::TempInput>("TempSensor10","TempHumidityController10"),
  cadmium::dynamic::translate::make_IC<HumiditySensor_defs::out, TempHumidityController_defs::HumInput>("HumiditySensor10","TempHumidityController10"),




  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Temperature, TempHumidityFusionControl_defs::s1T>("TempHumidityController1", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Temperature, TempHumidityFusionControl_defs::s2T>("TempHumidityController2", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Temperature, TempHumidityFusionControl_defs::s3T>("TempHumidityController3", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Temperature, TempHumidityFusionControl_defs::s4T>("TempHumidityController4", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Temperature, TempHumidityFusionControl_defs::s5T>("TempHumidityController5", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Temperature, TempHumidityFusionControl_defs::s6T>("TempHumidityController6", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Temperature, TempHumidityFusionControl_defs::s7T>("TempHumidityController7", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Temperature, TempHumidityFusionControl_defs::s8T>("TempHumidityController8", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Temperature, TempHumidityFusionControl_defs::s9T>("TempHumidityController9", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Temperature, TempHumidityFusionControl_defs::s10T>("TempHumidityController10", "TempHumidityFusionControl1"),

  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Humidity, TempHumidityFusionControl_defs::s1H>("TempHumidityController1", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Humidity, TempHumidityFusionControl_defs::s2H>("TempHumidityController2", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Humidity, TempHumidityFusionControl_defs::s3H>("TempHumidityController3", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Humidity, TempHumidityFusionControl_defs::s4H>("TempHumidityController4", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Humidity, TempHumidityFusionControl_defs::s5H>("TempHumidityController5", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Humidity, TempHumidityFusionControl_defs::s6H>("TempHumidityController6", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Humidity, TempHumidityFusionControl_defs::s7H>("TempHumidityController7", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Humidity, TempHumidityFusionControl_defs::s8H>("TempHumidityController8", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Humidity, TempHumidityFusionControl_defs::s9H>("TempHumidityController9", "TempHumidityFusionControl1"),
  cadmium::dynamic::translate::make_IC<TempHumidityController_defs::Humidity, TempHumidityFusionControl_defs::s10H>("TempHumidityController10", "TempHumidityFusionControl1"),

  cadmium::dynamic::translate::make_IC<TempHumidityFusionControl_defs::outT, ACActuationController_defs::TempFusedInput>("TempHumidityFusionControl1", "ACActuationController1"),
  cadmium::dynamic::translate::make_IC<TempHumidityFusionControl_defs::outH, ACActuationController_defs::HumFusedInput>("TempHumidityFusionControl1", "ACActuationController1"),
  cadmium::dynamic::translate::make_IC<ACActuationController_defs::ConditioningLevel, pwmOutput_defs::in>("ACActuationController1", "pwmOutput1")


};
#endif
CoupledModelPtr TOP = std::make_shared<cadmium::dynamic::modeling::coupled<TIME>>(
    "TOP",
    submodels_TOP,
    iports_TOP,
    oports_TOP,
    eics_TOP,
    eocs_TOP,
    ics_TOP
    );

    cadmium::dynamic::engine::runner<NDTime, logger_top> r(TOP, {0});

r.run_until(NDTime("100:00:00:000"));
#ifndef RT_ARM_MBED
return 0;
#endif

}
