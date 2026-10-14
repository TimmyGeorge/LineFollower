#include "Robot.hpp"

float Dual_Motor_Drive_Info::clamp(float velocity) const {
  if (velocity < min_velocity)
    return min_velocity;
  else if (velocity > max_velocity)
    return max_velocity;
  else
    return velocity;
}




Num_Steps_Step_Size::Num_Steps_Step_Size(unsigned long num_steps, float step_size): 
  Velocity_Change_Rate_Info(), 
  num_steps(num_steps), 
  step_size(step_size) 
{}


unsigned long Num_Steps_Step_Size::getNumSteps() const {
  return num_steps; 
}


float Num_Steps_Step_Size::getStepSize() const {
  return step_size; 
}




Num_Steps_Final_Velocity::Num_Steps_Final_Velocity(unsigned long num_steps, float final_velocity): 
  Velocity_Change_Rate_Info(), 
  num_steps(num_steps), 
  final_velocity(final_velocity)
{}


unsigned long Num_Steps_Final_Velocity::getNumSteps() const {
  return num_steps;
}


float Num_Steps_Final_Velocity::getStepSize() const {
  return (final_velocity - initial_velocity) / num_steps;
}




Final_Velocity_Step_Size::Final_Velocity_Step_Size(float final_velocity, float step_size):
  Velocity_Change_Rate_Info(), 
  final_velocity(final_velocity), 
  step_size(step_size)
{}


unsigned long Final_Velocity_Step_Size::getNumSteps() const {
    return (final_velocity - initial_velocity) / step_size;
}


float Final_Velocity_Step_Size::getStepSize() const {
  return step_size;
}



Three_Dual_Emitters::Three_Dual_Emitters(unsigned char maximum_dimming_level, unsigned char minimum_dimming_level, unsigned int pulse_time, unsigned int turn_off_time, unsigned char aodd, unsigned char aeven, unsigned char bodd, unsigned char beven, unsigned char codd, unsigned char ceven):
  Multiple_Emitters<getNumEmitters()>(maximum_dimming_level, minimum_dimming_level, pulse_time, turn_off_time, aodd, aeven, bodd, beven, codd, ceven)
{}


Dual_Emitter_Reference Three_Dual_Emitters::getDualEmitter(Dual_Emitter_Character index){
  return getDualEmitter(index, index + 1);
}


Single_Emitter_Reference Three_Dual_Emitters::getEmitter(Single_Emitter_Character index){
  return getEmitter((unsigned char)index);
}




Robot::Robot(Line_Sensor_Array_Read_Info&& read_info, Dual_Motor_Drive_Info&& drive_info, Antiwindup_PID_Controller&& controller, Dual_Motor_Driver_With_Standby&& driver, Line_Sensor_Array<QTRXHD07RC_NUM_SENSORS>&& sensor, Three_Dual_Emitters&& emitters) : 
  read_info((Line_Sensor_Array_Read_Info&&) read_info),
  drive_info((Dual_Motor_Drive_Info&&) drive_info),
  pid_controller((Antiwindup_PID_Controller&&) controller),
  tb6612fng((Dual_Motor_Driver_With_Standby&&) driver),
  qtrxhd07rc((Line_Sensor_Array<QTRXHD07RC_NUM_SENSORS>&&) sensor),
  qtrxhd07rc_emitters((Three_Dual_Emitters&&) emitters)
{}


float Robot::approximateInitialKp(float maximum_differential, Sensor_Position_Calculator* sensor_position_calculator, float num_sensors){
  return maximum_differential / Math::absolute(sensor_position_calculator->getMiddlePosition(num_sensors) - sensor_position_calculator->calcPositionalValue(0, num_sensors));
}


Robot& Robot::create(){
  unsigned char sensor_pins[QTRXHD07RC_NUM_SENSORS] = QTRXHD07RC_SENSOR_PINS;
  static Robot robot(
    Line_Sensor_Array_Read_Info {
      QTRXHD07RC_ON_LINE_THRESHOLD,
      0
    },
    Dual_Motor_Drive_Info {
      ROBOT_BASE_VELOCITY,
      MOTOR_MINIMUM_VELOCITY,
      MOTOR_MAXIMUM_VELOCITY,
      MOTOR_MINIMUM_ACCEPTABLE_VELOCITY_CHANGE_STEP_NUMBER
    },
    Antiwindup_PID_Controller(
      ANTIWINDUP_PID_CONTROLLER_KP,
      ANTIWINDUP_PID_CONTROLLER_KD,
      ANTIWINDUP_PID_CONTROLLER_KI,
      ANTIWINDUP_PID_CONTROLLER_UPPER_CLAMPING_SATURATION_LIMIT,
      ANTIWINDUP_PID_CONTROLLER_LOWER_CLAMPING_SATURATION_LIMIT
    ),
    Dual_Motor_Driver_With_Standby(
      TB6612FNG_AIN2, 
      TB6612FNG_AIN1, 
      TB6612FNG_PWMA,
      TB6612FNG_BIN2, 
      TB6612FNG_BIN1,  
      TB6612FNG_PWMB,
      TB6612FNG_STBY
    ),
    Line_Sensor_Array<QTRXHD07RC_NUM_SENSORS>(
      Calibrated_Sensor_Array<QTRXHD07RC_NUM_SENSORS>(
        Sensor_Array<QTRXHD07RC_NUM_SENSORS>(
          sensor_pins,
          QTRXHD07RC_LINE_SENSOR_CHARGE_TIME,
          QTRXHD07RC_LINE_SENSOR_TIMEOUT
        ),
        QTRXHD07RC_LINE_SENSOR_MINIMUM_POSSIBLE_READING
      ),
      LINE_POSITION_ESTIMATOR,
      SENSOR_POSITION_CALCULATOR
    ),
    Three_Dual_Emitters(
      QTRXHD07RC_EMITTER_MAXIMUM_DIMMING_LEVEL,
      QTRXHD07RC_EMITTER_MINIMUM_DIMMING_LEVEL,
      QTRXHD07RC_EMITTER_PULSE_TIME,
      QTRXHD07RC_EMITTER_TURN_OFF_TIME,
      QTRXHD07RC_AODD,
      QTRXHD07RC_AEVEN,
      QTRXHD07RC_BODD,
      QTRXHD07RC_BEVEN,
      QTRXHD07RC_CODD,
      QTRXHD07RC_CEVEN
    )
  );
  return robot;
} 


void Robot::calibrateLineSensorAutomatically(const unsigned long half_turn_time, const float wheel_velocity){
  OUT(Serial.println("Calibrating Line Sensor"));
  Millisecond_Timer timer;
  auto rotateAndCalibrate = [this, &timer](unsigned long rotation_time, float wheel_velocity){
    timer.start();
    this->tb6612fng.setVelocities(0, wheel_velocity);
    while(timer.seeTime() < rotation_time)
      this->qtrxhd07rc.calibrateOnce();
  };
  rotateAndCalibrate(half_turn_time, wheel_velocity);
  rotateAndCalibrate(2*half_turn_time, -wheel_velocity);
  rotateAndCalibrate(half_turn_time/2, wheel_velocity);
}


void Robot::calibrateLineSensorManually(unsigned long time){
  OUT(Serial.println("Calibrating Line Sensor"));
  qtrxhd07rc.calibrateByTime(time);
  qtrxhd07rc.trimCalibrationValues(0.3);
  OUT(Serial.println("Calibration Ended"));
}


void Robot::graduallyChangeVelocity(Motor_Driver* motor_driver, const Velocity_Change_Rate_Info& velocity_change_rate_info){
  OUT(Microsecond_Timer timer);
  OUT(timer.start());
  velocity_change_rate_info.initial_velocity = motor_driver->getDriveVelocity();
  float start_velocity = velocity_change_rate_info.initial_velocity;
  float step_size = velocity_change_rate_info.getStepSize();
  unsigned long num_steps = velocity_change_rate_info.getNumSteps();
  for(unsigned long step_num = 1; step_num < num_steps + 1; ++step_num){
    motor_driver->setVelocity(step_num * step_size + start_velocity);
  }
  OUT(unsigned long time = timer.seeTime());
  OUT(MySerial::print("Time Taken For Acceleraton: ", time, " us", ' '));
  OUT(MySerial::print("Velocity Change Per Increment ", step_size, ""));
}


Robot::Sensor_Data Robot::obtainSensorData(const unsigned int sample_size){
  OUT(Millisecond_Timer timer);
  OUT(timer.start());
  repeat:
  constexpr unsigned char num_sensors = decltype(qtrxhd07rc)::getNumSensors();
  Sensor_Data total = {0, 0, 0, 0, 0};
  for(unsigned int i = 0; i < sample_size; ++i){
    Calibrated_Readings<num_sensors> calibrated_readings = qtrxhd07rc.takeCalibratedReadings();
    calibrated_readings.invert();
    Line_Position_Status line_position_status = qtrxhd07rc.estimateLinePositionStatusUsing(calibrated_readings, read_info.on_line_threshold);
    if (line_position_status.status == Line_Position_Status::on_line){
      total.position += line_position_status.estimate;
      total.XL += calibrated_readings.get()[0];
      total.L += calibrated_readings.get()[1];
      total.XR += calibrated_readings.get()[num_sensors - 1];
      total.R += calibrated_readings.get()[num_sensors - 2];
    }
    else {
      goto repeat;
    }
  }
  total /= sample_size;
  OUT(unsigned long time = timer.seeTime());
  OUT(MySerial::print("Reading Time: ", time, "ms ", '\0'));
  return total;
}


float Robot::calcAveragePosition(const unsigned int sample_size){
  OUT(Millisecond_Timer timer);
  OUT(timer.start());
  repeat:
  float position_total = 0;
  for(unsigned int i = 0; i < sample_size; ++i){
    Line_Position_Status line_position_status = qtrxhd07rc.estimateWhiteLinePositionStatus(read_info.on_line_threshold);
    if (line_position_status.status == Line_Position_Status::on_line)
      position_total += line_position_status.estimate;
    else
      goto repeat;
  }
  OUT(unsigned long time = timer.seeTime());
  OUT(MySerial::print("Reading Time: ", time, "ms ", '\0'));
  return position_total / sample_size;
}


void Robot::OldfollowLinePID(){
  constexpr unsigned char num_sensors = decltype(qtrxhd07rc)::getNumSensors();
  Calibrated_Readings<num_sensors> calibrated_readings = qtrxhd07rc.takeCalibratedReadings();
  calibrated_readings.invert();
  if (calibrated_readings.get()[0] > read_info.on_line_threshold && calibrated_readings.get()[num_sensors - 1] < read_info.on_line_threshold){
    tb6612fng.motora.setVelocity(drive_info.max_velocity);
    tb6612fng.motorb.setVelocity(drive_info.min_velocity);
    return;
  } 
  else if  (calibrated_readings.get()[0] < read_info.on_line_threshold && calibrated_readings.get()[num_sensors - 1] > read_info.on_line_threshold){
    tb6612fng.motora.setVelocity(drive_info.min_velocity);
    tb6612fng.motorb.setVelocity(drive_info.max_velocity);
    return;
  } 
  Line_Position_Status line_position_status = qtrxhd07rc.estimateLinePositionStatusUsing(calibrated_readings, read_info.on_line_threshold);
  OUT(Line_Position_Status real_status = line_position_status);
  if (line_position_status.status != Line_Position_Status::on_line)
    line_position_status.estimate = read_info.last_valid_position;
  else 
    read_info.last_valid_position = line_position_status.estimate;
  const float line_position_error = qtrxhd07rc.getMiddlePosition() - line_position_status.estimate;
  const float wheel_differential = pid_controller.calcControllerOutput(line_position_error);
  const float motora_velocity = drive_info.clamp(drive_info.base_velocity + wheel_differential);
  const float motorb_velocity = drive_info.clamp(drive_info.base_velocity - wheel_differential);
  tb6612fng.motora.setVelocity(motora_velocity);
  tb6612fng.motorb.setVelocity(motorb_velocity);
  OUT(MySerial::printArray("calibrated_readings = ", calibrated_readings.get(), 7, ""));
  OUT(MySerial::print("real position = ", real_status.estimate, ""));
  OUT(MySerial::print("Line Position = ", line_position_status.estimate, ""));
  OUT(MySerial::print("line_position_error = ", line_position_error, ""));
  OUT(MySerial::print("wheel_differential = ", wheel_differential, ""));
  OUT(MySerial::print("motor a velocity = ", motora_velocity, ""));
  OUT(MySerial::print("motor b velocity = ", motorb_velocity, ""));
  OUT(Serial.println());
  OUT(delay(100));
}


void Robot::followLinePID(){
  Line_Position_Status line_position_status = qtrxhd07rc.estimateWhiteLinePositionStatus();
  OUT(Line_Position_Status real_status = line_position_status);
  if (line_position_status.status != Line_Position_Status::on_line)
    line_position_status.estimate = read_info.last_valid_position;
  else 
    read_info.last_valid_position = line_position_status.estimate;
  const float line_position_error = qtrxhd07rc.getMiddlePosition() - line_position_status.estimate;
  const float wheel_differential = pid_controller.calcControllerOutput(line_position_error);
  const float motora_velocity = drive_info.clamp(drive_info.base_velocity + wheel_differential);
  const float motorb_velocity = drive_info.clamp(drive_info.base_velocity - wheel_differential);
  tb6612fng.motora.setVelocity(motora_velocity);
  tb6612fng.motorb.setVelocity(motorb_velocity);
  OUT(MySerial::printArray("calibrated_readings = ", calibrated_readings.get(), 7, ""));
  OUT(MySerial::print("real position = ", real_status.estimate, ""));
  OUT(MySerial::print("Line Position = ", line_position_status.estimate, ""));
  OUT(MySerial::print("line_position_error = ", line_position_error, ""));
  OUT(MySerial::print("wheel_differential = ", wheel_differential, ""));
  OUT(MySerial::print("motor a velocity = ", motora_velocity, ""));
  OUT(MySerial::print("motor b velocity = ", motorb_velocity, ""));
  OUT(Serial.println());
  OUT(delay(100));
}


void Robot::followLinePIDOutputIsVelocityChange(){
  const float position = calcAveragePosition(10);
  OUT(Line_Position_Status real_status = line_position_status);
  const float line_position_error = qtrxhd07rc.getMiddlePosition() - position;
  const float wheel_differential = pid_controller.calcControllerOutput(line_position_error);
  const float motora_velocity = drive_info.clamp(drive_info.base_velocity + wheel_differential);
  const float motorb_velocity = drive_info.clamp(drive_info.base_velocity - wheel_differential);
  tb6612fng.motora.setVelocity(motora_velocity);
  tb6612fng.motorb.setVelocity(motorb_velocity);
  OUT(MySerial::print("Line Position = ", position, ""));
  OUT(MySerial::print("line_position_error = ", line_position_error, ""));
  OUT(MySerial::print("wheel_differential = ", wheel_differential, ""));
  OUT(MySerial::print("motor a velocity = ", motora_velocity, ""));
  OUT(MySerial::print("motor b velocity = ", motorb_velocity, ""));
  OUT(Serial.println());
  OUT(delay(100));
}


void Robot::followLinePIDWithPositionAveraging(){
  const float position = calcAveragePosition(10);
  const float line_position_error = qtrxhd07rc.getMiddlePosition() - position;
  const float wheel_differential = pid_controller.calcControllerOutput(line_position_error);
  const float motora_velocity = drive_info.clamp(drive_info.base_velocity + wheel_differential);
  const float motorb_velocity = drive_info.clamp(drive_info.base_velocity - wheel_differential);
  tb6612fng.motora.setVelocity(motora_velocity);
  tb6612fng.motorb.setVelocity(motorb_velocity);
  OUT(MySerial::print("Line Position = ", position, ""));
  OUT(MySerial::print("line_position_error = ", line_position_error, ""));
  OUT(MySerial::print("wheel_differential = ", wheel_differential, ""));
  OUT(MySerial::print("motor a velocity = ", motora_velocity, ""));
  OUT(MySerial::print("motor b velocity = ", motorb_velocity, ""));
  OUT(Serial.println());
  OUT(delay(100));
}


void Robot::followLinePIDWithPositionAveragingAnd90Turns(){
  Sensor_Data position = obtainSensorData(10);
  bool left_sensor_is_on_line = position.XL > read_info.on_line_threshold;
  bool right_sensor_is_on_line = position.XR > read_info.on_line_threshold;
  if (!left_sensor_is_on_line && right_sensor_is_on_line){
    tb6612fng.motora.setVelocity(drive_info.min_velocity);
    tb6612fng.motorb.setVelocity(drive_info.max_velocity);
    OUT(MySerial::print("Line Position = ", position.position, ""));
    OUT(Serial.println("Turning Left"));
  }
  else if (left_sensor_is_on_line && !right_sensor_is_on_line){
    tb6612fng.motora.setVelocity(drive_info.max_velocity);
    tb6612fng.motorb.setVelocity(drive_info.min_velocity);
    OUT(MySerial::print("Line Position = ", position, ""));
    OUT(Serial.println("Turning Right"));
  }
  else {
    const float line_position_error = qtrxhd07rc.getMiddlePosition() - position.position;
    const float wheel_differential = pid_controller.calcControllerOutput(line_position_error);
    const float motora_velocity = drive_info.clamp(drive_info.base_velocity + wheel_differential);
    const float motorb_velocity = drive_info.clamp(drive_info.base_velocity - wheel_differential);
    tb6612fng.motora.setVelocity(motora_velocity);
    tb6612fng.motorb.setVelocity(motorb_velocity);
    OUT(MySerial::print("Line Position = ", position.position, ""));
    OUT(MySerial::print("line_position_error = ", line_position_error, ""));
    OUT(MySerial::print("wheel_differential = ", wheel_differential, ""));
    OUT(MySerial::print("motor a velocity = ", motora_velocity, ""));
    OUT(MySerial::print("motor b velocity = ", motorb_velocity, ""));
    OUT(Serial.println());
    OUT(delay(100));
  }
}

void Robot::followLinePIDWithPositionAveragingAnd90TurnsMinOscillation(){
  Sensor_Data position = obtainSensorData(10);
  bool left_sensor_is_on_line = position.XL > read_info.on_line_threshold;
  bool right_sensor_is_on_line = position.XR > read_info.on_line_threshold;
  const unsigned int point_turn_time = 10*1000;
  if (!left_sensor_is_on_line && right_sensor_is_on_line){
    tb6612fng.motora.setVelocity(drive_info.min_velocity);
    tb6612fng.motorb.setVelocity(drive_info.max_velocity);
    delayMicroseconds(point_turn_time);
    OUT(MySerial::print("Line Position = ", position.position, ""));
    OUT(Serial.println("Turning Left"));
  }
  else if (left_sensor_is_on_line && !right_sensor_is_on_line){
    tb6612fng.motora.setVelocity(drive_info.max_velocity);
    tb6612fng.motorb.setVelocity(drive_info.min_velocity);
    delayMicroseconds(point_turn_time);
    OUT(MySerial::print("Line Position = ", position, ""));
    OUT(Serial.println("Turning Right"));
  }
  const float line_position_error = qtrxhd07rc.getMiddlePosition() - position.position;
  const float wheel_differential = pid_controller.calcControllerOutput(line_position_error);
  const float motora_velocity = drive_info.clamp(drive_info.base_velocity + wheel_differential);
  const float motorb_velocity = drive_info.clamp(drive_info.base_velocity - wheel_differential);
  tb6612fng.motora.setVelocity(motora_velocity);
  tb6612fng.motorb.setVelocity(motorb_velocity);
  OUT(MySerial::print("Line Position = ", position.position, ""));
  OUT(MySerial::print("line_position_error = ", line_position_error, ""));
  OUT(MySerial::print("wheel_differential = ", wheel_differential, ""));
  OUT(MySerial::print("motor a velocity = ", motora_velocity, ""));
  OUT(MySerial::print("motor b velocity = ", motorb_velocity, ""));
  OUT(Serial.println());
  OUT(delay(100));
}


void Robot::followLineSimpleWith90Turns(){
  //This fails as sensor is too small and nearly all sensors lie on the line - it just keeps moving side to side without moving forward
  //line_sensor.trimCalibrationValues(0.4);
  const float min_speed = 0;
  const float max_speed = 0.2;
  const unsigned int sample_size = 5;
  Sensor_Data sensor_data = obtainSensorData(sample_size);
  bool XL_is_on_line = sensor_data.XL > read_info.on_line_threshold;
  bool L_is_on_line = sensor_data.L > read_info.on_line_threshold;
  bool R_is_on_line = sensor_data.R > read_info.on_line_threshold;
  bool XR_is_on_line = sensor_data.XR > read_info.on_line_threshold;
  if ((XL_is_on_line && L_is_on_line && R_is_on_line && XR_is_on_line) || (!XL_is_on_line && !L_is_on_line && !R_is_on_line && !XR_is_on_line)){
    if (sensor_data.position < 0){
      tb6612fng.motora.setVelocity(max_speed);
      tb6612fng.motorb.setVelocity(min_speed);
    } 
    else {
      tb6612fng.motorb.setVelocity(max_speed);
      tb6612fng.motora.setVelocity(min_speed);
    }
  }
  if (L_is_on_line && XL_is_on_line){
    tb6612fng.motora.setVelocity(max_speed);
    tb6612fng.motorb.setVelocity(-max_speed);
  }
  else if (XR_is_on_line && R_is_on_line){
    tb6612fng.motora.setVelocity(-max_speed);
    tb6612fng.motorb.setVelocity(max_speed); 
  }
  OUT(MySerial::print("Position: ", sensor_data.position, "", ' '));
  OUT(MySerial::print("XL: ", sensor_data.XL, "", ' '));
  OUT(MySerial::print("L: ", sensor_data.L, "", ' '));
  OUT(MySerial::print("R: ", sensor_data.R, "", ' '));
  OUT(MySerial::print("XR: ", sensor_data.XR, ""));
  OUT(delay(1000));
}


void Robot::followLineSimple(){
  //line_sensor.trimCalibrationValues(0.4);
  const unsigned int sample_size = 5;
  const float position = calcAveragePosition(sample_size);
  if (position < -0.5){
    tb6612fng.motora.setVelocity(drive_info.base_velocity);
    tb6612fng.motorb.setVelocity(0);
  } 
  else if (position > 0.5) {
    tb6612fng.motorb.setVelocity(drive_info.base_velocity);
    tb6612fng.motora.setVelocity(0);
  }
  else {
    tb6612fng.motorb.setVelocity(drive_info.base_velocity);
    tb6612fng.motora.setVelocity(drive_info.base_velocity);
  }
  OUT(MySerial::print("Position: ", position, ""));
  OUT(delay(1000));
}


void Robot::checkDimmingLevelsAreWorkingManual(){
  for (int I = 1; I < 33; ++I){
    constexpr unsigned char num_sensors = decltype(this->qtrxhd07rc)::getNumSensors(); unsigned int raw_readings[num_sensors];
    digitalWrite(QTRXHD07RC_AODD, LOW);
    digitalWrite(QTRXHD07RC_BODD, LOW);
    digitalWrite(QTRXHD07RC_CODD, LOW);
    digitalWrite(QTRXHD07RC_AEVEN, LOW);
    digitalWrite(QTRXHD07RC_BEVEN, LOW);
    digitalWrite(QTRXHD07RC_CEVEN, LOW);
    delayMicroseconds(2);
    digitalWrite(QTRXHD07RC_AODD, HIGH);
    digitalWrite(QTRXHD07RC_BODD, HIGH);
    digitalWrite(QTRXHD07RC_CODD, HIGH);
    digitalWrite(QTRXHD07RC_AEVEN, HIGH);
    digitalWrite(QTRXHD07RC_BEVEN, HIGH);
    digitalWrite(QTRXHD07RC_CEVEN, HIGH);
    qtrxhd07rc.takeRawReadings(raw_readings); 
    MySerial::print("Dimming Level", I % 32, ""); 
    MySerial::printArray("Raw: ", raw_readings, num_sensors, ""); 
    delay(1000);
  }
}


void Robot::checkIncrementDimmingLevelsAreWorking(){
  constexpr unsigned char num_emitters = decltype(qtrxhd07rc_emitters)::getNumEmitters();
  unsigned char dimming_levels[num_emitters];
  for (unsigned char i = 0; i < num_emitters; ++i){
    dimming_levels[i] = qtrxhd07rc_emitters.getDimmingLevel(i);
  }
  constexpr unsigned char num_sensors = decltype(qtrxhd07rc)::getNumSensors();
  unsigned int raw_readings[num_sensors];
  qtrxhd07rc.takeRawReadings(raw_readings);
  MySerial::printArray("Dim: ", dimming_levels, num_emitters, "");
  MySerial::printArray("Raw: ", raw_readings, num_sensors, "");
  Serial.println();
  qtrxhd07rc_emitters.incrementDimmingLevel();
  delay(1000);
}

void Robot::checkSetDimmingLevelsAreWorking(){
  constexpr unsigned char num_emitters = decltype(qtrxhd07rc_emitters)::getNumEmitters();
  constexpr unsigned char num_sensors = decltype(qtrxhd07rc)::getNumSensors();
  unsigned char dimming_levels_to_test[5] = {0, 31, 17, 12, 23};
  for (unsigned char dimming_level_to_test : dimming_levels_to_test){
    qtrxhd07rc_emitters.setDimmingLevel(dimming_level_to_test);
    unsigned char dimming_levels[num_emitters];
    for (unsigned char i = 0; i < num_emitters; ++i){
      dimming_levels[i] = qtrxhd07rc_emitters.getDimmingLevel(i);
    }
    unsigned int raw_readings[num_sensors];
    qtrxhd07rc.takeRawReadings(raw_readings);
    MySerial::printArray("Dim: ", dimming_levels, num_emitters, "");
    MySerial::printArray("Raw: ", raw_readings, num_sensors, "");
    Serial.println();
    delay(1000);
  }
}


void Robot::checkSensorsAreWorkingDigital(){
  constexpr unsigned char num_sensors = decltype(qtrxhd07rc)::getNumSensors();
  Calibrated_Readings<num_sensors> calibrated_readings = qtrxhd07rc.takeCalibratedReadings();
  for (int i = 0; i < num_sensors; ++i){
    double& reading = calibrated_readings.get()[i];
    reading = reading > 0.5 ? 1 : 0;
  }
  MySerial::printArray("Raw Readings", calibrated_readings.get(), num_sensors, "");
  delay(1000);
}


void Robot::checkSensorsAreWorkingAnalogue(){
  constexpr unsigned char num_sensors = decltype(qtrxhd07rc)::getNumSensors();
  unsigned int raw_readings[num_sensors];
  qtrxhd07rc.takeRawReadings(raw_readings);
  Calibrated_Readings<num_sensors> calibrated_readings;
  qtrxhd07rc.calibrateReadings(raw_readings, calibrated_readings.get());
  MySerial::printArray("Raw: ", raw_readings, num_sensors, "");
  MySerial::printArray("Cal: ", calibrated_readings.get(), num_sensors, "");
  Serial.println();
  delay(1000);
}

void Robot::checkMotorsAreWorking(){
  tb6612fng.motora.setVelocity(0.2);
  tb6612fng.motorb.setVelocity(0.2);
  delay(1000);
  tb6612fng.motora.setVelocity(0.3);
  tb6612fng.motorb.setVelocity(0.3);
  delay(1000);
  tb6612fng.motora.setVelocity(0.4);
  tb6612fng.motorb.setVelocity(0.4);
  delay(1000);
  tb6612fng.motora.setVelocity(0.5);
  tb6612fng.motorb.setVelocity(0.5);
  delay(1000);
  tb6612fng.motora.setVelocity(0.6);
  tb6612fng.motorb.setVelocity(0.6);
  delay(1000);
  tb6612fng.motora.setVelocity(0.7);
  tb6612fng.motorb.setVelocity(0.7);
  delay(1000);
  tb6612fng.motora.setVelocity(0.8);
  tb6612fng.motorb.setVelocity(0.8);
  delay(1000);
  tb6612fng.motora.setVelocity(0.9);
  tb6612fng.motorb.setVelocity(0.9);
  delay(1000);
  tb6612fng.motora.setVelocity(1.0);
  tb6612fng.motorb.setVelocity(1.0);
  delay(1000);
  tb6612fng.motora.setVelocity(0.9);
  tb6612fng.motorb.setVelocity(0.9);
  delay(1000);
  tb6612fng.motora.setVelocity(0.8);
  tb6612fng.motorb.setVelocity(0.8);
  delay(1000);
  tb6612fng.motora.setVelocity(0.7);
  tb6612fng.motorb.setVelocity(0.7);
  delay(1000);
  tb6612fng.motora.setVelocity(0.6);
  tb6612fng.motorb.setVelocity(0.6);
  delay(1000);
  tb6612fng.motora.setVelocity(0.5);
  tb6612fng.motorb.setVelocity(0.5);
  delay(1000);
  tb6612fng.motora.setVelocity(0.4);
  tb6612fng.motorb.setVelocity(0.4);
  delay(1000);
  tb6612fng.motora.setVelocity(0.3);
  tb6612fng.motorb.setVelocity(0.3);
  delay(1000);
}


void Robot::doNothing(){
}


void Robot::loop(){
  followLinePIDWithPositionAveragingAnd90TurnsMinOscillation();
}


void Robot::setup(){
  const unsigned char optimal_dimming_level = 27;
  qtrxhd07rc_emitters.init();
  qtrxhd07rc_emitters.turnOn();
  qtrxhd07rc_emitters.setDimmingLevel(optimal_dimming_level);
  tb6612fng.init();
  Serial.println("Calibrating");
  qtrxhd07rc.calibrateByTime(15000000ULL);
  Serial.println("Ended Calibraton");
}


void Robot::Sensor_Data::operator/=(const float scalar){
  position /= scalar;
  L /= scalar;
  XL /= scalar;
  R /= scalar;
  XR /= scalar;
}


