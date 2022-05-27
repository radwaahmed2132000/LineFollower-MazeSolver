#include "movement.h"

#define leftMostIR A5
#define leftCenterIR A4
#define centerIR A3
#define rightCenterIR A2
#define rightMostIR A1


#define IRThreshold 400

#define leftFork    0b11100
#define rightFork   0b00111
#define noPath      0b00000

#define onlyForward 0b00100
#define onlyRight   0b00111
#define tFork       0b11111


#define leftMostBit 4
#define leftCenterBit 3
#define CenterBit 2
#define rightMostBit 1
#define rightCenterBit 0



#define debugIR 1
#define debugDecision 1

int readSensors(){
  int leftMostReading = analogRead(leftMostIR);
  int leftCenterReading = analogRead(leftCenterIR);
  int rightMostReading = analogRead(rightMostIR);
  int rightCenterReading = analogRead(rightCenterIR);
  int centerReading = analogRead(centerIR);

  int leftMostBlack = leftMostReading < IRThreshold;
  int leftCenterBlack = leftCenterReading < IRThreshold;

  int centerBlack = centerReading < IRThreshold;

  int rightMostBlack = rightMostReading < IRThreshold;
  int rightCenterBlack = rightCenterReading < IRThreshold;

  int reading = leftMostBlack << leftMostBit |
                leftCenterBlack << leftCenterBit |
                centerBlack << CenterBit |
                rightCenterBlack << rightCenterBit |
                rightMostBlack << rightMostBit;

#if debugIR == 1
  char printBuff[64];
  sprintf(printBuff, "%d|%d|%d|%d|%d, reading: %d", leftMostReading, leftCenterReading, centerReading, rightCenterReading, rightMostReading, reading);
  Serial.println(printBuff);
#endif

  return reading;
}

void MakeDecision(char* path, int &currentIndex, int pathLength){
  
  followLine();
  return;

  int sensorReading = readSensors();

  if(currentIndex == pathLength - 1){
    Serial.println("PATH ARRAY FULL");
  }

  char decision = ' ';

  // LSRB Algorithm

  switch(sensorReading){

    case tFork:
    case leftFork: {
      // forward(forwardDelay);
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
        // forward(forwardDelay);
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
      moveLeftMotor(0);
      moveRightMotor(0);
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