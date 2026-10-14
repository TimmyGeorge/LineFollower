#include <Line_Sensor.hpp>
#include <MySerial.hpp>
#include <Timer.hpp>
#include <Motor_Driver.hpp>

#define OUT(code) 
#define DELAY(code) 

constexpr float wheel_speed = 0.2;
Dual_Motor_Driver dual_motor_driver(22, 23, 4, 24, 25, 5);
unsigned char sensor_pins[7] = {36, 37, 38, 39, 40, 41, 42};
Line_Sensor<7> line_sensor(
  Dual_Emitter(
    45, 44, 31, 0, 2, 1500
  ),
  Line_Sensor_Array<7>(
    Calibrated_Sensor_Array<7>(
      Sensor_Array<7>(
        sensor_pins,
        20,
        60000
      ),
      100
    ),
    Line_Position_Algorithm::calcQuadraticInterpolatedMax,
    &sensor_position_calculator_with_negative_positions
  )
);

void setup(){
  Serial.begin(9600);
  Serial.println("I Have Setup");
  Serial.println("I Have Setup");
  Serial.println("I Have Setup");
  dual_motor_driver.init();
  line_sensor.init();
  line_sensor.turnOn();
  line_sensor.setDimmingLevel(20);
  
  Serial.println("Calibrating");
  Millisecond_Timer timer;
  while(timer.seeTime() < 10000){
    line_sensor.calibrateOnce();
  }

  //line_sensor.trimCalibrationValues(0.4);
  const unsigned int num_readings_to_average = 5;
  Serial.println("Finished Calibrating");
  while (true){
    timer.start();
    bool dont_enter_here_again = false;
    repeat:
      float total = 0;
      unsigned int count = 0;
      for(unsigned int i = 0; i < num_readings_to_average; ++i){
        Calibrated_Readings<7> calibrated_readings = line_sensor.takeCalibratedReadings().invert();
        Line_Position_Status line_position_status = line_sensor.line_position_estimator(calibrated_readings.get(), line_sensor.getNumReadings());
        line_position_status.estimate = line_sensor.calcPositionalValue(line_position_status.estimate);
        if (line_position_status.status == Line_Position_Status::on_line){
          total += line_position_status.estimate;
          ++count;
        } else {
          goto repeat;
        }
      }
    
    if ()
    if (total / count < 0){
      dual_motor_driver.setVelocities(0, 0, wheel_speed);
    } else{
      dual_motor_driver.setVelocities(0, wheel_speed, 0);
    }

    OUT(unsigned int time = timer.seeTime();)
    OUT(MySerial::print("Time: ", time, "ms ", '\0'));
    OUT(MySerial::print("count: ", count, " ", '\0'));
    OUT(MySerial::print("Position: ", total / count, ""));

    DELAY(delay(1000));
  }
}

void loop(){}