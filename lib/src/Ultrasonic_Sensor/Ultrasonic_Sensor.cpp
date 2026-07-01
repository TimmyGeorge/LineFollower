#include "Ultrasonic_Sensor.hpp"
#include <Timer/Timer.hpp>

const word Ultrasonic_Sensor::SPEED_OF_SOUND = 340;
const byte Ultrasonic_Sensor::RANGE = 4;
const float Ultrasonic_Sensor::MAXIMUM_CONVERTABLE_DISTANCE = (pow(2,8*sizeof(word))-1)*SPEED_OF_SOUND/(2*1000000);
const byte Ultrasonic_Sensor::DEFAULT_NUM_ITERATIONS = 3;
const word Ultrasonic_Sensor::MEASUREMENT_CYCLE = 60000;

word Ultrasonic_Sensor::convDistanceToEchoTime(float distance){
  //Can Detect Invalid Inputs for Echo_Time
  distance = abs(distance);
  return (distance > MAXIMUM_CONVERTABLE_DISTANCE)? 0 : distance/SPEED_OF_SOUND*2*1000000;
}

float Ultrasonic_Sensor::convEchoTimeToDistance(const word& echo_time){
  //Cannot Detect Invalid Inputs for echo_time.
  return ((float) echo_time*SPEED_OF_SOUND)/(2*1000000);
}

void Ultrasonic_Sensor::waitTillReadyForNextMeasurement(){
  delayMicroseconds(MEASUREMENT_CYCLE);
}
word Ultrasonic_Sensor::convDistanceToEchoTimeConstrained(float maximum_distance){
  return convDistanceToEchoTime(RANGE > abs(maximum_distance)? maximum_distance : RANGE);
}

Ultrasonic_Sensor::Ultrasonic_Sensor(const byte& _trig_pin, const byte& _echo_pin):
Ultrasonic_Sensor(_trig_pin, _echo_pin, RANGE){}

Ultrasonic_Sensor::Ultrasonic_Sensor(const byte& _trig_pin, const byte& _echo_pin, const float& maximum_distance):
Ultrasonic_Sensor(_trig_pin, _echo_pin, maximum_distance, DEFAULT_NUM_ITERATIONS){
}

Ultrasonic_Sensor::Ultrasonic_Sensor(const byte& _trig_pin, const byte& _echo_pin, const float& maximum_distance, const byte& _num_iterations):
trig_pin(_trig_pin), echo_pin(_echo_pin), 
maximum_echo_time(convDistanceToEchoTimeConstrained(maximum_distance)),
num_iterations(_num_iterations? _num_iterations : DEFAULT_NUM_ITERATIONS) {}

void Ultrasonic_Sensor::init() const {
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
}

void Ultrasonic_Sensor::setMaximumEchoTime(word _maximum_echo_time){
  word echo_time_for_range = convDistanceToEchoTime(RANGE);
  maximum_echo_time = (_maximum_echo_time > echo_time_for_range)? echo_time_for_range : _maximum_echo_time;
}

void Ultrasonic_Sensor::setMaximumDistance(float maximum_distance){
  maximum_echo_time = convDistanceToEchoTimeConstrained(maximum_distance);
}


word Ultrasonic_Sensor::sendPulse() const{
  //Clear The Trig Pin
  unsigned long time_taken = 0;
  Microsecond_Timer T;
  digitalWrite(trig_pin,LOW);
  delayMicroseconds(2);
  //Set Trig Pin High for 10us
  digitalWrite(trig_pin,HIGH);
  delayMicroseconds(10);
  
  digitalWrite(trig_pin,LOW);
  //Wait for EchoPin TO Go High
  while (digitalRead(echo_pin)==0){}
  //Time how Long The Echo Pin is High
  T.start();
  while (digitalRead(echo_pin) && time_taken < maximum_echo_time){
    time_taken = T.seeTime();
  }
  return time_taken;
}

float Ultrasonic_Sensor::getDistance() const {
  word pulse_time = sendPulse();
  return convEchoTimeToDistance(pulse_time);
}

word Ultrasonic_Sensor::getMedianPulseTime() const {
  if (num_iterations == 0){
    return sendPulse();
  }
  word pulse_times[num_iterations] = {0};
  word pulse_time;
  byte upper_index, lower_index, middle_index;
  byte position;
  for(int i = 0; i < num_iterations; ++i){
    pulse_time = sendPulse();
    //Begin a binary search to find where to insert pulse_time in pulse_times
    upper_index = num_iterations-1;
    lower_index = num_iterations-i;
    position = upper_index;
    while (lower_index <= upper_index){
      middle_index = (upper_index + lower_index)/2;
      if (pulse_time < pulse_times[middle_index]){
        upper_index = middle_index - 1;
        position = upper_index;
      }
      else{
        lower_index = middle_index + 1;
      }
    }
    //Insert pulse_time in pulse_times
    for(byte j = num_iterations-i; j < position+1; j++){
      pulse_times[j-1]=pulse_times[j];
    }
    pulse_times[position] = pulse_time;
    waitTillReadyForNextMeasurement();
  }
  return pulse_times[(num_iterations-1)/2];
}

float Ultrasonic_Sensor::getMedianDistance() const{
  return convEchoTimeToDistance(getMedianPulseTime());
}


