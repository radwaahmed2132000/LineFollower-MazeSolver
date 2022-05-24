#pragma once

#define leftMotorSpeedPin    3
#define leftMotorDirection1  4
#define leftMotorDirection2  5
#define rightMotorSpeedPin   6
#define rightMotorDirection1 7
#define rightMotorDirection2 8


// Delays for 90 degree turns
#define turnLeftDelay 250
#define turnRightDelay 250

// Delay to move the car forward so that the wheels are at where the IR sensors were 
#define forwardDelay 300

// Opposite of the above function, the car seems to go back further for the same speed.
#define backwardDelay 170


void moveLeftMotor(int speed) {
  if (speed > 100)
    speed = 100;

  analogWrite(leftMotorSpeedPin, speed);

  if (speed > 0) {
    digitalWrite(leftMotorDirection1, HIGH);
    digitalWrite(leftMotorDirection2, LOW);
  } else {
    digitalWrite(leftMotorDirection1, LOW);
    digitalWrite(leftMotorDirection2, HIGH);
  }
}

void moveRightMotor(int speed) {
  if (speed > 100)
    speed = 100;

  analogWrite(rightMotorSpeedPin, speed);

  if (speed > 0) {
    digitalWrite(rightMotorDirection1, HIGH);
    digitalWrite(rightMotorDirection2, LOW);
  } else {
    digitalWrite(rightMotorDirection1, LOW);
    digitalWrite(rightMotorDirection2, HIGH);
  }
}

void turnLeft(int delayMs) {
  moveLeftMotor(-100);
  moveRightMotor(100);
  delay(delayMs);

  moveLeftMotor(0);
  moveRightMotor(0);
}

void turnRight(int delayMs) {
  moveLeftMotor(100);
  moveRightMotor(-100);
  delay(delayMs);

  moveLeftMotor(0);
  moveRightMotor(0);
}

void forward(int delayMs) {
  moveLeftMotor(100);
  moveRightMotor(100);
  delay(delayMs);

  moveLeftMotor(0);
  moveRightMotor(0);
}
 
void backward(int delayMs) {
  moveLeftMotor(-100);
  moveRightMotor(-100);
  delay(delayMs);

  moveLeftMotor(0);
  moveRightMotor(0);
}