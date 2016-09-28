/*
  Ultrasonic.h - Library for Ultrasonic HC-SR04 sensor.
*/

#ifndef Ultrasonic_h
#define Ultrasonic_h

#include "Arduino.h"

class Ultrasonic {
	public:
		Ultrasonic();
		Ultrasonic(int triggerPin, int echoPin);
		long getDistance();

	private:
		int triggerPin;
		int echoPin;
		long distance;
};

#endif
