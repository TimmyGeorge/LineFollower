#define PRINT_CALIBRATED_READINGS 0
#if PRINT_CALIBRATED_READINGS == 1
  class Line_Sensor_Array_Debugger;
  #define PRINT_CALIBRATED_READINGS_FUNCTION(calibrated_readings) Line_Sensor_Array_Debugger::printArrayTypeMemberVariable("calibrated readings = ", calibrated_readings.get(), calibrated_readings.getNumReadings(), true);
#else
  #define PRINT_CALIBRATED_READINGS_FUNCTION(calibrated_readings) 
#endif

#define FOR_EACH_SENSOR for(unsigned char sensor_num = 0; sensor_num < num_sensors; ++sensor_num)


template<unsigned char num_sensors>
Calibrated_Sensor_Array<num_sensors>::Calibrated_Sensor_Array(Sensor_Array<num_sensors>&& sensor_array):
Calibrated_Sensor_Array((Sensor_Array<num_sensors>&&)sensor_array, default_minimum_possible_reading){}


template<unsigned char num_sensors>
Calibrated_Sensor_Array<num_sensors>::Calibrated_Sensor_Array(Sensor_Array<num_sensors>&& sensor_array, unsigned int _minimum_possible_reading):
Sensor_Array<num_sensors>((Sensor_Array<num_sensors>&&)sensor_array),
minimum_possible_reading(_minimum_possible_reading){
  resetCalibrationValues();
}


template<unsigned char num_sensors>
const unsigned int* Calibrated_Sensor_Array<num_sensors>::getMaximumReadings() const {
  return maximum_readings;
}


template<unsigned char num_sensors>
const unsigned int* Calibrated_Sensor_Array<num_sensors>::getMinimumReadings() const {
  return minimum_readings;
}


template<unsigned char num_sensors>
void Calibrated_Sensor_Array<num_sensors>::calibrateOnce(){
  unsigned int readings[num_sensors];
  this->takeRawReadings(readings);
  FOR_EACH_SENSOR{
    minimum_readings[sensor_num] = min(minimum_readings[sensor_num], readings[sensor_num]);
    maximum_readings[sensor_num] = max(maximum_readings[sensor_num], readings[sensor_num]);  
  }
}


template<unsigned char num_sensors>
void Calibrated_Sensor_Array<num_sensors>::calibrateByReadings(unsigned long num_readings){
  unsigned int readings[num_sensors];
  for(unsigned long reading_num = 0; reading_num < num_readings; ++reading_num){
    this->takeRawReadings(readings);
    FOR_EACH_SENSOR{
      minimum_readings[sensor_num] = min(minimum_readings[sensor_num], readings[sensor_num]);
      maximum_readings[sensor_num] = max(maximum_readings[sensor_num], readings[sensor_num]);  
    }
  }
}


template<unsigned char num_sensors>
void Calibrated_Sensor_Array<num_sensors>::calibrateByTime(unsigned long calibration_time){
  Microsecond_Timer Timer;
  Timer.start();
  while (Timer.seeTime() < calibration_time){
    calibrateOnce();
  }
}


template<unsigned char num_sensors>
void Calibrated_Sensor_Array<num_sensors>::trimCalibrationValues(double fraction){
  FOR_EACH_SENSOR{
    unsigned int trim = fraction*(maximum_readings[sensor_num]-minimum_readings[sensor_num]);
    maximum_readings[sensor_num] -= trim;
    minimum_readings[sensor_num] += trim;
  }
}


template<unsigned char num_sensors>
void Calibrated_Sensor_Array<num_sensors>::resetCalibrationValues(){
  FOR_EACH_SENSOR{
    maximum_readings[sensor_num] = minimum_possible_reading;
    minimum_readings[sensor_num] = this->timeout;
  }
}


template<unsigned char num_sensors>
void Calibrated_Sensor_Array<num_sensors>::calibrateReadings(const unsigned int* raw_readings, double* calibrated_readings) const {
  FOR_EACH_SENSOR{
    const unsigned int& rawr = raw_readings[sensor_num];
    const unsigned int& minr = minimum_readings[sensor_num];
    const unsigned int& maxr = maximum_readings[sensor_num];
    double& calr = calibrated_readings[sensor_num];
    if (minr == maxr)
      calr = 0;
    else if (rawr > maxr)
      calr = 1;
    else if (rawr < minr)
      calr =0;
    else
      calr = (double)(rawr - minr) / (maxr - minr);
  }
}


template <unsigned char num_sensors>
Calibrated_Readings<num_sensors> Calibrated_Sensor_Array<num_sensors>::takeCalibratedReadings() const {
  Calibrated_Readings<num_sensors> calibrated_readings;
  unsigned int buffer[num_sensors];
  this->takeRawReadings(buffer);
  calibrateReadings(buffer, calibrated_readings.get());
  PRINT_CALIBRATED_READINGS_FUNCTION(calibrated_readings);
  return calibrated_readings;
}

#undef FOR_EACH_SENSOR