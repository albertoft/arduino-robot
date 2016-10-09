/**
 * Robot.cpp
 */
 
#include "Arduino.h"
#include "Robot.h"


/**
 * Default constructor
 */
Robot::Robot()
{
	;
}


/**
 * Creates robot 
 */
void Robot::init(int triggerPin, int echoPin, int servoRightPin, int servoLeftPin, int redPin, int greenPin, int bluePin) 
{
	this->ledRGB = LedRGB(redPin, greenPin, bluePin);
	this->sonarFront = Ultrasonic(triggerPin, echoPin);
	this->servoRight.attach(servoRightPin);
	this->servoLeft.attach(servoLeftPin);
	this->compass = Compass(12345); // unique id
	
	this->lastHeading = 0;
	this->totalDistance = 0;
	this->lastTotalDistance = 0;
	this->lastPosition.x = 0;
	this->lastPosition.y = 0;
	
	stop();
	setCourse(NO_COURSE);
	setState(STATE_INIT);
	
	#ifdef ROBOT_TEST
	test();
	#endif
}


/**
 * Run robot. Transitions between states depending on retrieved data from sensors.
 */
void Robot::run()
{ 
	#ifndef ROBOT_TEST 
	if (!isObstacleDetected()) {
		if (!isCourseSet()) {
			setCourse(getHeading());
		}
		if (isCourseDeviated()) {
			steer();
		} else {
			forward();
		}
	} 
	else {
		setCourse(NO_COURSE);
		stop();
		findWayOut();
		if (isObstacleAlert()) {
			error();
		}
	}
	
	delay(ROBOT_DELAY);
	#endif
}


/**
 * Detects if there is any obstacle using front Ultrasonic sensor
 * @return true if obstacle detected in front of the robot
 */
bool Robot::isObstacleDetected() {
	return isObstacleDetected(DISTANCE_OBSTACLE);
}


/**
 * Detects if there is any obstacle using front Ultrasonic sensor
 * @return true if obstacle detected in front of the robot
 */
bool Robot::isObstacleDetected(long obstacleDistance) {
	this->obstacleDistance = this->sonarFront.getDistance();
	this->obstacleAlert = (this->obstacleDistance < DISTANCE_OBSTACLE_ALERT);
	
	return (this->obstacleDistance < obstacleDistance);
}

/**
 * Returns if an obstacle has been detected within the alert distance in the last isObstacleDetected scan
 * @return true if an obstacle has been detected within the alert distance
 */
bool Robot::isObstacleAlert()
{
	return this->obstacleAlert;
}


/**
 * Measures current robot heading using the compass
 * @return heading (in degrees) [0, 360º]
 */
float Robot::getHeading() {
	this->heading = this->compass.getHeadingDegrees();	
	return this->heading;
}


/**
 * Set the current course for robot
 * @param course new robot course in degrees [0, 360º]  
 */
void Robot::setCourse(float course) 
{
	if (isCourseSet()) {
		this->lastKnownCourse = this->course;
	}
	this->course = course;
}


/**
 * Measures if robot heading is deviated from current course considering an initial deviation direction.
 * @param initialDevDir initial deviation direction (RIGHT, LEFT). 
 * @return true if robot heading is deviated (maximum deviation degrees) or deviation direction has not changed.
 */
bool Robot::isCourseDeviated(bool initialDevDir)
{
	this->courseDeviation = this->compass.getDeviation(this->course, getHeading());
	
	bool devDir = (this->courseDeviation > 0);
	return ((isCourseSet()) && ((devDir == initialDevDir) || (abs(this->courseDeviation) > COURSE_MAX_DEVIATION)));
}


/**
 * Measures if robot heading is deviated from current course considering only maximum degrees of deviation.
 * @return true if robot heading is deviated (maximum deviation degrees).
 */
bool Robot::isCourseDeviated()
{
	this->courseDeviation = this->compass.getDeviation(this->course, getHeading());
	return ((isCourseSet()) && (abs(this->courseDeviation) > COURSE_MAX_DEVIATION));
}


/**
 * Returns if robot course has already been set
 * @return true if robot course is set
 */
bool Robot::isCourseSet()
{
	return (this->course != NO_COURSE);
}


/**
 * Forward state. Robot going forward. 
 */
void Robot::forward() 
{
	setState(STATE_FORWARD);
	
	this->servoRight.write(SERVO_RIGHT_FWD);
	this->servoLeft.write(SERVO_LEFT_FWD);
	delay(SERVO_FORWARD_DELAY);
	
	this->servoRight.write(SERVO_STOP);
	this->servoLeft.write(SERVO_STOP);
	
	this->totalDistance += SERVO_FORWARD_DISTANCE;
}


/** 
 * Stop state. Robot stopped.
 */
void Robot::stop() 
{
	setState(STATE_STOP);
	this->servoRight.write(SERVO_STOP);
	this->servoLeft.write(SERVO_STOP);
}


/**
 * Steer state. Robot steering towards current course
 */
void Robot::steer()
{
	steer(DISTANCE_OBSTACLE);
}


/**
 * Steer state. Robot steering towards current course
 */
void Robot::steer(long obstacleDistance)
{
	setState(STATE_STEER);
	
	bool initialDirection = (this->courseDeviation > 0);
	
	do {
		bool direction = (this->courseDeviation > 0);
		switch (direction) {
			case LEFT:
				this->servoRight.write(SERVO_RIGHT_STEER);
				this->servoLeft.write(SERVO_STOP);
				break;
				
			case RIGHT:
				this->servoRight.write(SERVO_STOP);
				this->servoLeft.write(SERVO_LEFT_STEER);
				break;
		}
		delay(SERVO_STEER_DELAY);
		
		this->servoRight.write(SERVO_STOP);
		this->servoLeft.write(SERVO_STOP);
		
		//this->totalDistance += SERVO_STEER_DISTANCE;
		
	} while ((isCourseDeviated(initialDirection)) && (!isObstacleDetected(obstacleDistance)));
}


/**
 * After a stop encountering an obstacle, find a way out.
 */
void Robot::findWayOut()
{
	setState(STATE_FINDWAYOUT);
	
	// turn right
	setCourse( fmod( (this->lastKnownCourse + 90.0), 360.0) );
	steer(DISTANCE_OBSTACLE_FINDWAYOUT);

	// remove course (if not obstacles ahead, it will go forward)
	setCourse(NO_COURSE);
}


/**
 * Error state. Robot has entered into error. 
 */
void Robot::error()
{
	setCourse(NO_COURSE);
	stop();
	setState(STATE_ERROR);
}


/**
 * Notifies a State Change event through Serial (Bluetooth). 
 */
void Robot::notifyStateChange() 
{ 
	calculatePosition();
	
	String line = 
		String("{") +
		String("\"_event\": \"sc\", ") +
		String("\"time\": ") + millis() + String(", ") +
		String("\"state\": \"") + this->state + String("\", ") +
		String("\"xpos\": ") + this->currentPosition.x + String(", ") +
		String("\"ypos\": ") + this->currentPosition.y + String(", ") +
		String("\"course\": ") + DISPLAY_VALUE(this->course, NO_COURSE) + String(", ") +
		String("\"heading\": ") + this->heading + String(", ") +
		String("\"totalDistance\": ") + this->totalDistance + String(", ") +
		String("\"deviation\": ") + this->courseDeviation + String(", ") +
		String("\"obstacle\": ") + this->obstacleDistance +
		String("}");
	;
	Serial.println(line);
}


/**
 * Set a robot state
 */
void Robot::setState(char state)
{
	char lastState = this->state;
	this->state = state;
	
	if (lastState != this->state) {
		notifyStateChange();
	}
	
	switch(state) {
		case STATE_FORWARD:
			this->ledRGB.setColor(COLOR_GREEN);
			break;

		case STATE_STOP:
			this->ledRGB.setColor(COLOR_RED);
			break;

		case STATE_STEER:
			this->ledRGB.setColor(COLOR_BLUE);
			break;
			
		case STATE_FINDWAYOUT:
			for (int i=0; i < STATE_FINDWAYOUT_N_BLINK; i++) {
				this->ledRGB.setColor(COLOR_CYAN);
				delay(STATE_FINDWAYOUT_BLINK_DELAY);
				this->ledRGB.setColor(COLOR_BLACK);
				delay(STATE_FINDWAYOUT_BLINK_DELAY);
			}
			break;

		case STATE_INIT:
			this->ledRGB.setColor(COLOR_PURPLE);
			delay(STATE_INIT_DELAY);
			break;
		  
		case STATE_ERROR:
			for (int i=0; i < STATE_ERROR_N_BLINK; i++) {
				this->ledRGB.setColor(COLOR_RED);
				delay(STATE_ERROR_BLINK_DELAY);
				this->ledRGB.setColor(COLOR_BLACK);
				delay(STATE_ERROR_BLINK_DELAY);
			}
			break;
	}
}

/*
 *
 */
void Robot::calculatePosition()
{
	#define PI 3.14159265
	
	float lastDistance = (this->totalDistance - this->lastTotalDistance);
	
	this->currentPosition.x = lastDistance * cos(this->lastHeading*PI/180) + this->lastPosition.x;
	this->currentPosition.y = lastDistance * sin(this->lastHeading*PI/180) + this->lastPosition.y;
	
	this->lastHeading = this->heading;
	this->lastTotalDistance = this->totalDistance;
	this->lastPosition.x = this->currentPosition.x;
	this->lastPosition.y = this->currentPosition.y;
}


/*
 * Test. Some fancy tests.
 */
void Robot::test() 
{
	for (int i=0; i < 5; i++) {
		this->servoRight.write(SERVO_RIGHT_FWD);
		this->servoLeft.write(SERVO_LEFT_FWD);
		delay(SERVO_FORWARD_DELAY);
		this->servoRight.write(SERVO_STOP);
		this->servoLeft.write(SERVO_STOP);
	}
}

