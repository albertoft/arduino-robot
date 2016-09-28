/*
  Ultrasonic.cpp
*/

#include "Arduino.h"
#include "Ultrasonic.h"

Ultrasonic::Ultrasonic() {
	;
}

Ultrasonic::Ultrasonic(int triggerPin, int echoPin) {
	pinMode(triggerPin, OUTPUT);
	pinMode(echoPin, INPUT);
	this->triggerPin = triggerPin;
	this->echoPin = echoPin;
}

long Ultrasonic::getDistance() {
	// trigger / echo cycle is used to determine the distance of the nearest object by bouncing soundwaves off of it  
	digitalWrite(this->triggerPin, LOW); 
	delayMicroseconds(2);
	digitalWrite(this->triggerPin, HIGH); 
	delayMicroseconds(10);
	digitalWrite(this->triggerPin, LOW);

	// echo returns the sound wave travel time (in microseconds)
	unsigned long duration = pulseIn(this->echoPin, HIGH);
	this->distance = duration / 29 / 2;

	return this->distance;
}
