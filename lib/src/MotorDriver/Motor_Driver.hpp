#ifndef MOTOR_DRIVER_HPP
#define MOTOR_DRIVER_HPP

class Motor_Driver{
  public:
	virtual ~Motor_Driver() = default;
  public:
    virtual void setVelocity(float _velocity) = 0;
    virtual void setSpeed(float speed) = 0;
    virtual void setForward() = 0;
    virtual void setBackward();
    virtual void init() = 0;
    virtual void stop() = 0;
	virtual float getDriveVelocity() const = 0;

};


class Single_Motor_Driver : public Motor_Driver{
  private:
    static constexpr unsigned char adc_write_resolution = 8;
    unsigned char in1_pin;
    unsigned char in2_pin;
    unsigned char pwm_pin;
    float velocity;
  public:
	virtual ~Single_Motor_Driver() = default;
    Single_Motor_Driver(unsigned char _in1_pin, unsigned char _in2_pin, unsigned char _pwm_pin);
  private:
  	void drive(float speed);
  public:
    virtual void setVelocity(float _velocity) override;
    virtual void setSpeed(float speed) override;
    virtual void setForward() override;
    virtual void setBackward() override;
    virtual void init() override;
    virtual void stop() override;
    virtual float getDriveVelocity() const override;
};


/*
class Acceleratable_Single_Motor_Driver : public Single_Motor_Driver {
	private:
		static constexpr unsigned char MaxSupportedMotors = 10;
		static unsigned char FreeSlots[MaxSupportedMotors];
		static unsigned char NumFreeSlots;
		static unsigned char OccupiedSlots[MaxSupportedMotors]
		static unsigned char NumOccupiedSlots;
		static Acceleratable_Single_Motor_Driver* MotorsToAccelerate[MaxSupportedMotors];
		static float Accelerations[MaxSupportedMotors]
	public:
		static void enableAccelerationTimer();
		static void disableAccelerationTimer();
	public:
		virtual void init() const override
		void startAccelerating(float InitialAcceleration);
		void setAcceleration(float acceleration); //in normalized speed per second
		void stopAccelerating();
	public:
		
};
*/

struct Velocities{
	float motora;
	float motorb;
};


class Dual_Motor_Driver : public Motor_Driver {
  public:
    Single_Motor_Driver motora;
    Single_Motor_Driver motorb;
  public:
	virtual ~Dual_Motor_Driver() = default;
    Dual_Motor_Driver(unsigned char _ina1_pin, unsigned char _ina2_pin, unsigned char _pwma_pin, unsigned char _inb1_pin, unsigned char _inb2_pin, unsigned char _pwmb_pin);
  public: 
    //Overriden functions
    virtual void setVelocity(float _velocity) override;
    virtual void setSpeed(float speed) override;
    virtual void setForward() override;
    virtual void setBackward() override;
    virtual void init() override;
    virtual void stop() override;
	virtual float getDriveVelocity() const override;
    //Specific functions
    void setVelocities(float base_velocity, float offset);
    Velocities getDriveVelocities() const;
};


class Dual_Motor_Driver_With_Standby : public Dual_Motor_Driver {
	private:
		unsigned char standby_pin;
		bool on_standby;
	public: 
		virtual ~Dual_Motor_Driver_With_Standby() = default;
		Dual_Motor_Driver_With_Standby(unsigned char _ina1_pin, unsigned char _ina2_pin, unsigned char _pwma_pin, unsigned char _inb1_pin, unsigned char _inb2_pin, unsigned char _pwmb_pin, unsigned char _standby_pin);
	public:
		virtual void init() override;
		bool isOnStandby() const;
		void placeOnStandby();
		void placeOffStandby();
};


#endif //MOTOR_DRIVER_HPP