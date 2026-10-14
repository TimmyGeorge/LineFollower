#ifndef LINE_SENSOR_HPP
#define LINE_SENSOR_HPP

#include "Emitter.hpp"
#include "Line_Sensor_Array.hpp"

/*
class Line_Sensor : public Dual_Emitter, public Line_Sensor_Array{
  public:
    Dual_Emitter dual_emitter;
    Line_Sensor_Array sensor_array;
  public:
    Line_Sensor(Dual_Emitter&& _dual_emitter, Line_Sensor_Array&& _sensor_array);
};
*/

//Ok I have been informed that multiple inheritance is a much better idea than using composition.
//Hence I will implement that now

template <unsigned char num_sensors>
class Line_Sensor : public Dual_Emitter, public Line_Sensor_Array<num_sensors>{
  public:
    Line_Sensor(Dual_Emitter&& _dual_emitter, Line_Sensor_Array<num_sensors>&& _sensor_array);
};


#include "Line_Sensor.tpp"
#endif