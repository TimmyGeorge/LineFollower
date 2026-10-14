#include <LineFollower.hpp>
#include <LineSensor/Line_Sensor.hpp>
#include <Serial/Serial.hpp>
#include <Timer/Timer.hpp>
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
    float total = 0;
    unsigned int count = 0;
    
    for(unsigned int i = 0; i < num_readings_to_average; ++i){
      Line_Position_Status line_position_status = line_sensor.estimateWhiteLinePositionStatus();
      if (line_position_status.status == Line_Position_Status::on_line){
        total += line_position_status.estimate;
        count += 1;
      }
    }
    unsigned int time = timer.seeTime();
    
    MySerial::print("Time: ", time, "ms ", '\0');
    MySerial::print("count: ", count, " ", '\0');
    MySerial::print("Position: ", total / count, "");

    delay(1000);
  }
}

void loop(){}