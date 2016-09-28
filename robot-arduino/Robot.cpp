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
	this->distanceIndex = 0;

	stop();
	setState(STATE_INIT);
}


/**
 * Run robot. Transitions between states depending on retrieved data from sensors.
 */
void Robot::run()
{ 
  if (!isObstacleDetected()) {
	if (!isCourseSet()) {
		setCourse(getHeading());
	}
	forward();
	if (isCourseDeviated()) {
		steer();
		if (this->wasTimedOut) {
			setCourse(NO_COURSE);
			error();
		}
	}
  } else {
	setCourse(NO_COURSE);
    stop();
  }
  
  report();
}


/**
 * Detects if there is any obstacle using front Ultrasonic sensor
 * @return true if obstacle detected in front of the robot
 */
bool Robot::isObstacleDetected() {
	long distance = this->sonarFront.getDistance();

	// store current measured distance to distances array
	this->distanceIndex = (++this->distanceIndex) % DISTANCES_SIZE;
	this->distances[this->distanceIndex] = distance;

	// if any of last stored distances represents an obstacle, return that an obstacle is detected
	// (avoid outlier values)
	for (int i=0; i < DISTANCES_SIZE; i++) {
		if (this->distances[i] < DISTANCE_OBSTACLE) {
		  return true;
		}
	}
	return false;
}


/**
 * Measures current robot heading using the compass
 * @return heading (in degrees) [0, 360º]
 */
float Robot::getHeading() {
	float headingSum = 0;
	for (int i=0; i < HEADING_AVG_MEASURES; i++) {
		headingSum += compass.getHeadingDegrees();
	}
	this->heading = (headingSum / HEADING_AVG_MEASURES);
	
	return this->heading;
}


/**
 * Set the current course for robot
 * @param course new robot course in degrees [0, 360º]  
 */
void Robot::setCourse(float course) 
{
	this->course = course;
}


/**
 * Measures if robot heading is deviated from current course
 * @return true if robot heading is deviated
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
 * Set a timeout time.
 * @param timeOut timeout time (in milliseconds)
 */
void Robot::setTimeOut(unsigned int timeOut)
{
  this->timeOut = timeOut;
  this->wasTimedOut = false;
  this->lastTime = millis();
}


/**
 * Measures current time and finds out if robot current timeout has occurred
 * @return true is timeout has occurred 
 */
bool Robot::isTimedOut()
{
	unsigned int now = millis();
	this->wasTimedOut = ((now - this->lastTime) > this->timeOut);
	return this->wasTimedOut;
}


/**
 * Forward state. Robot going forward. 
 */
void Robot::forward() 
{
  setState(STATE_FORWARD);
  this->servoRight.write(SERVO_RIGHT_FWD);
  this->servoLeft.write(SERVO_LEFT_FWD);
  delay(STATE_FORWARD_DELAY);
  this->servoRight.write(SERVO_STOP);
  this->servoLeft.write(SERVO_STOP);
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
	setState(STATE_STEER);
	setTimeOut(STATE_STEER_TIMEOUT);
	
	bool timedOut = false;
	while ((isCourseDeviated()) && (!timedOut) && (!isObstacleDetected())) {
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
		delay(STATE_STEER_DELAY);
		timedOut = isTimedOut();
		report();
	}
	this->servoRight.write(SERVO_STOP);
	this->servoLeft.write(SERVO_STOP);	
}



/**
 * Error state. Robot has entered into error. 
 */
void Robot::error()
{
	setState(STATE_ERROR);
	stop();
}


/**
 * Report current robot status. 
 */
void Robot::report()
{
	String line = 
		String("s=") + this->state + 
		String(" c=") + this->course +
		String(" d=") + this->distances[this->distanceIndex] + 
		String(" c=") + this->course + 
		String(" h=") + this->heading + 
		String(" dev=") + this->courseDeviation
	;
	Serial.println(line);
}


/**
 * Set a robot state
 */
void Robot::setState(char state)
{
	this->state = state;
	
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

