#include "Simple_Potential_Divider.hpp"

Simple_Potential_Divider::Simple_Potential_Divider(unsigned long _r1, unsigned long _r2):
r1(_r1), r2(_r2){}

float Simple_Potential_Divider::calcTransferFunction(){
	if (r1==0 && r2==0) return 0;
	return static_cast<float>(r2)/(r1+r2);
}
 