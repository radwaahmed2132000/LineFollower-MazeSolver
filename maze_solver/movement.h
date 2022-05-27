#pragma once

#define leftMotorSpeedPin    3

#define leftMotorDirection1  7
#define leftMotorDirection2  8

#define rightMotorSpeedPin   6

#define rightMotorDirection1 4
#define rightMotorDirection2 5

#define globalSpeed 100


// Delays for 90 degree turns
#define turnLeftDelay 250
#define turnRightDelay 250

// Delay to move the car forward so that the wheels are at where the IR sensors were 
#define forwardDelay 100

// Opposite of the above function, the car seems to go back further for the same speed.
#define backwardDelay 170



#define maxspeedSTR 100
#define Kp 0.7 /// 0.7//0.7
#define Kd 0.4 // 0.0//0.5

float CNYread[6];
#define ncounts              100

int countwhite = 0;
float error = 0;
float last_error = 0;
float line_pos = 0;


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
  moveLeftMotor(-globalSpeed);
  moveRightMotor(globalSpeed);
  delay(delayMs);

  moveLeftMotor(0);
  moveRightMotor(0);
}

void turnRight(int delayMs) {
  moveLeftMotor(globalSpeed);
  moveRightMotor(-globalSpeed);
  delay(delayMs);

  moveLeftMotor(0);
  moveRightMotor(0);
}

void forward(int delayMs) {
  moveLeftMotor(globalSpeed);
  moveRightMotor(globalSpeed);
  delay(delayMs);

  moveLeftMotor(0);
  moveRightMotor(0);
  delay(1000);
}
 
void backward(int delayMs) {
  moveLeftMotor(-globalSpeed);
  moveRightMotor(-globalSpeed);
  delay(delayMs);

  moveLeftMotor(0);
  moveRightMotor(0);
}

void followLine(){
  int count = 0;

  CNYread[0] = (1024 - analogRead(A1)) / 10 * 1;
  CNYread[1] = (1024 - analogRead(A2)) / 10 * 2;
  CNYread[2] = (1024 - analogRead(A3)) / 10 * 3;
  CNYread[3] = (1024 - analogRead(A4)) / 10 * 4;
  CNYread[4] = (1024 - analogRead(A5)) / 10 * 5;

  line_pos = (CNYread[0] + CNYread[1] + CNYread[2] + CNYread[3] + CNYread[4]);
  // Serial.println(line_pos);
  last_error = error;
  error = line_pos - 390; // 300 is start line pos

  float correction = Kp * error + Kd * (error - last_error);


  if (count < 5)
    countwhite = 0;
  if (count == 5)
    countwhite++;
  if (countwhite < 20) { // after 40 times of all ir are white stop
    moveLeftMotor(maxspeedSTR - correction);
    moveRightMotor(maxspeedSTR + correction);
  } else {
    moveLeftMotor(0);
    moveRightMotor(0);
  }
}