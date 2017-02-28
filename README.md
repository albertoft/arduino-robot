A very simple 3-wheel robot that can be managed from an external application.

The goal of the experiment is trying to solve, in simplistic and home-made way, some of the challenges of having an autonomous rover.

![Scheme](https://github.com/albertoft/robot/blob/master/doc/robot-scheme.png)

# Robot
* Follows a given course using a compass and avoiding obstacles.
* Reports current status and position through Bluetooth to an external application in JSON format.
* Accepts commands sent from external application.
* Operates in auto and manual modes.

## Components
* 2 x FS90R Servo motors.
* 1 x HMC5883L Digital Compass.
* 1 x HC-SR04 Ultrasonic sensor.
* 1 x HC-06 Bluetooth module.
* 1 x Led RGB.

![Robot](https://github.com/albertoft/robot/blob/master/doc/img/robot-1.jpg)

# NodeJS application
* Runs on a RaspBerry Pi 2 + CSL Bluetooth USB Adapter micro V4.0.
* Receives monitor data from robot using Bluetooth (heading, course, obstacle, position, state, etc.)
 * Sends live monitor data using websocket to client AngularJS application.
 * Stores received data in database (so it can be retrieved later).
* REST api that can be used to retrive monitor data or to send commands to robot.

![NodeJS-app](https://github.com/albertoft/robot/blob/master/doc/img/nodeapp-1-auto.jpg)

# AngularJS application
* Display robot monitor data using HighCharts.
 * Charts are updated in real-time as live monitor data is received through websockets. 
* Allows the user to send commands to the robot:
  * Set Auto / manual mode.
  * Go Forward / Left / Right.

![AngularJS-app-data](https://github.com/albertoft/robot/blob/master/doc/img/angularapp-monitor-data.jpg)
![AngularJS-app-position](https://github.com/albertoft/robot/blob/master/doc/img/angularapp-monitor-position.jpg)
![AngularJS-app-command](https://github.com/albertoft/robot/blob/master/doc/img/angularapp-command.PNG)
