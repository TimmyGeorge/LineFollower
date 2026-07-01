template <unsigned char num_pins>
void Pins<num_pins>::constructPinArray(unsigned char num_pins_not_pushed, unsigned char first_pin){
  pins[num_pins - num_pins_not_pushed] = first_pin;
}


template <unsigned char num_pins>
template <typename... IntegralTypes>
void Pins<num_pins>::constructPinArray(unsigned char num_pins_not_pushed, unsigned char first_pin, IntegralTypes... other_pins){
  pins[num_pins - num_pins_not_pushed] = first_pin;
  constructPinArray(num_pins_not_pushed - 1, other_pins...);
}

template <unsigned char num_pins>
template <typename... IntegralTypes>
Pins<num_pins>::Pins(IntegralTypes... input_pins){
  constructPinArray(num_pins, input_pins...);
}


template <unsigned char num_pins>
Pins<num_pins>::Pins(unsigned char* _pins){
  memcpy(pins, _pins, sizeof(pins));
}


template <unsigned char num_pins>
Pins<num_pins>::operator unsigned char*(){
  return pins;
}




