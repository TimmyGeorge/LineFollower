#ifndef TIMER_HPP
#define TIMER_HPP
class Time_Interval{
	public:
		unsigned long length;
		unsigned long end;
		unsigned long getStart() const;
};

class Timer{
  //Member Variables
  	public:
  		unsigned long start_time;
  //Constructor
  	public:
		Timer();
  //Member Functions
  	public:
		virtual unsigned long getTimestamp() const = 0; 
   		void start();
    	unsigned long seeTime() const;
    	Time_Interval captureTimeInterval() const;
};


class Millisecond_Timer : public Timer{
  //Overwritten Member Functions
 	public:
   		virtual unsigned long getTimestamp() const override;
};


class Microsecond_Timer : public Timer{
  //Overwritten Member Functions
	public:
    	virtual unsigned long getTimestamp() const override;
};
#endif