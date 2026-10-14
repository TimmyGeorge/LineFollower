template <unsigned char num_sensors>
Line_Sensor_Array<num_sensors>::Line_Sensor_Array(Calibrated_Sensor_Array<num_sensors>&& calibrated_sensor_array, Line_Position_Estimator _line_position_estimator, Sensor_Position_Calculator* _sensor_position_calculator):
Calibrated_Sensor_Array<num_sensors>((Calibrated_Sensor_Array<num_sensors>&&) calibrated_sensor_array), 
line_position_estimator(_line_position_estimator),
sensor_position_calculator(_sensor_position_calculator){}


template <unsigned char num_sensors>
double Line_Sensor_Array<num_sensors>::getMiddlePosition() const{
  return sensor_position_calculator->getMiddlePosition(num_sensors);
}


template <unsigned char num_sensors>
double Line_Sensor_Array<num_sensors>::calcPositionalValue(double sensor_num) const{
  return sensor_position_calculator->calcPositionalValue(sensor_num, num_sensors);
}


template <unsigned char num_sensors>
Line_Position_Status Line_Sensor_Array<num_sensors>::callLinePositionEstimator(double* calibrated_readings){
	return line_position_estimator(calibrated_readings, num_sensors);
}


template <unsigned char num_sensors>
Line_Position_Status Line_Sensor_Array<num_sensors>::estimateLinePositionUsing(double* calibrated_readings){
	Line_Position_Status line_position_status = callLinePositionEstimator(calibrated_readings);
	line_position_status.estimate = calcPositionalValue(line_position_status.estimate);
	return line_position_status;
}


template <unsigned char num_sensors>
Line_Position_Status Line_Sensor_Array<num_sensors>::estimateLinePositionStatusUsing(Calibrated_Readings<num_sensors> calibrated_readings, float threshold){
	return calibrated_readings.estimateLinePositionStatus(sensor_position_calculator, line_position_estimator, threshold);
}
    
       
template <unsigned char num_sensors>
Line_Position_Status Line_Sensor_Array<num_sensors>::estimateWhiteLinePosition() const {
  Calibrated_Readings<num_sensors> calibrated_readings = this->takeCalibratedReadings();
  calibrated_readings.invert();
  return calibrated_readings.estimateLinePosition(sensor_position_calculator, line_position_estimator);
}


template <unsigned char num_sensors>
Line_Position_Status Line_Sensor_Array<num_sensors>::estimateBlackLinePosition() const {
  return this->takeCalibratedReadings().estimateLinePosition(sensor_position_calculator, line_position_estimator);
}


template <unsigned char num_sensors>
Line_Position_Status Line_Sensor_Array<num_sensors>::estimateBlackLinePositionStatus(double threshold) const {
  return this->takeCalibratedReadings().estimateLinePositionStatus(sensor_position_calculator, line_position_estimator, threshold);
}


template <unsigned char num_sensors>
Line_Position_Status Line_Sensor_Array<num_sensors>::estimateWhiteLinePositionStatus(double threshold) const {
  Calibrated_Readings<num_sensors> calibrated_readings = this->takeCalibratedReadings();
  calibrated_readings.invert();
  return calibrated_readings.estimateLinePositionStatus(sensor_position_calculator, line_position_estimator, threshold);
}