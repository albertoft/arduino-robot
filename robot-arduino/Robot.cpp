/*
  Robot.cpp.
*/

#include "Arduino.h"
#include "Robot.h"

Robot::Robot() 
{
  ;
}

void Robot::attach(int triggerPin, int echoPin, int servoRightPin, int servoLeftPin, int redPin, int greenPin, int bluePin) 
{
  _ledRGB = LedRGB(redPin, greenPin, bluePin);
  _sonarFront = Ultrasonic(triggerPin, echoPin);
  _servoRight.attach(servoRightPin);
  _servoLeft.attach(servoLeftPin);
  _report=String();
  _compass = Compass(12345);

  _distanceIndex = 0;

  stop();
  setStatus(STATUS_INIT);
  delay(5000);
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

float Robot::getHeading() {
  _heading = _compass.getHeadingDegrees();
  return _heading;
}

void Robot::setCourse(float course) 
{
  _course = course;
}

void Robot::stop() 
{
  setStatus(STATUS_STOP);
  setCourse(NO_COURSE);
  _servoRight.write(SERVO_STOP);
  _servoLeft.write(SERVO_STOP);
}

bool Robot::isCourseDeviated()
{
  _courseDeviation = _compass.getDeviation(_course, getHeading());
  return ((isCourseSet()) && (abs(_courseDeviation) > COURSE_MAX_DEVIATION));
}

bool Robot::isCourseSet()
{
  return (_course != NO_COURSE);
}

void Robot::forward() 
{
  if (isCourseDeviated()) {
      bool dir = (_courseDeviation > 0);
      steer(dir);
  } else {
    if (_course == NO_COURSE) {
      setCourse(getHeading());
    }
    setStatus(STATUS_FORWARD);
    _servoRight.write(SERVO_RIGHT_FWD);
    _servoLeft.write(SERVO_LEFT_FWD);
  }
}

void Robot::steer(bool direction) 
{
  setStatus(STATUS_STEER);
  switch (direction) {
    case LEFT:
      _servoRight.write(SERVO_RIGHT_STEER);
      _servoLeft.write(SERVO_STOP);
      break;
    case RIGHT:
      _servoRight.write(SERVO_STOP);
      _servoLeft.write(SERVO_LEFT_STEER);
      break;
  }
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
  _report = String("s=") + _status + String(" d=") + _distances[_distanceIndex] + String(" c=") + _course + String(" h=") + _heading + String(" dev=") + _courseDeviation;
  Serial.println(_report);
}

void Robot::setStatus(char status)
{
  _status = status;
  
  switch(status) {
    case STATUS_FORWARD:
      _ledRGB.setColor(COLOR_GREEN);
      break;

    case STATUS_STOP:
      _ledRGB.setColor(COLOR_RED);
      break;

    case STATUS_STEER:
      _ledRGB.setColor(COLOR_BLUE);
      break;

    case STATUS_INIT:
      _ledRGB.setColor(COLOR_PURPLE);
      break;
  }
  
}

