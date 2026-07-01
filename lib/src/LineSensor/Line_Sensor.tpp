template <unsigned char num_sensors>
Line_Sensor<num_sensors>::Line_Sensor(Dual_Emitter&& _dual_emitter, Line_Sensor_Array<num_sensors>&& _sensor_array):
Dual_Emitter(_dual_emitter), Line_Sensor_Array<num_sensors>((Line_Sensor_Array<num_sensors>&&)_sensor_array){}
