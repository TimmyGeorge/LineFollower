#include <Math/Math.hpp>
#include "Motor_Encoder.hpp"
#include "Arduino.h"

static constexpr unsigned long us_to_s = 1000UL*1000;

namespace {
  namespace Default{
    constexpr unsigned int pulses_per_revolution = 1000;
    constexpr float wheel_diameter = 0.1f;
    constexpr unsigned long speed_measurement_time_interval = 20000;
  };
}


Motor_Encoder* Motor_Encoder::encoder_for_isr[num_interrupts] = {0};

#define DEFINE_ENCODER_ISR(interrupt_num) \
void Motor_Encoder::encoderIsr##interrupt_num(){ \
  encoder_for_isr[interrupt_num]->isrChangeEncoderCount(); \
} 


DEFINE_ENCODER_ISR(0)
DEFINE_ENCODER_ISR(1)
DEFINE_ENCODER_ISR(2)
DEFINE_ENCODER_ISR(3)
DEFINE_ENCODER_ISR(4)
DEFINE_ENCODER_ISR(5)


void (*Motor_Encoder::getPointerToEncoderIsr(unsigned char interrupt_num))(){
  switch (interrupt_num){
    case 0:
      return &encoderIsr0;
    case 1:
      return &encoderIsr1;
    case 2:
      return &encoderIsr2;
    case 3:
      return &encoderIsr3;
    case 4:
      return &encoderIsr4;
    case 5:
      return &encoderIsr5;
    default:
      return nullptr;
  }
}


Motor_Encoder::Motor_Encoder(unsigned char _encoder_a_pin, unsigned char _encoder_b_pin):
Motor_Encoder(_encoder_a_pin, _encoder_b_pin, Default::pulses_per_revolution){}


Motor_Encoder::Motor_Encoder(unsigned char _encoder_a_pin, unsigned char _encoder_b_pin, unsigned int _pulses_per_revolution):
Motor_Encoder(_encoder_a_pin, _encoder_b_pin, _pulses_per_revolution, Default::wheel_diameter){}


Motor_Encoder::Motor_Encoder(unsigned char _encoder_a_pin, unsigned char _encoder_b_pin, unsigned int _pulses_per_revolution, float _wheel_diameter):
Motor_Encoder(_encoder_a_pin, _encoder_b_pin, _pulses_per_revolution, _wheel_diameter, Default::speed_measurement_time_interval){}

Motor_Encoder::Motor_Encoder(unsigned char _encoder_a_pin, unsigned char _encoder_b_pin, unsigned int _pulses_per_revolution, float _wheel_diameter, unsigned long _speed_measurement_time_interval):
encoder_a_pin(_encoder_a_pin), 
encoder_b_pin(_encoder_b_pin), 
pulses_per_revolution(_pulses_per_revolution), 
wheel_diameter(_wheel_diameter),
speed_counter(0),
distance_counter(0),
displacement_counter(0),
reset_counter(0),
previous_speed(0),
Timer(),
is_measuring(false),
speed_measurement_time_interval(_speed_measurement_time_interval){}


Motor_Encoder::~Motor_Encoder(){
  if (is_measuring){
    stopMeasuring();
  }
}


void Motor_Encoder::isrChangeEncoderCount(){
  Time_Interval interval = Timer.captureTimeInterval();
  distance_counter++;
  if (digitalRead(encoder_b_pin)){
    displacement_counter--;
    speed_counter--;
  }
  else{
    displacement_counter++;
    speed_counter++;
  }
  if (interval.length > speed_measurement_time_interval){
    previous_speed = (float)us_to_s * speed_counter / interval.length;
    Timer.start_time = interval.end;
    speed_counter = 0;
    ++reset_counter;
  }
}


void Motor_Encoder::startMeasuring(){
  distance_counter = 0;
  displacement_counter = 0;
  speed_counter = 0;
  previous_speed = 0;
  reset_counter = 0;
  is_measuring = true;
  unsigned char interrupt_num = digitalPinToInterrupt(encoder_a_pin);
  encoder_for_isr[interrupt_num] = this;
  attachInterrupt(interrupt_num, getPointerToEncoderIsr(interrupt_num), RISING);
  Timer.start();
}


void Motor_Encoder::stopMeasuring() {
  detachInterrupt(digitalPinToInterrupt(encoder_a_pin));
  is_measuring = false;
  //No need to set encoder_for_isr entry to nullptr
}


void Motor_Encoder::checkForIdling(unsigned long maximum_allowed_idle_time){
  noInterrupts();
  unsigned long time = Timer.seeTime();
  if (time > maximum_allowed_idle_time){ //i.e. count hasnt changed for a long time, so it probably stationary
    previous_speed = 0;
    speed_counter = 0;
    Timer.start();
    interrupts();
    return;
  }
  unsigned long time_for_this_check = Timer.seeTime() - time;
  Timer.start_time += time_for_this_check;
  interrupts();
}


void Motor_Encoder::checkForIdling(Motor_Speed minimum_allowed_speed){
  noInterrupts();
  unsigned long time = Timer.seeTime();
  if (time > speed_measurement_time_interval){
    //calculates the pulse speed if one pulse occurs in the time above the speed_measurement_time_interval
    float maximum_possible_current_pulse_speed = 1.0f/(time - speed_measurement_time_interval) * 1000ull *1000ull;
    if (maximum_possible_current_pulse_speed < minimum_allowed_speed.convToPulseSpeed()){
      previous_speed = 0;
      speed_counter = 0;
      Timer.start();
      interrupts();
      return;
    }
  }
  unsigned long time_for_this_check = Timer.seeTime() - time;
  Timer.start_time += time_for_this_check;
  interrupts();
}


const Motor_Encoder_Constants* Motor_Encoder::getConstants() const {
  return &encoder_constants;
}


Motor_Speed Motor_Encoder::getPreviousSpeed(){
  return Motor_Speed::createUsingPulseSpeed(previous_speed, this);
}


Motor_Speed Motor_Encoder::getCurrentSpeed(){
  noInterrupts();
  Time_Interval interval = Timer.captureTimeInterval();
  if (interval.length > speed_measurement_time_interval){
    previous_speed = (float)us_to_s * speed_counter / interval.length;
    Timer.start_time = interval.end;
    speed_counter = 0;
    interrupts();
    return Motor_Speed::createUsingPulseSpeed(previous_speed, this);
  }
  else{
    float count = speed_counter + previous_speed * (speed_measurement_time_interval - interval.length) / (float)us_to_s;
    float pulse_speed = us_to_s * count / speed_measurement_time_interval;
    // Serial.print("\t2 ");
    // Serial.print(speed_counter);
    // Serial.print('\t');
    interrupts();
    return Motor_Speed::createUsingPulseSpeed(pulse_speed, this);
  }
}


Motor_Distance Motor_Encoder::getDistance() const {
  return Motor_Distance::createUsingCount(distance_counter, this);
}


Motor_Displacement Motor_Encoder::getDisplacement() const {
  return Motor_Displacement::createUsingCount(displacement_counter, this);
}


unsigned long Motor_Encoder::getResetCounter() const {
  return reset_counter;
}

void Motor_Encoder::resetDistanceCounter(){
  distance_counter = 0;
}


void Motor_Encoder::resetDisplacementCounter(){
  displacement_counter = 0;
}




Motor_Speed::Motor_Speed(float _pulse_speed, const Motor_Encoder_Constants* _p_encoder_constants):
pulse_speed(_pulse_speed),
p_encoder_constants(_p_encoder_constants){}


Motor_Speed Motor_Speed::createUsingPulseSpeed(float pulse_speed, const Motor_Encoder* _p_encoder){
  return Motor_Speed(pulse_speed, _p_encoder->getConstants());
}


Motor_Speed Motor_Speed::createUsingRpm(float rpm, const Motor_Encoder* _p_encoder){
  const Motor_Encoder_Constants* p_encoder_constants = _p_encoder->getConstants();
  float pulse_speed = rpm / 60 * p_encoder_constants->pulses_per_revolution;
  return Motor_Speed(pulse_speed, p_encoder_constants);
}


Motor_Speed Motor_Speed::createUsingVelocity(float velocity, const Motor_Encoder* _p_encoder){
  const Motor_Encoder_Constants* p_encoder_constants = _p_encoder->getConstants();
  float pulse_speed = velocity / (Math::pi * p_encoder_constants->wheel_diameter) * p_encoder_constants->pulses_per_revolution;
  return Motor_Speed(pulse_speed, p_encoder_constants);
}


float Motor_Speed::convToPulseSpeed() const {
  return pulse_speed;
}


float Motor_Speed::convToRpm() const {
  return 60 * convToPulseSpeed() / p_encoder_constants->pulses_per_revolution;
}


float Motor_Speed::convToVelocity() const {
  return p_encoder_constants->wheel_diameter * Math::pi * convToRpm() / 60;
}




