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

struct Point {
	int x, y;
};

class Robot {
	public:
		Robot();
		
		// Initilizes robot
		void init(int triggerPin, int echoPin, int servoRightPin, int servoLeftPin, int redPin, int greenPin, int bluePin);
		
		// Main loop. Transitions between states depending on sensors
		void run();
    
	private:
		// Sensor. Detect obstacles
		bool isObstacleDetected();
		bool isObstacleDetected(long obstacleDistance);
		bool isObstacleAlert();
		
		// Sensor. Get heading
		float getHeading();
		
		// Control course
		void setCourse(float course);
		bool isCourseSet();
		bool isCourseDeviated();
		bool isCourseDeviated(bool initialDevDir);
		
		// States
		void forward();
		void forward(int iterations);
		void stop();
		void steer();
		void steer(long obstacleDistance);
		void steer(bool direction, int iterations);
		void findWayOut();
		void error();
		void setState(char state);
		
		// Positioning
		void calculatePosition();
		
		// Commands
		bool isCommandReceived();
		void doCommand();

		// Utils. Timeout
		void setTimeOut(unsigned int timeout);
		bool isTimedOut();
		
		// Utils. Send data
		void notifyStateChange();
		
		// Internal. Test methods.
		void test();
		
		// Component. RGB led to display current status
		LedRGB ledRGB;
		
		// Component. Ultrasonic sensor (front) and distances to nearest obstacle.
		//  alert is true if obstacle is dangerously close.
		Ultrasonic sonarFront;
		long obstacleDistance;
		bool obstacleAlert; 
		
		// Component. Compass, heading, course and deviation
		Compass compass;
		float heading;
		float lastHeading;
		float course;
		float courseDeviation;
		float lastKnownCourse;

		// Component. Servo motors (continuos rotation)
		Servo servoRight;
		Servo servoLeft;

		// current state
		char state;
		
		// current mode (auto, manual)
		char mode;
		
		// command received through bluetooth
		char command;
		
		// Calculated. Total travelled distance (cm)
		float totalDistance;
		float lastTotalDistance;
		
		// Calculated. Robot position on 2D plane
		Point currentPosition;
		Point lastPosition;
		Point obstaclePosition;
};

#endif
