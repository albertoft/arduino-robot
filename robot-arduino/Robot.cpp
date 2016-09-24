/*
  Robot.cpp.
*/

#include "Arduino.h"
#include "Robot.h"

Robot::Robot() 
{
  ;
}

void Robot::attach(int triggerPin, int echoPin, int servoRightPin, int servoLeftPin) 
{
  _sonarFront = Ultrasonic(triggerPin, echoPin);
  _servoRight.attach(servoRightPin);
  _servoLeft.attach(servoLeftPin);
  _report=String();
  _compass = Compass(12345);
  stop();  
}

bool Robot::obstacleDetected() {
  _distanceFront = _sonarFront.getDistance();
  return bool(_distanceFront < 50);
}

void Robot::stop() 
{
  _servoRight.write(90);
  _servoLeft.write(90);
}

void Robot::forward() 
{
  _servoRight.write(0);
  _servoLeft.write(180);
}

void Robot::update()
{
  if (!obstacleDetected()) { 
    forward();
  } else {
    stop();
  }
  report();
}

void Robot::report()
{
  _report = String("d=") + _distanceFront + String(" h=") + _compass.getHeadingDegrees();
  Serial.println(_report);
}

