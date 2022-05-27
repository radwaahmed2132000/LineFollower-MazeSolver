#include "maze_solver.h"


#define MAX_PATH 64
char path[MAX_PATH];
int currentIndex = 0;



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
}
