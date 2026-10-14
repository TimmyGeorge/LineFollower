#include <LineFollower.hpp>
#include <LineSensor/Line_Sensor.hpp>
#include <Serial/Serial.hpp>
#include <Timer/Timer.hpp>
#include <MotorDriver/Motor_Driver.hpp>

#define OUT(code) 
#define DELAY(code) 

constexpr float wheel_speed = 0.5;
Dual_Motor_Driver dual_motor_driver(22, 23, 4, 24, 25, 5);
//Single_Motor_Driver single_motor_driver(23, 22, 4);

void setup(){
  Serial.begin(9600);
  Serial.println("I Have Setup");
  Serial.println("I Have Setup");
  Serial.println("I Have Setup");
  dual_motor_driver.init();
  dual_motor_driver.motora.setVelocity(wheel_speed);
  dual_motor_driver.motorb.setVelocity(wheel_speed);
  while (true){
    
  }
}

void loop(){}