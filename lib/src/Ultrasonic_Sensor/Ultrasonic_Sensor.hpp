#ifndef ULTRASONIC_SENSOR_HPP
#define ULTRASONIC_SENSOR_HPP
#include <Arduino.h>
/*
Note that all distances are in Meters
Note that all times are in Microseconds
*/
class Ultrasonic_Sensor{
  friend void debug(const Ultrasonic_Sensor& US);
  //Static Member Variables
  public:
    static const word SPEED_OF_SOUND;
    static const byte RANGE;
    static const word MEASUREMENT_CYCLE;
  private:
    static const float MAXIMUM_CONVERTABLE_DISTANCE;
    static const byte DEFAULT_NUM_ITERATIONS;
  //Member Variables
  private: 
    byte trig_pin;
    byte echo_pin;
    word maximum_echo_time;
  public:
    byte num_iterations;
  //Constructors
  public:
    Ultrasonic_Sensor(const byte& _trig_pin, const byte& _echo_pin); //C
    Ultrasonic_Sensor(const byte& _trig_pin, const byte& _echo_pin, const float& maximum_distance); //C
    Ultrasonic_Sensor(const byte& _trig_pin, const byte& _echo_pin, const float& maximum_distance, const byte& _num_iterations); //C
  //Member Functions
    //Static Member Functions
  public:
    static float convEchoTimeToDistance(const word& echo_time); //c
    static word convDistanceToEchoTime(float distance); //c
    static void waitTillReadyForNextMeasurement();
  private:
    word convDistanceToEchoTimeConstrained(float maximum_distance); //C
  public:
    void init() const;
    void setMaximumEchoTime(word _maximum_echo_time);
    void setMaximumDistance(float maximum_distance);
    word sendPulse() const;
    float getDistance() const;
    word getMedianPulseTime() const;
    float getMedianDistance() const;
};
#endif