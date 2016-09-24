#include "Ultrasonic.h"
#include "Robot.h"

Robot robot;

void setup() {
  Serial.begin(9600);
  robot.attach(11, 12, 10, 9);
}

void loop() {
  robot.update();
}


