#include <LineFollower.hpp>
#include <LineSensor/Sensor_Array.hpp>
#include <Serial/Serial.hpp>

#define SENSOR_PINS {46, 52, 44, 47, 53, 48, 50}
#define NUM_SENSORS 7
#define CHARGE_TIME 10
#define TIMEOUT 60000

Sensor_Array<NUM_SENSORS> qtrx((unsigned char[])SENSOR_PINS, CHARGE_TIME, TIMEOUT);

void setup(){
  Serial.begin(9600);
  Serial.println("I Have setup");
  Serial.println("I Have setup");
  Serial.println("I Have setup");
}


void loop(){
  unsigned int raw_readings[NUM_SENSORS];
  qtrx.takeRawReadings(raw_readings);
  MySerial::printArray("", raw_readings, NUM_SENSORS, "");
  delay(1000);
}