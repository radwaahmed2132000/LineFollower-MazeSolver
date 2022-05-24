#include "movement.h"

// TODO: check whether this is correct or not
#define leftMostIR 5
#define leftCenterIR 4
#define centerIR 3
#define rightCenterIR 2
#define rightMostIR 1

#define IRThreshold 400

// Reads the leftmost, rightmost, and center IR sensors
// 000 -> 0
// 001 -> 11
// 010 -> 5
// 011 -> 16
// 100 -> 1
// 101 -> 12
// 110 -> 6
// 111 -> 17
#define leftFork    6
#define rightFork   16
#define noPath      0

#define onlyForward 5
#define onlyRight 11

#define tFork 17


#define debugIR 1
#define debugDecision 0

int readSensors(){
  int leftReading = analogRead(leftMostIR);
  int rightReading = analogRead(rightMostIR);
  int centerReading = analogRead(centerIR);

  int leftBlack = leftReading < IRThreshold;
  int centerBlack = centerReading < IRThreshold;
  int rightBlack = rightReading < IRThreshold;

  int reading = leftBlack * 1 +
                centerBlack * 5 +
                rightBlack * 11;

#if debugIR == 1
  char printBuff[32];
  sprintf(printBuff, "%d|%d|%d, reading: %d", leftReading, centerReading, rightReading, reading);
  Serial.println(printBuff);
#endif

  return reading;
}

void MakeDecision(char* path, int &currentIndex, int pathLength){
  int sensorReading = readSensors();

  if(currentIndex == pathLength - 1){
    Serial.println("PATH ARRAY FULL");
  }

  char decision = ' ';

  // LSRB Algorithm

  switch(sensorReading){

    case tFork:
    case leftFork: {
      forward(forwardDelay);
      turnLeft(turnLeftDelay);

      decision = 'L';
      break;
    }

    case rightFork: {
      float nudge = 1/10;         // Go forward a bit to check for a straight road

      // Used for when there's a forward path to go the rest of the way
      // So that we go forward (1*forwardDelay) in total
      float remainder = 1-nudge;  
      
          // Go forward a bit to check if there's a straight road ahead
      // forwardDelay/10 is an arbitrary choice here
      forward(forwardDelay*nudge);

      sensorReading = readSensors();

      // There's also a forwad path
      if(sensorReading == onlyForward){
        forward(forwardDelay * remainder);
        decision = 'F';
      } 
      // No forward path, only a right path
      else {
        forward(forwardDelay);
        turnRight(turnRightDelay);
        decision = 'R';
      }

      break;
    }

    case onlyForward: {
      forward(forwardDelay);

      decision = 'F';
      break;
    }

    case noPath: {
      turnLeft(2*turnLeftDelay);

      decision = 'B';
      break;
    }

    default: {
      // Do nothing
      decision = 'X';
      break;
    }

    path[currentIndex] = decision;
    currentIndex++;
  }

#if debugDecision == 1
  char printBuff[11];
  sprintf(printBuff, "Decision %c",decision);
  Serial.println(printBuff);
#endif

}