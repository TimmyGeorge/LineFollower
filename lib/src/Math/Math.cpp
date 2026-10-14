#include "Math.hpp"

long long Math::powi(long long base, unsigned char exponent){
		long long pow = 1;
		while (exponent){
			if (exponent & 1) pow*=base;
			base*=base;
			exponent >>= 1;
		}
		return pow;
}