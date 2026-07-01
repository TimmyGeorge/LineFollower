#if defined (ESP32)
#include "Esp32_Battery_Monitor.hpp"
#include <Simple_Potential_Divider/Simple_Potential_Divider.hpp>


Esp32_Battery_Monitor::Esp32_Battery_Monitor(float _minimum_vin, float _maximum_vin, Simple_Potential_Divider* _p_potential_divider, Esp32_Voltage_Reader* _p_voltage_reader):
minimum_vin(_minimum_vin), 
maximum_vin(_maximum_vin), 
p_potential_divider(_p_potential_divider), 
p_voltage_reader(_p_voltage_reader){}


float Esp32_Battery_Monitor::getBatteryVoltage() const{
  return p_voltage_reader->readVoltage()/p_potential_divider->calcTransferFunction();
}


float Esp32_Battery_Monitor::convVinToPercentage(float _vin) const{
  if (_vin < minimum_vin) return 0;
  else if (_vin > maximum_vin) return 100;
  else return (_vin-minimum_vin)/(maximum_vin-minimum_vin)*100;
}


float Esp32_Battery_Monitor::getBatteryPercentage() const{
  return convVinToPercentage(getBatteryVoltage());
}

#endif