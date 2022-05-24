#include "maze_solver.h"

float CNYread[6];
#define ncounts              100

#define MAX_PATH 64
char path[MAX_PATH];
int currentIndex = 0;


#define maxspeedSTR 100
#define Kp 0.7 /// 0.7//0.7
#define Kd 0.4 // 0.0//0.5

int countwhite = 0;
float error = 0;
float last_error = 0;
float line_pos = 0;


void setup() {
  // IR Sensors
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
  MakeDecision(path, currentIndex, MAX_PATH);

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
