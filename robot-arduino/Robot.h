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
#include "LedRGB.h"

#define DISTANCES_SIZE 2
#define DISTANCE_OBSTACLE 50

#define HEADINGS_SIZE 1

#define NO_COURSE -999
#define COURSE_MAX_DEVIATION 5

#define LEFT 0
#define RIGHT 1

#define STATUS_INIT 'I' 
#define STATUS_STOP 'S'
#define STATUS_FORWARD 'F'
#define STATUS_STEER 'x' 

class Robot {
  public:
    Robot();
    void attach(int triggerPin, int echoPin, int servoRightPin, int servoLeftPin, int redPin, int greenPin, int bluePin);

    bool obstacleDetected();

    float getHeading();
    void setCourse(float course);
    bool isCourseSet();
    bool isCourseDeviated();
    
    void forward();
    void stop();
    void steer(bool direction);

    void setStatus(char status);
    void update();
    void report();
    
  private:
    LedRGB _ledRGB;
    Ultrasonic _sonarFront;
    Compass _compass;
    Servo _servoRight;
    Servo _servoLeft;
   
    long _distances[DISTANCES_SIZE];
    int _distanceIndex;

    float _headings[HEADINGS_SIZE];
    int _headingIndex;

    float _course;
    float _courseDeviation;
    //float _heading;
    
    String _report;
    char _status;
    
};

#endif
