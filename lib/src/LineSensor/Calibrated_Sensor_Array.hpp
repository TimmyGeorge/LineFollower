#ifndef CALIBRATED_SENSOR_ARRAY_HPP
#define CALIBRATED_SENSOR_ARRAY_HPP
#include "Sensor_Array.hpp"
#include "Calibrated_Readings.hpp"
#include <Timer/Timer.hpp>

template<unsigned char num_sensors>
class Calibrated_Sensor_Array : public Sensor_Array<num_sensors>{
  friend class Line_Sensor_Array_Debugger;
  private:
    static constexpr unsigned int default_minimum_possible_reading = 100;
  public:
    unsigned int minimum_possible_reading;
  private:
    unsigned int maximum_readings[num_sensors];
    unsigned int minimum_readings[num_sensors];
  public:
    Calibrated_Sensor_Array(Sensor_Array<num_sensors>&& sensor_array);
    Calibrated_Sensor_Array(Sensor_Array<num_sensors>&& sensor_array, unsigned int _minimum_possible_reading);
  public:
    const unsigned int* getMaximumReadings() const;
    const unsigned int* getMinimumReadings() const;
    void calibrateOnce();
    void calibrateByReadings(unsigned long num_readings);
    void calibrateByTime(unsigned long calibration_time = 10000000LL); //10 seconds
    void trimCalibrationValues(double fraction = 0.2);
    void resetCalibrationValues();
    void calibrateReadings(const unsigned int* raw_readings, double* calibrated_readings) const ;
    Calibrated_Readings<num_sensors> takeCalibratedReadings() const;
};

#include "Calibrated_Sensor_Array.tpp"
#endif