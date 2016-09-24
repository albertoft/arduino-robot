/*
  Robot.h - Robot class.
*/

#ifndef Robot_h
#define Robot_h

#include "Arduino.h"
#include <Servo.h>
#include <SoftwareSerial.h>

#include "Ultrasonic.h"
#include "Compass.h"

#define DISTANCES_SIZE 2
#define DISTANCE_OBSTACLE 50

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
   
    long _distances[DISTANCES_SIZE];
    int _distanceIndex;
    float _heading;
    String _report;
};

#endif
