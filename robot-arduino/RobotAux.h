/**
 * ROBOT STATES
 */

// init
#define STATE_INIT 'I' 
#define STATE_INIT_DELAY 5000

// stop
#define STATE_STOP 'S'

// forward
#define STATE_FORWARD 'F'
#define STATE_FORWARD_DELAY 30

// steer
#define STATE_STEER 'x'
#define STATE_STEER_DELAY 5
#define STATE_STEER_TIMEOUT 20000

// error
#define STATE_ERROR 'E'
#define STATE_ERROR_BLINK_DELAY 500
#define STATE_ERROR_N_BLINK 5


/**
 * COURSE CONTROL
 */
#define NO_COURSE -999
#define COURSE_MAX_DEVIATION 3


/**
 * SENSORS
 */
// heading. number of measures to gather (calculates average)
#define HEADING_AVG_MEASURES 3 
// ultrasonic. number of measures to consider when detecting obstacles
#define DISTANCES_SIZE 2
// ultrasonic. obstacle max distance (cm)
#define DISTANCE_OBSTACLE 50


/**
 * SERVO MOTORS
 */
#define LEFT 0
#define RIGHT 1

#define SERVO_STOP 90
#define SERVO_RIGHT_FULL 0   // [0, 180]
#define SERVO_LEFT_FULL 180  // [180, 0]

#define SERVO_FWD_SPEED 83
#define SERVO_STEER_SPEED 87

#define SERVO_RIGHT_FWD_INC 0
#define SERVO_LEFT_FWD_INC 0

#define SERVO_RIGHT_STEER_INC 0
#define SERVO_LEFT_STEER_INC 0

#define SERVO_RIGHT_FWD SERVO_RIGHT_FULL + SERVO_FWD_SPEED - SERVO_RIGHT_FWD_INC
#define SERVO_LEFT_FWD SERVO_LEFT_FULL - SERVO_FWD_SPEED + SERVO_LEFT_FWD_INC

#define SERVO_RIGHT_STEER SERVO_RIGHT_FULL + SERVO_STEER_SPEED - SERVO_RIGHT_STEER_INC
#define SERVO_LEFT_STEER SERVO_LEFT_FULL - SERVO_STEER_SPEED + SERVO_LEFT_STEER_INC
