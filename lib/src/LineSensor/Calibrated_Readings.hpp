#ifndef CALIBRATED_READINGS_HPP
#define CALIBRATED_READINGS_HPP
#include "Line_Position_Status.hpp"
#include "Sensor_Position_Calculator.hpp"
#include "Line_Position_Algorithm.hpp"
#include <string.h>

template <unsigned char num_sensors>
class Calibrated_Readings {
  public: //to allow aggregate initialization
    double calibrated_readings[num_sensors];
  public:
    unsigned char getNumReadings() const;
    double* get();
    void set(const double* _calibrated_readings);
    void invert();
    bool isOnLine(float threshold = 0.5) const;
    Line_Position_Status estimateLinePosition(Sensor_Position_Calculator* sensor_position_calculator, Line_Position_Estimator line_position_estimator) const;
    Line_Position_Status estimateLinePositionStatus(Sensor_Position_Calculator* sensor_position_calculator, Line_Position_Estimator line_position_estimator, float threshold = 0.5) const;
};

#include "Calibrated_Readings.tpp"

#endif