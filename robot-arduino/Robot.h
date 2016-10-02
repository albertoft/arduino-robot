/*
 * Robot - 3 Wheels Robot
 * 
 */
 
#ifndef Robot_h
#define Robot_h

#include "Arduino.h"
#include <Servo.h>
#include <SoftwareSerial.h>

#include "Ultrasonic.h"
#include "Compass.h"

#include "LedRGB.h"
#include "RobotAux.h" 

class Robot {
	public:
		Robot();
		
		/// Initilizes robot
		void init(int triggerPin, int echoPin, int servoRightPin, int servoLeftPin, int redPin, int greenPin, int bluePin);
		
		/// Main loop. Transitions between states depending on sensors
		void run();
    
	private:
		/// Sensor. Detect obstacles
		bool isObstacleDetected();
		bool isObstacleDetected(long obstacleDistance);
		bool isObstacleAlert();
		
		/// Sensor. Get heading
		float getHeading();
		
		/// Control course
		void setCourse(float course);
		bool isCourseSet();
		bool isCourseDeviated();
		bool isCourseDeviated(bool initialDevDir);
		
		/// States
		void forward();
		void stop();
		void steer();
		void steer(long obstacleDistance);
		void findWayOut();
		void error();
		void setState(char state);

		/// Utils. Timeout
		void setTimeOut(unsigned int timeout);
		bool isTimedOut();
		
		/// Utils. 
		void notifyStateChange();
		
		/// Component. RGB led to display current status
		LedRGB ledRGB;
		
		/// Component. Ultrasonic sensor (front) and distances to nearest obstacle
		Ultrasonic sonarFront;
		long distance;

		/// Component. Compass, current heading, course and deviation
		Compass compass;
		float heading;
		float course;
		float courseDeviation;

		/// Component. Servo motors (continuos rotation)
		Servo servoRight;
		Servo servoLeft;

		/// current state
		char state;
		
		/// current timeout (milliseconds)
		unsigned int timeOut;
		
		/// actual time (milliseconds) when timeout was set
		unsigned int lastTime;
		
		/// true if a timeout has occurred
		bool wasTimedOut;
		
		/// true if obstacle detected too close (alert distance)
		bool obstacleAlert;
};

#endif
