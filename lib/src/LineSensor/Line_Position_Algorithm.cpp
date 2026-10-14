#include "Line_Position_Algorithm.hpp"
#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

void Line_Position_Algorithm::findHighestReadingSensorNum(const double* calibrated_readings, unsigned char num_sensors, unsigned char* out_start, unsigned char* out_length) {
  //if out_length = 0, all readings are 0 and there is no maximum
  unsigned char previous_start = 0;
  unsigned char previous_length = 0;
  unsigned char current_start = 0;
  unsigned char current_length = 0;
  double maximum_reading = 0;
  bool run_ended = true;
  //Find the string of consecutive sensors with the highest reading
  for(int sensor_num = 0; sensor_num < num_sensors; ++sensor_num){
    if (calibrated_readings[sensor_num] > maximum_reading){
      previous_start = 0;
      previous_length = 0;
      current_start = sensor_num;
      current_length = 1;
      maximum_reading = calibrated_readings[sensor_num];
      run_ended = false;
    }
    else if (calibrated_readings[sensor_num] == maximum_reading){
      if (run_ended){
        if (previous_length < current_length){
          previous_start = current_start;
          previous_length = current_length;
          current_start = sensor_num;
          current_length = 1;
          run_ended = false;
        }
      }
      else current_length++;
    }
    else if (!run_ended) run_ended = true;
  }
  if (previous_length < current_length){
    previous_start = current_start;
    previous_length = current_length;
  }
  *out_start = previous_start;
  *out_length = previous_length;
}


Line_Position_Status Line_Position_Algorithm::calcWeightedAverage(const double* calibrated_readings, unsigned char num_sensors) {
  double readings_sum = 0;                                           
  double weighted_sum = 0;
  for(unsigned char sensor_num=0; sensor_num<num_sensors; ++sensor_num){
    weighted_sum += sensor_num*calibrated_readings[sensor_num];
    readings_sum += calibrated_readings[sensor_num];
  }
  if (readings_sum == 0) return {0,Line_Position_Status::off_line};
  else return {weighted_sum/readings_sum,Line_Position_Status::unknown};
}


Line_Position_Status Line_Position_Algorithm::calcQuadraticInterpolatedMax(const double* calibrated_readings, unsigned char num_sensors) {
  double interpolated_max_sensor;
  unsigned char first_max_reading_sensor;
  unsigned char length;
  findHighestReadingSensorNum(calibrated_readings, num_sensors, &first_max_reading_sensor, &length);
  double max_reading_sensor = first_max_reading_sensor + (length - 1)/2.0f;
  if (length==0) return {0,Line_Position_Status::off_line};
  else if (length > 1) interpolated_max_sensor = max_reading_sensor;
  else{
    //readings are y1, y2, y3 with sensor_nums x, x+1, x+2
    //assume readings follow a quadratic 
    //s.t. y1 = ax^2 + bx + c, y2 = a(x+1)^2 + b(x+1) + c, y3 = a(x+2)^2 + b(x+2) + c
    //Calculate the turning point
    
    //initial values are given to make the compiler happy
    unsigned char x = 0; //co-ordinate to start quadratic interpolation
    unsigned char* px = &x;
    double a = 0;
    double b = 0;
    
    //create a function that takes an offset from the start x-value and computes the y-value
    auto y = [calibrated_readings, px] (unsigned int offset) -> double {
      return calibrated_readings[*px + offset - 1];
      //-1 is so that y(1) gives calibrated_readings[x] and not calibrated_readings[x+1]
    };
    if (first_max_reading_sensor == 0){
      x = first_max_reading_sensor;
      
      a = (y(1) + y(3) - 2 * y(2)) / 2;
      if (a > 0){
        interpolated_max_sensor = first_max_reading_sensor;
      }
      else {
        b = y(2) - y(1) - 2*a*x - a;
        interpolated_max_sensor = -b / (2*a);
        interpolated_max_sensor = max(interpolated_max_sensor, first_max_reading_sensor);
      }
    }
    else if (first_max_reading_sensor == num_sensors-1){
      x = first_max_reading_sensor - 2;
      a = (y(1) + y(3) - 2 * y(2)) / 2;
      if (a > 0){
        interpolated_max_sensor = first_max_reading_sensor;
      } 
      else {
        b = y(2) - y(1) - 2*a*x - a;
        interpolated_max_sensor = -b / (2*a);
        interpolated_max_sensor = min(interpolated_max_sensor, first_max_reading_sensor);
      }
    }
    else {
      x = max_reading_sensor - 1;    
      a = (y(1) + y(3) - 2 * y(2)) / 2;
      b = y(2) - y(1) - 2*a*x - a;
      interpolated_max_sensor = -b / (2*a);
    }
  }
  return {interpolated_max_sensor, Line_Position_Status::unknown};
}


Line_Position_Status Line_Position_Algorithm::findHighestReadingPosition(const double* calibrated_readings, unsigned char num_sensors) {
  unsigned char first_max_reading_sensor;
  unsigned char run_length;
  findHighestReadingSensorNum(calibrated_readings, num_sensors, &first_max_reading_sensor, &run_length);
  if (run_length == 0) return {0,Line_Position_Status::off_line};
  double max_reading_sensor = first_max_reading_sensor + (run_length - 1)/2.0f;
  return {max_reading_sensor, Line_Position_Status::unknown};
}


bool Line_Position_Algorithm::isOnLine(const double* calibrated_readings, unsigned char num_sensors, float threshold){
	for(unsigned char sensor_num = 0; sensor_num < num_sensors; ++sensor_num){
		if (calibrated_readings[sensor_num] >= threshold) return true;
	}
	return false;
}