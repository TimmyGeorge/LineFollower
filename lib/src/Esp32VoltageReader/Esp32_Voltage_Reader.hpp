#if defined (ESP32)
#ifndef ESP32_VOLTAGE_READER_HPP
#define ESP32_VOLTAGE_READER_HPP
#include <VoltageReader/Voltage_Reader.hpp>

class Esp32_Voltage_Reader : public Voltage_Reader{
    private:
      float mapReadingToVoltage(int adc_reading) const override;
    public:
      Esp32_Voltage_Reader(unsigned char _adc_pin);
      Esp32_Voltage_Reader(unsigned char _adc_pin, unsigned char _adc_after_resolution);
};


#endif
#endif