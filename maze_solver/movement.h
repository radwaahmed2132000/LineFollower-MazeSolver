/*
 * set speed of frist motor
 * @param speed int
 * @return void
 */
void setMot1Speed(int speed) {
  if (speed > maxspeed) speed = maxspeed;
  if (speed > 0) {
    digitalWrite(mot1_dir1, HIGH);
    digitalWrite(mot1_dir2, LOW);
    analogWrite(mot1_speed, speed);
  } else {
    digitalWrite(mot1_dir1, LOW);
    digitalWrite(mot1_dir2, HIGH);
    analogWrite(mot1_speed, 0);
  }
}
/*
 * set speed of second motor
 * @param speed int
 * @return void
 */
void setMot2Speed(int speed) {
  if (speed > maxspeed) speed = maxspeed;
  if (speed > 0) {
    digitalWrite(mot2_dir1, HIGH);
    digitalWrite(mot2_dir2, LOW);
    analogWrite(mot2_speed, speed);
  } else {
    digitalWrite(mot2_dir1, LOW);
    digitalWrite(mot2_dir2, HIGH);
    analogWrite(mot2_speed, 0);
  }
}
/*
 * read IR sensor value
 * @return void
 */
void readSensor() {
  // read ir sensor & add weights for it
  CNYread[0] = (1024 - analogRead(A1)) / 10 * 1;
  CNYread[1] = (1024 - analogRead(A2)) / 10 * 2;
  CNYread[2] = (1024 - analogRead(A3)) / 10 * 3;
  CNYread[3] = (1024 - analogRead(A4)) / 10 * 4;
  CNYread[4] = (1024 - analogRead(A5)) / 10 * 5;
  // end of analog
}
/*
 *  calculate correction value for PD controller
 * @return void
 */
void calculateCorrection() {
  // sum values to get line pos
  line_pos = (CNYread[0] + CNYread[1] + CNYread[2] + CNYread[3] + CNYread[4]);
  // store previous error
  last_error = error;
  error = line_pos - initalpos;
  correction = Kp * error + Kd * (error - last_error);
}
void followLine() {
  readSensor();
  calculateCorrection();
  setMot1Speed(maxspeedSTR - correction);
  setMot2Speed(maxspeedSTR + correction);
}
