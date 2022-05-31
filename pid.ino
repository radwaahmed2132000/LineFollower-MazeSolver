#include <util/delay.h>

#pragma region DEBUG_MACROS
#define DEBUG
#define DEBUG_IR
#define DEBUG_DECISION 1
#define DEBUG_SENSORS_ERROR_CORRECTION \
  {                                    \
    Serial.print(Sensors[0]);          \
    Serial.print("|");                 \
    Serial.print(Sensors[1]);          \
    Serial.print("|");                 \
    Serial.print(Sensors[2]);          \
    Serial.print("|");                 \
    Serial.print(Sensors[3]);          \
    Serial.print("|");                 \
    Serial.println(Sensors[4]);        \
  }
  // Serial.print(delta_index.delta);  \
    Serial.print(" | ");               \
    Serial.print(delta_index.index);   \
    Serial.print(" | ");               \
    //Serial.println(correction);        
#pragma endregion

#pragma region __Pins__
//==============================
#pragma region MotorPins

#define RIGHT_MOTOR_SPEED 3
#define RIGHT_MOTOR_F 4
#define RIGHT_MOTOR_B 5
#define LEFT_MOTOR_SPEED 6
#define LEFT_MOTOR_F 7
#define LEFT_MOTOR_B 8
#define maxspeedSTR 50

#pragma endregion
//==============================
#pragma region SensorPins

#define LEFT_MOST_IR 4
#define LEFT_CENTER_IR 3
#define CENTER_IR 2
#define RIGHT_CENTER_IR 1
#define RIGHT_MOST_IR 0

#pragma endregion
//==============================
#pragma endregion

#pragma region Maze_Shapes

#define LEFT_FORK_PATH 0b11100
#define RIGHT_FORK_PATH 0b00111
#define T_FORK_PATH 0b11111
#define NO_PATH 0b00000

#pragma endregion

#pragma region CarPositions

#define IR_THRESHOLD 400
#define _CENTER_ 0b00100
#define _LEFT_ 0b11000
#define _RIGHT_ 0b00011
#define IS_CENTERED (read_position() & _CENTER_)
#define IS_LEFT (read_position() & _LEFT_)
#define IS_RIGHT (read_position() & _RIGHT_)

#pragma endregion

/*________________*/
int Sensors[5];
/*________________*/
#define Kp 0.05f
#define Ki 0.005f
#define Kd 0.05f
/*________________*/
int opt_reading[5] = {970, 970, 970, 945, 945};
/*________________*/
float error_curr = 0;
float error_prev = 0;
float time_curr = 0;
float time_prev = 0;
/*________________*/
float integral;
float diff;
float prop;
/*________________*/
float time_elapsed;
/*________________*/
typedef struct {
  float delta;
  int index;
} error_info_t;
/*________________*/
//=================================
#pragma region MotorMotion

void set_right_motor_speed(int speed) {
  if (speed > 100) speed = 100;
  if (speed > 0) {
    digitalWrite(RIGHT_MOTOR_F, HIGH);
    digitalWrite(RIGHT_MOTOR_B, LOW);
    analogWrite(RIGHT_MOTOR_SPEED, speed);
  } else {
    digitalWrite(RIGHT_MOTOR_F, LOW);
    digitalWrite(RIGHT_MOTOR_B, HIGH);
    analogWrite(RIGHT_MOTOR_SPEED, speed);
  }
}
void set_left_motor_speed(int speed) {
  if (speed > 100) speed = 100;
  if (speed > 0) {
    digitalWrite(LEFT_MOTOR_F, HIGH);
    digitalWrite(LEFT_MOTOR_B, LOW);
    analogWrite(LEFT_MOTOR_SPEED, speed);
  } else {
    digitalWrite(LEFT_MOTOR_F, LOW);
    digitalWrite(LEFT_MOTOR_B, HIGH);
    analogWrite(LEFT_MOTOR_SPEED, speed);
  }
}
#pragma endregion
//=================================
void read_sensors() {
  Sensors[0] = analogRead(A1);
  Sensors[1] = analogRead(A2);
  Sensors[2] = analogRead(A3);
  Sensors[3] = analogRead(A4);
  Sensors[4] = analogRead(A5);
}
//=================================
/*
 ! Returns a bit sequence (a byte) that determines the current positioning of
 ! the car.
 ! INVARIANT : read_sensors() can only be called in this function
*/
uint8_t read_position() {
  // ? neccessary to update the sensors' readings
  // ? try to only call it here
  read_sensors();
  uint8_t leftMostBlack = Sensors[LEFT_MOST_IR] < IR_THRESHOLD;
  uint8_t leftCenterBlack = Sensors[LEFT_CENTER_IR] < IR_THRESHOLD;
  uint8_t centerBlack = Sensors[CENTER_IR] < IR_THRESHOLD;
  uint8_t rightCenterBlack = Sensors[RIGHT_CENTER_IR] < IR_THRESHOLD;
  uint8_t rightMostBlack = Sensors[RIGHT_MOST_IR] < IR_THRESHOLD;

  uint8_t reading =
      leftMostBlack << LEFT_MOST_IR | leftCenterBlack << LEFT_CENTER_IR |
      centerBlack << CENTER_IR | rightCenterBlack << RIGHT_CENTER_IR |
      rightMostBlack << RIGHT_MOST_IR;


  return reading;
}
//=================================
#pragma region Direction_Movement

void move_forward(int speed) {
  set_left_motor_speed(speed);
  set_right_motor_speed(speed);
  while (IS_CENTERED)
    ;
  set_left_motor_speed(0);
  set_right_motor_speed(0);
}

void move_backward(int speed) {
  set_left_motor_speed(-speed);
  set_right_motor_speed(-speed);
  while (IS_CENTERED)
    ;
  set_left_motor_speed(0);
  set_right_motor_speed(0);
}
/*
  ! This is similar to rotate_left, except it rotates until it sees a line,
  ! **NOT**  until it is centered, rotation is counterclockwise
*/
void rotate_until_line(int speed) {
  set_left_motor_speed(-speed);
  set_right_motor_speed(speed);
  while (!read_position())
    ;
  set_left_motor_speed(0);
  set_right_motor_speed(0);
}

/*
 ! Checks if the center sensor is on black if not; keeps rotating left, until
 ! corrected.
 */
void rotate_left(int speed) {
  set_left_motor_speed(-speed);
  set_right_motor_speed(speed);

  while (!IS_CENTERED)
    ;
  set_left_motor_speed(0);
  set_right_motor_speed(0);
}
//=================================

void rotate_right(int speed) {
  set_left_motor_speed(speed);
  set_right_motor_speed(-speed);
  while (!IS_CENTERED)
    ;

  set_left_motor_speed(0);
  set_right_motor_speed(0);
}
#pragma endregion

inline void follow_and_solve(char*, int*, int) __attribute__((always_inline));
void follow_and_solve(char* path, int* currentIndex, int pathLength) {
  if (*currentIndex == pathLength - 1) {
    Serial.println("PATH ARRAY FULL");
  }

  //! XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  //? WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
  int speed = 60;
  //? WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
  //! XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

  char decision = ' ';
  switch (read_position()) {

    case NO_PATH: {
      rotate_until_line(speed);
      break;
    }
    case T_FORK_PATH:
    case LEFT_FORK_PATH: {
      rotate_left(speed);
      break;
    }
    case RIGHT_FORK_PATH: {
      // ? This should work, there are two cases:
      // ? 1) There is a forward path, then the car will move forward until the
      // ? car is not centered and start again
      // ? 2) There is no forward path, then  the car will move forward, find
      // ? that there is no path, the next ? iteration, it rotates a full
      // ? rotation until it finds a line, the new left after rotation is the
      // ? old right
      move_forward(speed);
      break;
    }
  }

  if (IS_CENTERED) {
    move_forward(speed);
  }
  if (IS_LEFT) {
    rotate_left(speed);
  }
  if (IS_RIGHT) {
    rotate_right(speed);
  }
}
//=================================

float pid_control() {
  time_curr = millis();
  time_elapsed = time_curr - time_prev;
  time_prev = time_curr;

  integral += Ki * (time_elapsed * error_curr);
  diff = Kd * (error_curr - error_prev) / time_elapsed;
  prop = Kp * error_curr;

  error_prev = error_curr;

  return integral + diff + prop;
}

error_info_t calculate_error() {
  float differences[5];
  for (int i = 0; i < 5; i++) {
    differences[i] = Sensors[i] - opt_reading[i];
  }
  float max_diff = abs(differences[0]);
  int max_diff_index = 0;
  for (int i = 1; i < 5; i++) {
    max_diff = max_diff < abs(differences[i])
                   ? (max_diff_index = i, abs(differences[i]))
                   : max_diff;
  }
  return {differences[max_diff_index], max_diff_index};
}

void setup() {
  // put your setup code here, to run once:
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(RIGHT_MOTOR_SPEED, OUTPUT);
  pinMode(RIGHT_MOTOR_F, OUTPUT);
  pinMode(RIGHT_MOTOR_B, OUTPUT);
  pinMode(LEFT_MOTOR_SPEED, OUTPUT);
  pinMode(LEFT_MOTOR_F, OUTPUT);
  pinMode(LEFT_MOTOR_B, OUTPUT);
  Serial.begin(9600);
  time_prev = millis();
}

#define MAX_PATH 64
char path[MAX_PATH];
int currentIndex[1] = {0};
void loop() {
  // error_info_t delta_index = calculate_error();
  // error_curr = delta_index.delta;
  // float correction = pid_control();
  follow_and_solve(path, currentIndex, MAX_PATH);
#ifdef DEBUG
  DEBUG_SENSORS_ERROR_CORRECTION
#endif
}
