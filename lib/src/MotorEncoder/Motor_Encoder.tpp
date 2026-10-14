template <typename Travel_Type>
Motor_Travel<Travel_Type>::Motor_Travel(Travel_Type _count, const Motor_Encoder_Constants* _p_encoder_constants):
count(_count),
p_encoder_constants(_p_encoder_constants){}


template <typename Travel_Type>
Motor_Travel<Travel_Type> Motor_Travel<Travel_Type>::createUsingCount(Travel_Type count, const Motor_Encoder* _p_encoder){
  return Motor_Travel<Travel_Type>(count, _p_encoder->getConstants());
}


template <typename Travel_Type>
Motor_Travel<Travel_Type> Motor_Travel<Travel_Type>::createUsingMeters(float meters, const Motor_Encoder* _p_encoder){
  const Motor_Encoder_Constants* encoder_constants = _p_encoder->getConstants();
  float counts = meters / (Math::pi * encoder_constants->wheel_diameter) * encoder_constants->pulses_per_revolution;
  return Motor_Travel<Travel_Type>(counts, encoder_constants);
}


template <typename Travel_Type>
Travel_Type Motor_Travel<Travel_Type>::convToCount() const {
			return count;
}


template <typename Travel_Type>
float Motor_Travel<Travel_Type>::convToMeters() const {
			return convToCount() * Math::pi * p_encoder_constants->wheel_diameter / p_encoder_constants->pulses_per_revolution;
}

    
