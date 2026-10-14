#if defined (ESP32)
#ifndef VOLTAGE_DEBUGGER_HPP
#define VOLTAGE_DEBUGGER_HPP
#include "Esp32_Battery_Monitor.hpp"

class Voltage_Debugger{
  public:
    template <typename Type>
    static void printValue(const char* prefix, const Type& value, const char* suffix, bool new_line);
    static void printValue(const char* prefix, const Simple_Potential_Divider value, const char* suffix, bool new_line);
    static void printValue(const char* prefix, const Voltage_Reader& value, const char* suffix, bool new_line);
    static void printValue(const char* prefix, const Esp32_Voltage_Reader& value, const char* suffix, bool new_line);
    static void printValue(const char* prefix, const Esp32_Battery_Monitor& value, const char* suffix, bool new_line);
    static void testFirstConstructor();
    static void testSecondConstructor();
    static void testConvVinToPercentage();
    static void testReadVoutVoltage();
    static void testGetBatteryVoltage();
    static void testGetBatteryPercentage();
};

#include "Voltage_Debugger.tpp"
#endif
#endif