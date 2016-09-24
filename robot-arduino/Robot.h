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
#include "Speed.h"

#define DISTANCES_SIZE 2
#define DISTANCE_OBSTACLE 50

#define NO_COURSE -999
#define COURSE_MAX_DEVIATION 1

#define LEFT 0
#define RIGHT 1


class Robot {
  public:
    Robot();
    void attach(int triggerPin, int echoPin, int servoRightPin, int servoLeftPin);

    bool obstacleDetected();

    float getHeading();
    void setCourse(float course);
    bool isCourseSet();
    bool isCourseDeviated();
    
    void forward();
    void stop();
    void steer(bool direction);
    
    void update();
    void report();
    
  private:
    Ultrasonic _sonarFront;
    Compass _compass;
    Servo _servoRight;
    Servo _servoLeft;
   
    long _distances[DISTANCES_SIZE];
    int _distanceIndex;

    float _course;
    float _courseDeviation;
    float _heading;
    
    String _report;
};

#endif
