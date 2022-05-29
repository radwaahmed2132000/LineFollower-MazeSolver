#define LEFT_MOST_IR A5
#define LEFT_CENTER_IR A4
#define CENTER_IR A3
#define RIGHT_CENTER_IR A2
#define RIGHT_MOST_IR A1

#define IR_THRESHOLD 400

#define LEFT_FORK 0b11100
#define RIGHT_FORK 0b00111
#define NO_PATH 0b00000

#define ONLY_FORWARD 0b00100
#define ONLY_RIGHT 0b00111
#define T_FORK 0b11111

#define LEFT_MOST_BIT 4
#define LEFT_CENTER_BIT 3
#define CENTER_BIT 2
#define RIGHT_MOST_BIT 1
#define RIGHT_CENTER_BIT 0

#define DEBUG_IR 1
#define DEBUG_DECISION 1

int readSensors() {
  int leftMostReading = analogRead(LEFT_MOST_IR);
  int leftCenterReading = analogRead(LEFT_CENTER_IR);
  int rightMostReading = analogRead(RIGHT_MOST_IR);
  int rightCenterReading = analogRead(RIGHT_CENTER_IR);
  int centerReading = analogRead(CENTER_IR);

  int leftMostBlack = leftMostReading < IR_THRESHOLD;
  int leftCenterBlack = leftCenterReading < IR_THRESHOLD;

  int centerBlack = centerReading < IR_THRESHOLD;

  int rightMostBlack = rightMostReading < IR_THRESHOLD;
  int rightCenterBlack = rightCenterReading < IR_THRESHOLD;

  int reading = leftMostBlack << LEFT_MOST_BIT |
                leftCenterBlack << LEFT_CENTER_BIT | centerBlack << CENTER_BIT |
                rightCenterBlack << RIGHT_CENTER_BIT |
                rightMostBlack << RIGHT_MOST_BIT;

#if DEBUG_IR == 1
  char printBuff[64];
  sprintf(printBuff, "%d|%d|%d|%d|%d, reading: %d", leftMostReading,
          leftCenterReading, centerReading, rightCenterReading,
          rightMostReading, reading);
  Serial.println(printBuff);
#endif

  return reading;
}

void MakeDecision(char* path, int& currentIndex, int pathLength) {
  followLine();

  int sensorReading = readSensors();

  if (currentIndex == pathLength - 1) {
    Serial.println("PATH ARRAY FULL");
  }

  char decision = ' ';

  // LSRB Algorithm

  switch (sensorReading) {
    case T_FORK:

    case LEFT_FORK: {
      // forward(forwardDelay);
      turnLeft(turnLeftDelay);
      path[currentIndex] = 'L';
      currentIndex++;
      break;
    }

    case RIGHT_FORK: {
      // Go forward a bit to check for a straight road
      float nudge = 1 / 10;

      // Used for when there's a forward path to go the rest of the way
      // So that we go forward (1*forwardDelay) in total
      float remainder = 1 - nudge;

      // Go forward a bit to check if there's a straight road ahead
      // forwardDelay/10 is an arbitrary choice here
      forward(forwardDelay * nudge);

      sensorReading = readSensors();

      // There's also a forwad path
      if (sensorReading == ONLY_FORWARD) {
        forward(forwardDelay * remainder);
        decision = 'F';
      }
      // No forward path, only a right path
      else {
        // forward(forwardDelay);
        turnRight(turnRightDelay);
        decision = 'R';
      }
      path[currentIndex] = decision;
      currentIndex++;

      break;
    }

    case ONLY_FORWARD: {
      forward(forwardDelay);
      path[currentIndex] = 'F';
      currentIndex++;
      break;
    }

    case NO_PATH: {
      turnLeft(2 * turnLeftDelay);
      path[currentIndex] = 'B';
      currentIndex++;
      break;
    }

    default: {
      // Do nothing
      moveLeftMotor(0);
      moveRightMotor(0);
      break;
    }
  }

#if DEBUG_DECISION == 1
  char printBuff[11];
  sprintf(printBuff, "Decision %c", decision);
  Serial.println(printBuff);
#endif
}