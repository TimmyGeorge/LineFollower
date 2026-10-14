#if defined (ESP32)
#include "Esp32_Voltage_Reader.hpp"
#include <math.h>


namespace{
  namespace Default{
    constexpr unsigned char esp32_adc_read_resolution = 12;
    constexpr unsigned char esp32_adc_after_resolution = 12;
    constexpr double maximum_adc_voltage = 3.3;
  }
}

Esp32_Voltage_Reader::Esp32_Voltage_Reader(unsigned char _adc_pin):
Esp32_Voltage_Reader(_adc_pin, Default::esp32_adc_after_resolution){}


Esp32_Voltage_Reader::Esp32_Voltage_Reader(unsigned char _adc_pin, unsigned char _adc_after_resolution):
Voltage_Reader(Default::maximum_adc_voltage, _adc_pin, Default::esp32_adc_read_resolution, _adc_after_resolution){}


//https://github.com/G6EJD/ESP32-ADC-Accuracy-Improvement-function/blob/master/ESP32_ADC_Read_Voltage_Accurate.ino
float Esp32_Voltage_Reader::mapReadingToVoltage(int adc_reading) const {
  //Obtained From Internet - uses a non-linear relation to map adc_reading to voltage
  int& reading = adc_reading;
  float voltage = -0.000000000000016 * pow(reading,4) + 0.000000000118171 * pow(reading,3)- 0.000000301211691 * pow(reading,2)+ 0.001109019271794 * reading + 0.034143524634089;
  return voltage;
}

/* //Somehow the microcontroller now works without it... was the orignal adc pin 2 used bugged?
float Esp32_Voltage_Reader::readTrueVoltage() const {
  //My Adjustment - takes into account that connecting the ADC across the component
  float voltage = readVoltage();
  if (voltage < 0.01) return voltage;
  float true_voltage = 1.4623 * voltage + 0.1478;
  return true_voltage;
}
*/
#endif