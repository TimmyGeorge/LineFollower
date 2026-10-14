#ifndef LINE_SENSOR_ARRAY_HPP
#define LINE_SENSOR_ARRAY_HPP
#include "Calibrated_Sensor_Array.hpp"
#include "Calibrated_Readings.hpp"
#include "Line_Position_Status.hpp"
#include "Line_Position_Algorithm.hpp"

template <unsigned char num_sensors>
class Line_Sensor_Array : public Calibrated_Sensor_Array<num_sensors>{
  public:
    Line_Position_Estimator line_position_estimator;
    mutable Sensor_Position_Calculator* sensor_position_calculator;
  public:
    Line_Sensor_Array(Calibrated_Sensor_Array<num_sensors>&& calibrated_sensor_array, Line_Position_Estimator _line_position_estimator, Sensor_Position_Calculator* _sensor_position_calculator);
  public:
    double getMiddlePosition() const;
    double calcPositionalValue(double sensor_num) const;
    Line_Position_Status callLinePositionEstimator(double* calibrated_readings);
    Line_Position_Status estimateLinePositionUsing(double* calibrated_readings);
	Line_Position_Status estimateLinePositionStatusUsing(Calibrated_Readings<num_sensors> calibrated_readings, float threshold);
    Line_Position_Status estimateWhiteLinePosition() const ;
    Line_Position_Status estimateBlackLinePosition() const ;
    Line_Position_Status estimateBlackLinePositionStatus(double threshold = 0.5) const;
    Line_Position_Status estimateWhiteLinePositionStatus(double threshold = 0.5) const;
    
};

#include "Line_Sensor_Array.tpp"

#endif