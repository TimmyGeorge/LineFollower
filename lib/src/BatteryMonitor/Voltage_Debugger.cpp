#if defined (ESP32)
#include "Voltage_Debugger.hpp"
#include "Esp32_Battery_Monitor.hpp"
#include <HardwareSerial.h>


void Voltage_Debugger::printValue(const char* prefix, const Simple_Potential_Divider value, const char* suffix, bool new_line){
  if (prefix != nullptr) Serial.print(prefix);
  Serial.print('{');
  Serial.print(value.r1);
  Serial.print(", ");
  Serial.print(value.r2);
  Serial.print('}');
  if (suffix != nullptr) Serial.print(suffix);
  if (new_line) Serial.println();
}


void Voltage_Debugger::printValue(const char* prefix, const Voltage_Reader& value, const char* suffix, bool new_line){
  if (prefix != nullptr) Serial.print(prefix);
  Serial.print("{mav=");
  Serial.print(value.maximum_adc_voltage);
  Serial.print(", ap=");
  Serial.print(value.adc_pin);
  Serial.print(", arr=");
  Serial.print(value.adc_read_resolution);
  Serial.print(", aar=");
  #ifndef ANALOGUE_READ_RESOLUTION_NOT_SUPPORTED
    Serial.print(value.adc_after_resolution);
  #endif
  Serial.print('}');
  if (suffix != nullptr) Serial.print(suffix);
  if (new_line) Serial.println();
}

void Voltage_Debugger::printValue(const char* prefix, const Esp32_Voltage_Reader& value, const char* suffix, bool new_line){
  printValue(prefix, (const Voltage_Reader&)value, suffix, new_line);
}

void Voltage_Debugger::printValue(const char* prefix, const Esp32_Battery_Monitor& value, const char* suffix, bool new_line){
  if (prefix != nullptr) Serial.print(prefix);
  Serial.print('{');
  printValue("minv=", value.minimum_vin, ", ", false);
  printValue("maxv=", value.maximum_vin, ", ", false);
  printValue("pd=", *value.p_potential_divider, ", ", false);
  printValue("vr=", *value.p_voltage_reader, nullptr, false);
  Serial.print('}');
  if (suffix != nullptr) Serial.print(suffix);
  if (new_line) Serial.println();
}


void Voltage_Debugger::testFirstConstructor(){
  Simple_Potential_Divider pd(1000, 2000);
  Esp32_Voltage_Reader vr(5); //CHANGE
  Esp32_Battery_Monitor bm(2.7, 4.2, &pd, &vr);
  printValue("bm = ", bm, nullptr, true);
}


void Voltage_Debugger::testSecondConstructor(){
  Simple_Potential_Divider divider(1500000, 4500000);
  Esp32_Voltage_Reader reader(1, 7); //CHANGE
  Esp32_Battery_Monitor monitor(0.5, 2.3, &divider, &reader);
  printValue("bm = ", monitor, nullptr, true);
}

void Voltage_Debugger::testConvVinToPercentage(){
  constexpr unsigned char num_trials = 3;
  Esp32_Voltage_Reader vr(4); //CHANGE
  struct Trial{
    float minv;
    float maxv;
    float _vin;
  };
  Trial Test[num_trials] = {
    {3.5, 6.3, 4.5},
    {2, 4.3, 12},
    {12.34, 13.12, 1},
  };
  for(const Trial& trial : Test){
    Esp32_Battery_Monitor bm(trial.minv, trial.maxv, nullptr, &vr);
    float percentage = bm.convVinToPercentage(trial._vin);
    printValue("minv=", trial.minv, " ", false);
    printValue("maxv=", trial.maxv, " ", false);
    printValue("_vin=", trial._vin, ", ", false);
    printValue(nullptr, percentage, nullptr, true);
  }
}

void Voltage_Debugger::testReadVoutVoltage(){
  Esp32_Voltage_Reader reader(15); //CHANGE
  char input[65] = {0};
  while (!strstr(input,"stop")){
    float voltage = reader.readVoltage();
    printValue("votlage: ", voltage, "V", true);
    Serial.flush();
    Serial.readBytes(input, 64);
    delay(2000);
  }
}


void Voltage_Debugger::testGetBatteryVoltage(){
  Simple_Potential_Divider divider(17000,9900);
  Esp32_Voltage_Reader reader(15);
  Esp32_Battery_Monitor battery_monitor(5.4, 8.4, &divider, &reader);
  char input[65] = {0};
  while (!strstr(input,"stop")){
    float voltage = battery_monitor.getBatteryVoltage();
    printValue("votlage: ", voltage, "V", true);
    Serial.flush();
    Serial.readBytes(input, 64);
    delay(2000);
  }
}


void Voltage_Debugger::testGetBatteryPercentage(){
  Simple_Potential_Divider divider(17000,9900);
  Esp32_Voltage_Reader reader(15);
  Esp32_Battery_Monitor battery_monitor(5.4, 8.4, &divider, &reader);
  char input[65] = {0};
  while (!strstr(input,"stop")){
    float percentage = battery_monitor.getBatteryPercentage();
    printValue("percentage: ", percentage, "%", true);
    Serial.flush();
    Serial.readBytes(input, 64);
    delay(2000);
  }
}
#endif