#ifndef SIMPLE_POTENTIAL_DIVIDER_HPP
#define SIMPLE_POTENTIAL_DIVIDER_HPP

class Simple_Potential_Divider{
	public:
		unsigned long r1;
		unsigned long r2;
	public:
		Simple_Potential_Divider(unsigned long _r1, unsigned long _r2);
		float calcTransferFunction();
};

#endif