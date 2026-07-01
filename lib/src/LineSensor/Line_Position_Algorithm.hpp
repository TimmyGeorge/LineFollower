#ifndef LINE_POSITION_ALGORITHM_HPP
#define LINE_POSITION_ALGORITHM_HPP
#include "Line_Position_Status.hpp"

using Line_Position_Estimator = Line_Position_Status (*)(const double*, unsigned char);

struct Line_Position_Algorithm{
    static void findHighestReadingSensorNum(const double* calibrated_readings, unsigned char num_sensors, unsigned char* out_start, unsigned char* out_run_length);
    static Line_Position_Status calcWeightedAverage(const double* calibrated_readings, unsigned char num_sensors);
    static Line_Position_Status calcQuadraticInterpolatedMax(const double* calibrated_readings, unsigned char num_sensors);
    static Line_Position_Status findHighestReadingPosition(const double* calibrated_readings, unsigned char num_sensors);
	static bool isOnLine(const double* calibrated_readings, unsigned char num_sensors, float threshold);
};

#endif