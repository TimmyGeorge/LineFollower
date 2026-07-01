#ifndef MYSERIAL_HPP
#define MYSERIAL_HPP

#include <Arduino.h>

class MySerial{
	public:
		static void waitTillISaySomething();
		static void zeroBuffer(char* buffer, unsigned int length);
		static void waitForInput(char (&input)[65]);
		
		template <typename T>
		static void print(const char* prefix, const T& item, const char* suffix, const char new_line = '\n'){
			Serial.print(prefix);
			Serial.print(item);
			Serial.print(suffix);
			Serial.print(new_line);
		}
		
		template <typename T>
		static void printArray(const char* prefix, const T* array, const unsigned long num_elements, const char* suffix, const char new_line = '\n'){
			Serial.print(prefix);
			Serial.print('{');
			if (num_elements > 0){
				for(unsigned long i = 0; i < num_elements - 1; ++i){
					Serial.print(array[i]);
					Serial.print(',');
					Serial.print(' ');
				}
				Serial.print(array[num_elements - 1]);
			}
			Serial.print('}');
			Serial.print(suffix);
			Serial.print(new_line);
		}
};

#endif