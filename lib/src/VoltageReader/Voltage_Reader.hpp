#ifndef VOLTAGE_READER_HPP
#define VOLTAGE_READER_HPP

#include <arduino.h>



#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_LEONARDO) || defined(ARDUINO_AVR_MICRO) || defined(ARDUINO_AVR_PROMINI)
  #define ANALOGUE_READ_RESOLUTION_NOT_SUPPORTED
#endif

class Voltage_Reader{
  friend class Voltage_Debugger;
  private:
    float maximum_adc_voltage;
    unsigned char adc_pin;
  public:
    #ifndef ANALOGUE_READ_RESOLUTION_NOT_SUPPORTED
      unsigned char adc_read_resolution;
      unsigned char adc_after_resolution;
    #else
      static constexpr unsigned char adc_read_resolution = 10;
    #endif
  public:
    Voltage_Reader(float _maximum_adc_voltage, unsigned char _adc_pin);
    #ifndef ANALOGUE_READ_RESOLUTION_NOT_SUPPORTED
      Voltage_Reader(float _maximum_adc_voltage, unsigned char _adc_pin, unsigned char _adc_read_resolution, unsigned char _adc_after_resolution);
    #endif
  private:
    virtual float mapReadingToVoltage(int adc_reading) const;
  public:
    
    float readVoltage() const;
    //possible readVoltageConsecutive would be a good idea to not have to set adc_resolution each time - could be a costly operation?
};




#endif