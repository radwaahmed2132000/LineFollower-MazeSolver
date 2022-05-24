#include "maze_solver.h"

float CNYread[6];

#define ncounts              100
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


void moveLeftMotor(int speed);
void moveRightMotor(int speed);

void turnLeft(int delayMs);
void turnRight(int delayMs);

void forward(int delayMs);
void backward(int delayMs);

void setup() {
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);

  pinMode(leftMotorSpeedPin, OUTPUT);
  pinMode(leftMotorDirection1, OUTPUT);
  pinMode(leftMotorDirection2, OUTPUT);

  pinMode(rightMotorSpeedPin, OUTPUT);
  pinMode(rightMotorDirection1, OUTPUT);
  pinMode(rightMotorDirection2, OUTPUT);

  Serial.begin(9600);
 
}

void loop() {

  // CNYread[0] = analogRead(A1);
  // CNYread[1] = analogRead(A2);
  // CNYread[2] = analogRead(A3);
  // CNYread[3] = analogRead(A4);
  // CNYread[4] = analogRead(A5);

  // char buffer[64];
  // sprintf(buffer, "\r%f|%f|%f|%f|%f|%f", CNYread[0], CNYread[1], CNYread[2],CNYread[3], CNYread[4]);
  // Serial.write('\r');

  // turnLeft(turnLeftDelay);
  // delay(3000);
  // turnRight(turnRightDelay);
  // delay(3000);

  forward(forwardDelay);
  delay(3000);
  backward(backwardDelay);
  delay(3000);



}

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