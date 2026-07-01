#define PRINT_RAW_SENSOR_READINGS 0

#if PRINT_RAW_SENSOR_READINGS == 1
  #include "Line_Sensor_Array_Debugger.hpp"
  #define FUNCTION_PRINT_RAW_SENSOR_READING(readings, num_readings) Line_Sensor_Array_Debugger::printArrayTypeMemberVariable("raw_reading=", readings, num_readings, true);
#else
  #define FUNCTION_PRINT_RAW_SENSOR_READING(readings, num_readings) 
#endif

template <unsigned char num_sensors>
Sensor_Array<num_sensors>::Sensor_Array(unsigned char* _sensor_pins):
Sensor_Array(_sensor_pins, default_charge_time){}


template<unsigned char num_sensors>
Sensor_Array<num_sensors>::Sensor_Array(unsigned char* _sensor_pins, unsigned int _charge_time): 
Sensor_Array(_sensor_pins, _charge_time, default_maximum_possible_reading){}


template<unsigned char num_sensors>
Sensor_Array<num_sensors>::Sensor_Array(unsigned char* _sensor_pins, unsigned int _charge_time, unsigned int _timeout):
charge_time(_charge_time),
timeout(_timeout){
  memcpy(sensor_pins, _sensor_pins, sizeof(sensor_pins));
}


template<unsigned char num_sensors>
constexpr unsigned char Sensor_Array<num_sensors>::getNumSensors() {
  return num_sensors;
}


//https://www.pololu.com/docs/0J13/2
//https://pololu.github.io/qtr-sensors-arduino/_q_t_r_sensors_8cpp_source.html
template<unsigned char num_sensors>
void Sensor_Array<num_sensors>::takeRawReadings(unsigned int* raw_readings) const {
  //Microsecond_Timer Timer;
  long long time = 0;
  unsigned char num_low_pins = 0;
  //Disable Interrupts So We can Switch All Pins to Ouptut as fast as possible
  //Set All IO lines to output
  noInterrupts();
  for(int sensor_num=0; sensor_num<num_sensors; ++sensor_num){
    raw_readings[sensor_num] = timeout;
    pinMode(sensor_pins[sensor_num],OUTPUT);
    digitalWrite(sensor_pins[sensor_num],HIGH);
  }
  interrupts();

  //Leave High for 10us
  delayMicroseconds(charge_time);

  //Disable Interrupts to switch all pins to input as fast as possible
  //Set all pins to input, starting a timer at the point when the first sensor
  //is set to input
  noInterrupts();
  //Timer.start();
    unsigned long start_time = micros();
  for(int sensor_num=0; sensor_num<num_sensors; ++sensor_num){
    pinMode(sensor_pins[sensor_num],INPUT);
  }
  interrupts();

  //Measure the time it takes for the voltage to decay to LOW
  while (time < timeout){
    //Disable Interrupts to check all pins as fast as possible
    //Check whether a pin has just gone low. store that time.
    noInterrupts();
    //time = Timer.seeTime();
      time = micros() - start_time;
    for (unsigned char sensor_num=0; sensor_num<num_sensors; ++sensor_num){
      if (digitalRead(sensor_pins[sensor_num])==LOW && time < raw_readings[sensor_num]){
        raw_readings[sensor_num] = time;
        num_low_pins++;
        if (num_low_pins == num_sensors){
          interrupts();
          FUNCTION_PRINT_RAW_SENSOR_READING(raw_readings, num_sensors);
          return;
        };
      }
    }
    interrupts();
  }
  FUNCTION_PRINT_RAW_SENSOR_READING(raw_readings, num_sensors);
}
