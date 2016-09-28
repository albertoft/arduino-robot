#include "Robot.h"

Robot robot;

void setup() {
  Serial.begin(9600);
  robot.init(7, 8, 10, 9, 11, 5, 6);
}

void loop() {
  robot.run();
}


