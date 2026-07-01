#ifndef PID_CONTROLLER_HPP
#define PID_CONTROLLER_HPP


class PID_Controller{
	protected:
		float previous_error;
		float integral;
	public:
		float kp;
		float kd;
		float ki;
	public:
		PID_Controller();
		PID_Controller(float _kp, float _kd, float _ki);
	public:
		virtual float calcControllerOutput(float _error);	
};


class Antiwindup_PID_Controller : PID_Controller {
	public:
		float upper_clamping_saturation_limit;
		float lower_clamping_saturation_limit;
	private:
		bool is_clamping;
	public:
		Antiwindup_PID_Controller();
		Antiwindup_PID_Controller(float _kp, float _kd, float _ki, float _upper_clamping_saturation_limit, float _lower_clamping_saturation_limit);
	public:
		virtual float calcControllerOutput(float _error) override;
};

#endif