#undef ROBOT_TEST

#define ROBOT_DELAY 25

/**
 * ROBOT STATES
 */

// init
#define STATE_INIT 'I' 
#define STATE_INIT_DELAY 30000

// stop
#define STATE_STOP 'S'

// forward
#define STATE_FORWARD 'F'

// steer
#define STATE_STEER 'X'

// find a way out
#define STATE_FINDWAYOUT 'W'
#define STATE_FINDWAYOUT_ANGLE 90
#define STATE_FINDWAYOUT_BLINK_DELAY 100
#define STATE_FINDWAYOUT_N_BLINK 5

// error
#define STATE_ERROR 'E'
#define STATE_ERROR_BLINK_DELAY 500
#define STATE_ERROR_N_BLINK 10


/**
 * COURSE CONTROL
 */
#define NO_COURSE -1
#define COURSE_MAX_DEVIATION 4


/**
 * SENSORS
 */ 
// ultrasonic. obstacle max distance (cm)
#define DISTANCE_OBSTACLE 30
#define DISTANCE_OBSTACLE_FINDWAYOUT 20
#define DISTANCE_OBSTACLE_ALERT 10


/**
 * SERVO MOTORS
 * 
 * RIGHT: [180 (full backward) -> 90 (stop) -> 0 (full forward)]
 * LEFT: [0 (full backward) -> 90 (stop) -> 180 (full forward)]
 */
#define LEFT 0
#define RIGHT 1

#define LEFT_SPEED(rightSpeed) (180-rightSpeed)

#define SERVO_FORWARD_DELAY 50
#define SERVO_STEER_DELAY 50

#define SERVO_STOP 90
#define SERVO_RIGHT_FWD 80
#define SERVO_LEFT_FWD LEFT_SPEED(SERVO_RIGHT_FWD)-5

#define SERVO_RIGHT_STEER 85
#define SERVO_LEFT_STEER LEFT_SPEED(SERVO_RIGHT_STEER)-2

#define SERVO_FORWARD_DISTANCE 0.9
#define SERVO_STEER_DISTANCE 0.295

/**
 * DISPLAY_VALUE
 */
#define DISPLAY_VALUE(X, UNDEF) ((X != UNDEF) ? String(X):String("null")) 


/**
 * ROBOT COMMANDS
 */
#define COMMAND_NONE '.'
#define COMMAND_FORWARD 'f'
#define COMMAND_RIGHT 'r'
#define COMMAND_LEFT 'l'
#define COMMAND_MODE_MANUAL 'm'
#define COMMAND_MODE_AUTO 'a'


/**
 * ROBOT MODES
 */
#define MODE_AUTO 'a'
#define MODE_MANUAL 'm'