/*===============================*/
#include <util/delay.h>
#include "config.h"
#include "movement.h"
#include "maze_solver.h"
/*===============================*/

/*
 * setup input & output
 * @return void
 */
void setup() {
  // sensor pins
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  // motor pins
  pinMode(mot1_speed, OUTPUT);
  pinMode(mot1_dir1, OUTPUT);
  pinMode(mot1_dir2, OUTPUT);
  pinMode(mot2_speed, OUTPUT);
  pinMode(mot2_dir1, OUTPUT);
  pinMode(mot2_dir2, OUTPUT);
  // Serial begin
  Serial.begin(9600);
}
void loop() { MakeDecision(path, currentIndex, MAX_PATH); }
