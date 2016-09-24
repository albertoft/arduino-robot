/*
  Robot.h - Robot class.
*/

#ifndef Compass_h
#define Compass_h

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

class Compass {
  public:
    Compass();
    Compass(int id);
    
    float getHeadingDegrees();
    float getDeviation(float course, float heading);
       
  private:
    Adafruit_HMC5883_Unified _hmc;
    float _headingDegrees;
};

#endif
