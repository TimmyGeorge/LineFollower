#include "PID_Controller.hpp"
#include <Math/Math.hpp>

PID_Controller::PID_Controller():
PID_Controller(0,0,0){}


PID_Controller::PID_Controller(float _kp, float _kd, float _ki):
previous_error(0), integral(0), kp(_kp), kd(_kd), ki(_ki) {}


float PID_Controller::calcControllerOutput(float _error){
	integral+= _error;
	float output = kp*_error+kd*(_error-previous_error)+ki*integral;
	previous_error = _error;
	return output;
}




Antiwindup_PID_Controller::Antiwindup_PID_Controller(): 
PID_Controller(), upper_clamping_saturation_limit(0), lower_clamping_saturation_limit(0), is_clamping(0){}


Antiwindup_PID_Controller::Antiwindup_PID_Controller(float _kp, float _kd, float _ki, float _upper_clamping_saturation_limit, float _lower_clamping_saturation_limit):
PID_Controller(_kp, _kd, _ki), upper_clamping_saturation_limit(_upper_clamping_saturation_limit), lower_clamping_saturation_limit(_lower_clamping_saturation_limit), is_clamping(0){}


float Antiwindup_PID_Controller::calcControllerOutput(float _error) {
	integral += !is_clamping * _error;
	float output = kp*_error+kd*(_error-previous_error)+ki*integral;
	previous_error = _error;
	if (output > upper_clamping_saturation_limit){
		is_clamping = (_error < 0 && output < 0) || (_error > 0 && output > 0);
		output = upper_clamping_saturation_limit;
	}
	else if (output < lower_clamping_saturation_limit){
		is_clamping = (_error < 0 && output < 0) || (_error > 0 && output > 0);
		output = lower_clamping_saturation_limit;
	}
	else{
		is_clamping = false;
	}
	return output;
}