#include "Voltage_Reader.hpp"
#include <Arduino.h>
#include <Math/Math.hpp>

#ifndef ANALOGUE_READ_RESOLUTION_NOT_SUPPORTED
  namespace{
    namespace Default{
      constexpr unsigned char adc_read_resolution = 10;
      constexpr unsigned char adc_after_resolution = 10;
    }
  }
#endif


#ifndef ANALOGUE_READ_RESOLUTION_NOT_SUPPORTED
  Voltage_Reader::Voltage_Reader(float _maximum_adc_voltage, unsigned char _adc_pin):
  Voltage_Reader(_maximum_adc_voltage, _adc_pin, Default::adc_read_resolution, Default::adc_after_resolution){}


  Voltage_Reader::Voltage_Reader(float _maximum_adc_voltage, unsigned char _adc_pin, unsigned char _adc_read_resolution, unsigned char _adc_after_resolution):
  maximum_adc_voltage(_maximum_adc_voltage),
  adc_pin(_adc_pin),
  adc_read_resolution(_adc_read_resolution), 
  adc_after_resolution(_adc_after_resolution){}
#else
  Voltage_Reader::Voltage_Reader(float _maximum_adc_voltage, unsigned char _adc_pin):
  maximum_adc_voltage(_maximum_adc_voltage),
  adc_pin(_adc_pin){}
#endif



float Voltage_Reader::mapReadingToVoltage(int adc_reading) const {
  return maximum_adc_voltage * adc_reading / Math::powi(2, adc_read_resolution);
}


float Voltage_Reader::readVoltage() const {
  #ifndef ANALOGUE_READ_RESOLUTION_NOT_SUPPORTED
  analogReadResolution(adc_read_resolution);
  #endif
  int adc_reading = analogRead(adc_pin);
  float voltage = mapReadingToVoltage(adc_reading);
  #ifndef ANALOGUE_READ_RESOLUTION_NOT_SUPPORTED
  analogReadResolution(adc_after_resolution);
  #endif
  return voltage;
}
