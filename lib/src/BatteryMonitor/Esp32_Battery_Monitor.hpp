#if defined (ESP32)
#ifndef ESP32_BATTERY_MONITOR_HPP
#define ESP32_BATTERY_MONITOR_HPP
#include <Simple_Potential_Divider/Simple_Potential_Divider.hpp>
#include <Esp32VoltageReader/Esp32_Voltage_Reader.hpp>


class Esp32_Battery_Monitor{
  friend class Voltage_Debugger;
  private:
    float minimum_vin;
    float maximum_vin;
    Simple_Potential_Divider* p_potential_divider;
    Esp32_Voltage_Reader* p_voltage_reader;
  public:
    Esp32_Battery_Monitor(float _minimum_vin, float _maximum_vin, Simple_Potential_Divider* _p_potential_divider, Esp32_Voltage_Reader* _p_voltage_reader);
    float getBatteryVoltage() const;
    float convVinToPercentage(float _vin) const; //Could be improved for a better curve fitting model to my batteries
    float getBatteryPercentage() const;
};

#endif
#endif