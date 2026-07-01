#include "Serial.hpp"

void MySerial::waitTillISaySomething(){
	Serial.flush();
	while (Serial.available() == 0){
		delay(200);
	}
	Serial.flush();
	while (Serial.available()){
		Serial.read();
	}
}


void MySerial::waitForInput(char (&input)[65]){
	Serial.flush();
	while (Serial.available() == 0){
		delay(200);
	}
	Serial.flush();
	unsigned char i = 0;
	while (Serial.available()){
		input[i] = Serial.read();
		i++;
	}
	input[i]=0;
}

void MySerial::zeroBuffer(char* buffer, unsigned int length){
	for(unsigned int i = 0; i < length; ++i){
		buffer[i] = 0;
	}
}