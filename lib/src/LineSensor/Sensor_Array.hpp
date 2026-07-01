#ifndef SENSOR_ARRAY_HPP
#define SENSOR_ARRAY_HPP
#include <Arduino.h>
#include <string.h>
#include <Timer/Timer.hpp>


template<unsigned char num_sensors>
class Sensor_Array{
  friend class Line_Sensor_Array_Debugger;
  private:
    static constexpr unsigned int default_maximum_possible_reading = 50000;
    static constexpr unsigned int default_charge_time = 10;
  public:
    unsigned int charge_time;
    unsigned int timeout;    
  private:
    unsigned char sensor_pins[num_sensors] = {0};
  public:
    Sensor_Array(unsigned char* _sensor_pins);
    Sensor_Array(unsigned char* _sensor_pins, unsigned int _charge_time);
    Sensor_Array(unsigned char* _sensor_pins, unsigned int _charge_time, unsigned int _timeout);
  public:
    static constexpr unsigned char getNumSensors();
    void takeRawReadings(unsigned int* raw_readings) const;
};

#include "Sensor_Array.tpp"

#endif
