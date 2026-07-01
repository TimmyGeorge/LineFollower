#define SERIAL_PRINTS_ENABLED 0
#include "Robot.hpp"


void setup(){
  OUT(Serial.begin(9600));
  OUT(Serial.println("I have setup!"));
  OUT(Serial.println("I have setup!"));
  OUT(Serial.println("I have setup!"));
  Robot& robot = Robot::create();
  robot.setup();
  while (true){
    robot.loop();
  }
}

void loop(){}