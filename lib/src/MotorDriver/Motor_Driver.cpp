#include <Math/Math.hpp>
#include <Arduino.h>
#include "Motor_Driver.hpp"


Single_Motor_Driver::Single_Motor_Driver(unsigned char _in1_pin, unsigned char _in2_pin, unsigned char _pwm_pin):
in1_pin(_in1_pin),
in2_pin(_in2_pin),
pwm_pin(_pwm_pin),
velocity(0){}


void Single_Motor_Driver::drive(float speed){
	float adc_speed_value = speed * (Math::powi(2, adc_write_resolution)-1);
  	analogWrite(pwm_pin, adc_speed_value);
}


void Single_Motor_Driver::setVelocity(float _velocity) {
  if (velocity == _velocity) 
    return;
  else if (_velocity > 0){
    if (velocity <= 0)
      setForward();
    velocity = _velocity;
    drive(_velocity);
  }
  else if (_velocity < 0){
    if (velocity >= 0)
      setBackward();
    velocity = _velocity;
    drive(-_velocity);
  } 
  else{
	analogWrite(pwm_pin, 0);
	velocity = 0;
  }
}


void Single_Motor_Driver::setSpeed(float speed) {
  velocity = velocity < 0 ? -speed : speed;
  drive(speed);
};


void Single_Motor_Driver::setForward() {
  velocity = Math::absolute(velocity);
  digitalWrite(in1_pin, HIGH);
  digitalWrite(in2_pin, LOW);
}


void Single_Motor_Driver::setBackward() {
  velocity = -Math::absolute(velocity);
  digitalWrite(in1_pin, LOW);
  digitalWrite(in2_pin, HIGH);
}


void Single_Motor_Driver::init() {
  pinMode(in1_pin, OUTPUT);
  pinMode(in2_pin, OUTPUT);
}


void Single_Motor_Driver::stop() {
  velocity = 0;
  digitalWrite(in1_pin, LOW);
  digitalWrite(in2_pin, LOW);
  analogWrite(pwm_pin, 0);
}


float Single_Motor_Driver::getDriveVelocity() const {
  return velocity;
}



/*
void Acceleratable_Single_Motor_Driver::accelerate(float acceleration); //in normalized speed per second
	
}
*/




Dual_Motor_Driver::Dual_Motor_Driver(unsigned char _ina1_pin, unsigned char _ina2_pin, unsigned char _pwma_pin, unsigned char _inb1_pin, unsigned char _inb2_pin, unsigned char _pwmb_pin):
motora(_ina1_pin, _ina2_pin, _pwma_pin),
motorb(_inb1_pin, _inb2_pin, _pwmb_pin){}


void Dual_Motor_Driver::setVelocity(float _velocity){
  motora.setVelocity(_velocity);
  motorb.setVelocity(_velocity);
}


void Dual_Motor_Driver::setSpeed(float speed){
  motora.setSpeed(speed);
  motorb.setSpeed(speed);
}


void Dual_Motor_Driver::setForward(){
  motora.setForward();
  motorb.setForward();
}


void Dual_Motor_Driver::setBackward(){
  motora.setBackward();
  motorb.setBackward();
}


void Dual_Motor_Driver::init() {
  motora.init();
  motorb.init();
}


void Dual_Motor_Driver::stop() {
  motora.stop();
  motorb.stop();
}

float Dual_Motor_Driver::getDriveVelocity() const{
  return (motora.getDriveVelocity() + motorb.getDriveVelocity()) / 2;
}


void Dual_Motor_Driver::setVelocities(float base_velocity, float offset){
  motora.setVelocity(base_velocity + offset);
  motorb.setVelocity(base_velocity - offset);
}


Velocities Dual_Motor_Driver::getDriveVelocities() const{
  return {motora.getDriveVelocity(), motorb.getDriveVelocity()};
}




Dual_Motor_Driver_With_Standby::Dual_Motor_Driver_With_Standby(unsigned char _ina1_pin, unsigned char _ina2_pin, unsigned char _pwma_pin, unsigned char _inb1_pin, unsigned char _inb2_pin, unsigned char _pwmb_pin, unsigned char _standby_pin):
Dual_Motor_Driver(_ina1_pin, _ina2_pin, _pwma_pin, _inb1_pin, _inb2_pin, _pwmb_pin), 
standby_pin(_standby_pin), 
on_standby(true)
{}


void Dual_Motor_Driver_With_Standby::init() {
	Dual_Motor_Driver::init();
	pinMode(standby_pin, OUTPUT);
	placeOffStandby();
}


bool Dual_Motor_Driver_With_Standby::isOnStandby() const {
	return on_standby;
}


void Dual_Motor_Driver_With_Standby::placeOnStandby(){
	digitalWrite(standby_pin, LOW);
	on_standby = true;
}


void Dual_Motor_Driver_With_Standby::placeOffStandby(){
	digitalWrite(standby_pin, HIGH);
	on_standby = false;
};
