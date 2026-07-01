#ifndef MYMATH_H
#define MYMATH_H
class Math{
	public:
		static constexpr double pi = 3.141592653589793238462643;
	public:
		static long long powi(long long base, unsigned char exponent);
		template <typename Type> static Type absolute(Type value);
		template <typename Type> static char sign(Type value);
		template <typename Type> static Type maxof(Type a, Type b);
		template <typename Type> static Type minof(Type a, Type b);
};

#include "Math.tpp"

#endif