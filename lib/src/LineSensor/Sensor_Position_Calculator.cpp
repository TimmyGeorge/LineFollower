#include "Sensor_Position_Calculator.hpp"

double Sensor_Position_Calculator_With_Negative_Positions::calcPositionalValue(double sensor_num, unsigned char num_sensors) const {
  return sensor_num-(num_sensors-1)/2.0f;
}


double Sensor_Position_Calculator_With_Negative_Positions::getMiddlePosition(unsigned char) const {
  return 0;
}





double Sensor_Position_Calculator_Without_Negative_Positions::calcPositionalValue(double sensor_num, unsigned char) const {
  return sensor_num;
}


double Sensor_Position_Calculator_Without_Negative_Positions::getMiddlePosition(unsigned char num_sensors) const {
  return (num_sensors-1)/2.0f;
}


Sensor_Position_Calculator_With_Negative_Positions sensor_position_calculator_with_negative_positions;
Sensor_Position_Calculator_Without_Negative_Positions sensor_position_calculator_without_negative_positions;