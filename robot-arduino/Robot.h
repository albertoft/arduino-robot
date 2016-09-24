/*
  Robot.h - Robot class.
*/

#ifndef Robot_h
#define Robot_h

#define DEBUG_BUILD 1

#include "Arduino.h"
#include <Servo.h>
#include <SoftwareSerial.h>

#include "Ultrasonic.h"
#include "Compass.h"

class Robot {
  public:
    Robot();
    void attach(int triggerPin, int echoPin, int servoRightPin, int servoLeftPin);

    bool obstacleDetected();
    float getHeading();
    void stop();
    void forward();

    void update();
    void report();
        
  private:
    Ultrasonic _sonarFront;
    Compass _compass;
    Servo _servoRight;
    Servo _servoLeft;
   
    long _distanceFront;
    float _heading;
    String _report;
};

#endif
