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
  
  _distanceIndex = 0;
  stop();  
}

bool Robot::obstacleDetected() {
  long distance = _sonarFront.getDistance();

  // store current measured distance to distances array
  _distanceIndex = (++_distanceIndex) % DISTANCES_SIZE;
  _distances[_distanceIndex] = distance;

  // if any of last stored distances represents an obstacle, return that an obstacle is detected
  // (avoid outlier values)
  for (int i=0; i < DISTANCES_SIZE; i++) {
    if (_distances[i] < DISTANCE_OBSTACLE) {
      return true;
    }
  }
  return false;
}

void Robot::stop() 
{
  _servoRight.write(90);
  _servoLeft.write(90);
}

void Robot::forward() 
{
  //0, 180 => full speed
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
  delay(200);
}

void Robot::report()
{
  _report = String("d=") + _distances[_distanceIndex] + String(" h=") + _compass.getHeadingDegrees();
  Serial.println(_report);
}

