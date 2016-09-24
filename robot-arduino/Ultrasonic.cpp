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
	_triggerPin = triggerPin;
	_echoPin = echoPin;
}

long Ultrasonic::getDistance() {
  // trigger / echo cycle is used to determine the distance of the nearest object by bouncing soundwaves off of it  
  digitalWrite(_triggerPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(_triggerPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(_triggerPin, LOW);

  // echo returns the sound wave travel time (in microseconds)
  unsigned long duration = pulseIn(_echoPin, HIGH);
  _distance = duration / 29 / 2;
  
  return _distance;
}
