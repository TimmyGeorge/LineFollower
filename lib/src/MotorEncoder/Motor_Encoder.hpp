#ifndef MOTOR_ENCODER_HPP
#define MOTOR_ENCODER_HPP

#include <Timer/Timer.hpp>
#include <Math/Math.hpp>

class Motor_Encoder;

//all wheel_diameters are in meters
//all pulse_speeds are in pulses per second

struct Motor_Encoder_Constants{
  unsigned int pulses_per_revolution;
  float wheel_diameter;
};

class Motor_Speed {
  public:
    static Motor_Speed createUsingPulseSpeed(float pulse_speed, const Motor_Encoder* _p_encoder);
    static Motor_Speed createUsingRpm(float rpm, const Motor_Encoder* _p_encoder);
    static Motor_Speed createUsingVelocity(float velocity, const Motor_Encoder* _p_encoder);
  private:
    float pulse_speed;
    const Motor_Encoder_Constants* p_encoder_constants;
  private:
    Motor_Speed(float _pulse_speed, const Motor_Encoder_Constants* _p_encoder_constants);
  public:
    float convToPulseSpeed() const;
    float convToRpm() const;
    float convToVelocity() const;
};

template<typename Travel_Type>
class Motor_Travel{
  public:
    static Motor_Travel createUsingCount(Travel_Type count, const Motor_Encoder* _p_encoder);
    static Motor_Travel createUsingMeters(float meters, const Motor_Encoder* _p_encoder);
	private:
		Travel_Type count;
		const Motor_Encoder_Constants* p_encoder_constants;
	private:
    Motor_Travel(Travel_Type _count, const Motor_Encoder_Constants* _p_encoder_constants);
  public:
		Travel_Type convToCount() const;
		float convToMeters() const;
  
};

using Motor_Distance = Motor_Travel<unsigned long>;
using Motor_Displacement = Motor_Travel<long>;

class Motor_Encoder{
    friend class Motor_Encoder_Debugger;
  private:
    static constexpr unsigned char num_interrupts = 6;
    static Motor_Encoder* encoder_for_isr[num_interrupts];
    unsigned char encoder_a_pin;
    unsigned char encoder_b_pin;
    
    union {
      struct {
        unsigned int pulses_per_revolution;
        float wheel_diameter;
      };
      Motor_Encoder_Constants encoder_constants;
    };

    volatile long speed_counter;
    volatile unsigned long distance_counter;
    volatile long displacement_counter;
    volatile unsigned long reset_counter;
    float previous_speed;
    Microsecond_Timer Timer;
    bool is_measuring;
  public:
    unsigned long speed_measurement_time_interval; //in micro_seconds
  public:
    Motor_Encoder(unsigned char _encoder_a_pin, unsigned char _encoder_b_pin);
    Motor_Encoder(unsigned char _encoder_a_pin, unsigned char _encoder_b_pin, unsigned int _pulses_per_revolution);
    Motor_Encoder(unsigned char _encoder_a_pin, unsigned char _encoder_b_pin, unsigned int _pulses_per_revolution, float _wheel_diameter);
    Motor_Encoder(unsigned char _encoder_a_pin, unsigned char _encoder_b_pin, unsigned int _pulses_per_revolution, float _wheel_diameter, unsigned long _speed_measurement_time_interval);
    ~Motor_Encoder();
  private:
    static void encoderIsr0();
    static void encoderIsr1();
    static void encoderIsr2();
    static void encoderIsr3();
    static void encoderIsr4();
    static void encoderIsr5();
    static void (*getPointerToEncoderIsr(unsigned char interrupt_num))();
    void isrChangeEncoderCount();
  public:
    const Motor_Encoder_Constants* getConstants() const;

    void startMeasuring();
    void stopMeasuring();

    void checkForIdling(unsigned long maximum_allowed_idle_time);
    void checkForIdling(Motor_Speed minimum_allowed_speed);
    
    Motor_Speed getPreviousSpeed();
    Motor_Speed getCurrentSpeed(); //in pulses per second
    
    Motor_Distance getDistance() const;
    Motor_Displacement getDisplacement() const;

    unsigned long getResetCounter() const;
    void resetDistanceCounter();
    void resetDisplacementCounter();
};

#include "Motor_Encoder.tpp"
#endif