template <unsigned char num_sensors>  
unsigned char Calibrated_Readings<num_sensors>::getNumReadings() const{
  return num_sensors;
}


template <unsigned char num_sensors>
double* Calibrated_Readings<num_sensors>::get(){
  return calibrated_readings;
}


template <unsigned char num_sensors>
void Calibrated_Readings<num_sensors>::set(const double* _calibrated_readings){
  memcpy(calibrated_readings, _calibrated_readings, sizeof(calibrated_readings));
}


template <unsigned char num_sensors>
void Calibrated_Readings<num_sensors>::invert(){
  for(unsigned char sensor_num=0; sensor_num<num_sensors; ++sensor_num)
    calibrated_readings[sensor_num] = 1 - calibrated_readings[sensor_num];
}


template <unsigned char num_sensors>
bool Calibrated_Readings<num_sensors>::isOnLine(float threshold) const {
  return Line_Position_Algorithm::isOnLine(calibrated_readings, num_sensors, threshold);
}


template <unsigned char num_sensors>
Line_Position_Status Calibrated_Readings<num_sensors>::estimateLinePosition(Sensor_Position_Calculator* sensor_position_calculator, Line_Position_Estimator line_position_estimator) const {
  Line_Position_Status line_position_status = line_position_estimator(calibrated_readings, num_sensors);
  line_position_status.estimate = sensor_position_calculator->calcPositionalValue(line_position_status.estimate, num_sensors);
  return line_position_status;
}


template <unsigned char num_sensors>
Line_Position_Status Calibrated_Readings<num_sensors>::estimateLinePositionStatus(Sensor_Position_Calculator* sensor_position_calculator, Line_Position_Estimator line_position_estimator, float threshold) const{
  Line_Position_Status line_position_status = estimateLinePosition(sensor_position_calculator, line_position_estimator);
  switch(line_position_status.status){
    case Line_Position_Status::unknown:
    case Line_Position_Status::error:
      line_position_status.status = (Line_Position_Status::Status)isOnLine(threshold);
      break;
    default:
      break;
  }
  return line_position_status;
}