#include "Timer.hpp"
#include <Arduino.h>


unsigned long Time_Interval::getStart() const {
	return end - length;
}




Timer::Timer():
start_time(0) {}


void Timer::start(){
	start_time = getTimestamp();
}


unsigned long Timer::seeTime() const{
	return getTimestamp() - start_time;
}


Time_Interval Timer::captureTimeInterval() const {
	unsigned long end_time = getTimestamp();
	return {end_time - start_time, end_time};
}



unsigned long Millisecond_Timer::getTimestamp() const {
  return millis();
}




unsigned long Microsecond_Timer::getTimestamp() const{
  return micros();
}

