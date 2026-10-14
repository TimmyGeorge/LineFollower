#include <LineFollower.hpp>
#include <Serial/Serial.hpp>
#include <Math/Math.hpp>
#include <Timer/Timer.hpp>
#include <PID_Controller/PID_Controller.hpp>
#include <LineSensor/Line_Sensor_Array.hpp>
#include <LineSensor/Emitter.hpp>
#include <MotorDriver/Motor_Driver.hpp>
#include "RobotConstants.hpp"
#include "Out.hpp"

struct Line_Sensor_Array_Read_Info {
  float on_line_threshold;
  float last_valid_position;
};


struct Dual_Motor_Drive_Info {
  public:
    float base_velocity;
    float min_velocity;
    float max_velocity;
    unsigned long min_acceptable_velocity_change_step_num;
  public:
    float clamp(float velocity) const;
};


struct Velocity_Change_Rate_Info{
    mutable float initial_velocity;
  public:
    virtual unsigned long getNumSteps() const = 0;
    virtual float getStepSize() const = 0;
};


struct Num_Steps_Step_Size : public Velocity_Change_Rate_Info{
  public:
    unsigned long num_steps;
    float step_size;
  public:
    Num_Steps_Step_Size(unsigned long num_steps, float step_size);
    virtual unsigned long getNumSteps() const override;
    virtual float getStepSize() const override;
};


struct Num_Steps_Final_Velocity : public Velocity_Change_Rate_Info{
  public:
    unsigned long num_steps;
    float final_velocity;
  public:
    Num_Steps_Final_Velocity(unsigned long num_steps, float final_velocity);
    virtual unsigned long getNumSteps() const override;
    virtual float getStepSize() const override;
};


struct Final_Velocity_Step_Size : public Velocity_Change_Rate_Info{
  float final_velocity;
  float step_size;
  Final_Velocity_Step_Size(float final_velocity, float step_size);
  virtual unsigned long getNumSteps() const override;
  virtual float getStepSize() const override;
};


class Three_Dual_Emitters : public Multiple_Emitters<6> {
    using Multiple_Emitters<getNumEmitters()>::getDualEmitter;
    using Multiple_Emitters<getNumEmitters()>::getEmitter; 
  public:
    enum Dual_Emitter_Character : unsigned char {
      A = 0, 
      B = 2,
      C = 4
    };
    enum Single_Emitter_Character : unsigned char {
      AODD,
      AEVEN,
      BODD,
      BEVEN,
      CODD,
      CEVEN
    };
  public:
    Three_Dual_Emitters(unsigned char maximum_dimming_level, unsigned char minimum_dimming_level, unsigned int pulse_time, unsigned int turn_off_time, unsigned char aodd, unsigned char aeven, unsigned char bodd, unsigned char beven, unsigned char codd, unsigned char ceven);
    Three_Dual_Emitters(Three_Dual_Emitters&&) = default;
  public:
    Dual_Emitter_Reference getDualEmitter(Dual_Emitter_Character);
    Single_Emitter_Reference getEmitter(Single_Emitter_Character);
};


class Robot {
  private:
    static constexpr unsigned char num_sensors = QTRXHD07RC_NUM_SENSORS;
  private:
    Line_Sensor_Array_Read_Info read_info;
    Dual_Motor_Drive_Info drive_info;
    Antiwindup_PID_Controller pid_controller;
    Dual_Motor_Driver_With_Standby tb6612fng;
    Line_Sensor_Array<QTRXHD07RC_NUM_SENSORS> qtrxhd07rc;
    Three_Dual_Emitters qtrxhd07rc_emitters;
  private:
    struct Sensor_Data;
  private:
    Robot() = delete;
    Robot(Line_Sensor_Array_Read_Info&&, Dual_Motor_Drive_Info&&, Antiwindup_PID_Controller&&, Dual_Motor_Driver_With_Standby&&, Line_Sensor_Array<QTRXHD07RC_NUM_SENSORS>&&, Three_Dual_Emitters&&);
  public:
    static float approximateInitialKp(float maximum_differential, Sensor_Position_Calculator*, float num_sensors);
    static Robot& create();
  private:
    void calibrateLineSensorAutomatically(const unsigned long half_turn_time, const float wheel_velocity);
    void calibrateLineSensorManually(unsigned long time = 10000000);
    void graduallyChangeVelocity(Motor_Driver*, const Velocity_Change_Rate_Info&);
    Sensor_Data obtainSensorData(const unsigned int sample_size);
    float calcAveragePosition(const unsigned int sample_size);
    //loop functions
    void OldfollowLinePID();
    void followLinePID();
    void followLinePIDOutputIsVelocityChange();
    void followLinePIDWithPositionAveraging();  //Works Successfully bv=0.3 minv=-0.3 maxv=0.3
    void followLinePIDWithPositionAveragingAnd90Turns(); //Works but causes oscillations
    void followLinePIDWithPositionAveragingAnd90TurnsMinOscillation(); 
    void followLineSimpleWith90Turns();
    void followLineSimple();
    void checkDimmingLevelsAreWorkingManual();
    void checkIncrementDimmingLevelsAreWorking();
    void checkSetDimmingLevelsAreWorking();
    void checkSensorsAreWorkingDigital();
    void checkSensorsAreWorkingAnalogue();
    void checkMotorsAreWorking();
    void doNothing();
  public:
    void loop();
    void setup();
};


struct Robot::Sensor_Data {
  public:
    float position;
    float L;
    float XL;
    float R;
    float XR;
  public:
    void operator/=(const float scalar);
};