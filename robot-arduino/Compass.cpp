#include "Arduino.h"
#include "Compass.h"

Compass::Compass() 
{
  ;
}

Compass::Compass(int id) 
{
  _hmc = Adafruit_HMC5883_Unified(id);
  if (!_hmc.begin()) {
    Serial.println("Compass error!");
    while(1);
  }
}

float Compass::getHeadingDegrees()
{
  sensors_event_t event; 
  _hmc.getEvent(&event);
 
  // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  
  // http://www.magnetic-declination.com/ BARCELONA. 0º 49', which is 0.0174533 degrees, which is 0.000304617550338 radians 
  float declinationAngle = 0.000304;
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI; 
  
  _headingDegrees = headingDegrees;
  return _headingDegrees;
}

float Compass::getDeviation(float course, float heading)
{
  // course and heading in [0, 359]
  if (heading < course) heading += 360;

  // dev in [0, 359]
  float dev = heading - course;  

  // smallest angle
  dev = (dev < 180) ? -dev : (360-dev);
  return dev;
}

