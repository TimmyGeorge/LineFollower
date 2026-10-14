#ifndef PINS_HPP
#define PINS_HPP

template<unsigned char num_pins>
struct Pins{
  public:
    unsigned char pins[num_pins] = {0};
  private:
    void constructPinArray(unsigned char num_pins_not_pushed, unsigned char first_pin);
    template <typename... IntegralTypes> void constructPinArray(unsigned char num_pins_not_pushed, unsigned char first_pin, IntegralTypes... other_pins);
  public:
    template <typename... IntegralTypes> Pins(IntegralTypes... input_pins);
    Pins(unsigned char* _pins);
    explicit operator unsigned char*();
};

#include "Pins.tpp"

#endif