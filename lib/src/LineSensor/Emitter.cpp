#include "Emitter.hpp"
#include <Arduino.h>

Emitter::Emitter(unsigned char maximum_dimming_level, unsigned char minimum_dimming_level, unsigned int pulse_time, unsigned int turn_off_time):
	maximum_dimming_level(maximum_dimming_level),
	minimum_dimming_level(minimum_dimming_level),
	pulse_time(pulse_time),
	turn_off_time(turn_off_time)
{}


unsigned char Emitter::calcNewDimmingLevel(unsigned char _dimming_level, unsigned char _increment){
  _increment += _dimming_level - getMinimumDimmingLevel();
  _increment %= getNumDimmingLevels();
  return _increment + getMinimumDimmingLevel();
}


unsigned char Emitter::calcIncrementRequired(unsigned char _dimming_level, unsigned char _desired_dimming_level){
  return (_desired_dimming_level < _dimming_level) ? (getNumDimmingLevels() + _desired_dimming_level - _dimming_level) : (_desired_dimming_level - _dimming_level);
}


unsigned char Emitter::getMaximumDimmingLevel() const {
	return maximum_dimming_level;
}


unsigned char Emitter::getMinimumDimmingLevel() const {
	return minimum_dimming_level;
}


unsigned char Emitter::getNumDimmingLevels() const {
	return maximum_dimming_level - minimum_dimming_level + 1;
}


unsigned int Emitter::getPulseTime() const {
	return pulse_time;
}


unsigned int Emitter::getTurnOffTime() const {
	return turn_off_time;
}


void Emitter::init(){
  setAsOutput();
  turnOff();
}


void Emitter::turnOff(){
  setLow();
  delayMicroseconds(turn_off_time);
  setStoredDimmingLevel(maximum_dimming_level);
}


void Emitter::turnOn(){
  setHigh();
  delayMicroseconds(pulse_time);
  setStoredDimmingLevel(minimum_dimming_level);
}


void Emitter::incrementDimmingLevel(unsigned char _increment){
  _increment %= getNumDimmingLevels();
  for(unsigned char i = 0; i < _increment; ++i){
	setLow();
    delayMicroseconds(pulse_time);
    setHigh();
	delayMicroseconds(pulse_time);
  }
  incrementStoredDimmingLevel(_increment);
}



Unbounded_Single_Emitter::Unbounded_Single_Emitter():
	pin(0),
	dimming_level(0)
{}


Unbounded_Single_Emitter::Unbounded_Single_Emitter(unsigned char emitter_pin, unsigned char dimming_level):
	pin(emitter_pin),
	dimming_level(dimming_level)
{}


void Unbounded_Single_Emitter::setAsOutput() const{
	pinMode(pin, OUTPUT);
}


void Unbounded_Single_Emitter::setHigh() const {
	digitalWrite(pin, HIGH);
}


void Unbounded_Single_Emitter::setLow() const {
	digitalWrite(pin, LOW);
}


unsigned char Unbounded_Single_Emitter::getDimmingLevel() const {
	return dimming_level;
}




Single_Emitter::Single_Emitter(unsigned char maximum_dimming_level, unsigned char minimum_dimming_level, unsigned int pulse_time, unsigned int turn_off_time, unsigned char emitter_pin):
	Single_Emitter_CRTP<Single_Emitter>(maximum_dimming_level, minimum_dimming_level, pulse_time, turn_off_time),
	emitter(emitter_pin, 0)	
{}


Unbounded_Single_Emitter& Single_Emitter::getUnboundedSingleEmitter(){
	return emitter;
}


const Unbounded_Single_Emitter& Single_Emitter::getUnboundedSingleEmitter() const {
	return emitter;
}



Single_Emitter_Reference::Single_Emitter_Reference(unsigned char maximum_dimming_level, unsigned char minimum_dimming_level, unsigned int pulse_time, unsigned int turn_off_time, Unbounded_Single_Emitter* emitter):
	Single_Emitter_CRTP<Single_Emitter_Reference>(maximum_dimming_level, minimum_dimming_level, pulse_time, turn_off_time),
	emitter(emitter)
{}


Unbounded_Single_Emitter& Single_Emitter_Reference::getUnboundedSingleEmitter() const {
	return *emitter;
}



Dual_Emitter::Dual_Emitter(unsigned char maximum_dimming_level, unsigned char minimum_dimming_level, unsigned int pulse_time, unsigned int turn_off_time, unsigned char odd_emitter_pin, unsigned char even_emitter_pin):
	Dual_Emitter_CRTP<Dual_Emitter>(maximum_dimming_level, minimum_dimming_level, pulse_time, turn_off_time),
	odd_emitter(odd_emitter_pin, 0),
	even_emitter(even_emitter_pin, 0)
{}


Unbounded_Single_Emitter& Dual_Emitter::getOddUnboundedSingleEmitter() {
		return odd_emitter;
}


const Unbounded_Single_Emitter& Dual_Emitter::getOddUnboundedSingleEmitter() const {
	return odd_emitter;
}


Unbounded_Single_Emitter& Dual_Emitter::getEvenUnboundedSingleEmitter() {
	return even_emitter;
}


const Unbounded_Single_Emitter& Dual_Emitter::getEvenUnboundedSingleEmitter() const {
	return even_emitter;
}




Dual_Emitter_Reference::Dual_Emitter_Reference(unsigned char maximum_dimming_level, unsigned char minimum_dimming_level, unsigned int pulse_time, unsigned int turn_off_time, Unbounded_Single_Emitter* odd_emitter, Unbounded_Single_Emitter* even_emitter):
	Dual_Emitter_CRTP<Dual_Emitter_Reference>(maximum_dimming_level, minimum_dimming_level, pulse_time, turn_off_time),
	odd_emitter(odd_emitter),
	even_emitter(even_emitter)
{}


Unbounded_Single_Emitter& Dual_Emitter_Reference::getOddUnboundedSingleEmitter() const {
	return *odd_emitter;
}


Unbounded_Single_Emitter& Dual_Emitter_Reference::getEvenUnboundedSingleEmitter() const {
	return *even_emitter;
}