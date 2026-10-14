#ifndef VOLTAGE_DEBUGGER_TPP
#define VOLTAGE_DEBUGGER_TPP

#include <HardwareSerial.h>

template<typename Type>
void Voltage_Debugger::printValue(const char* prefix, const Type& value, const char* suffix, bool new_line){
  if (prefix != nullptr) Serial.print(prefix);
  Serial.print(value);
  if (suffix != nullptr) Serial.print(suffix);
  if (new_line) Serial.println();
}

#endif