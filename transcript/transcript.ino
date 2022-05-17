#include <QTRSensors.h>

#define SETPOINT    0.75  // The goal for readLine (center)
#define KP          0.02  // The P value in PID
#define KD          0.01     // The D value in PID
#define L_MOTOR     6     // Left motor pin
#define R_MOTOR     3    // Right motor pin
#define MAX_SPEED   140   // The max speed to set motors to
#define SET_SPEED   140   // The goal speed to set motors to
#define MIN_SPEED   0     // The min speed to set motors to
#define NUM_SENSORS 6     // The number of QTR sensors
#define TIMEOUT     2500  // Timeout for the QTR sensors to go low
#define EMITTER_PIN 2     // Emitter pin for QTR sensor emitters
#define mot1_speed 3
#define mot1_dir1 4
#define mot1_dir2 5
#define mot2_speed 6
#define mot2_dir1 7
#define mot2_dir2 8
#define  counter 500

// PID **************************************
int lastError = 0;  // For storing PID error

// SENSORS **********************************
// sensors 0 through 7 are connected to digital pins 3 through 10, respectively
unsigned int sensorValues[NUM_SENSORS];   // For sensor values of readLine()

void setup() {
  // Initialize Pins
  pinMode(L_MOTOR, OUTPUT);
  pinMode(R_MOTOR, OUTPUT);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(A4,INPUT);
  pinMode(A5,INPUT);
  pinMode(mot1_speed,OUTPUT);
  pinMode(mot1_dir1,OUTPUT);
  pinMode(mot1_dir2,OUTPUT);
  pinMode(mot2_speed,OUTPUT);
  pinMode(mot2_dir1,OUTPUT);
  pinMode(mot2_dir2,OUTPUT);
  digitalWrite(mot1_dir1,HIGH);
  Serial.begin(9600);
  digitalWrite(mot1_dir2,LOW);
  digitalWrite(mot2_dir1,HIGH);
  digitalWrite(mot2_dir2,LOW);
}

void loop() {
  // Take a reading'
//  for(int i=0;i<5;i++)
//  {
    Serial.print( analogRead(A0));
    Serial.print("|");
    Serial.print( analogRead(A1));
    Serial.print("|");
    Serial.print( analogRead(A2));
    Serial.print("|");
    Serial.print( analogRead(A3));
    Serial.print("|");
    Serial.print( analogRead(A4));
    Serial.print("|");
//    }
  for (int  j=0;j<counter ;j++){
    sensorValues[0]+= analogRead(A0);
    sensorValues[1]+= analogRead(A1);
    sensorValues[2]+= analogRead(A2);
    sensorValues[3]+= analogRead(A3);
    sensorValues[4]+= analogRead(A4);
  //  sensorValues[5]+= analogRead(A5);
 }

 float sum=0,avg =0;
 for(int i=0;i<5;i++) {
  sum += sensorValues[i];
  avg += sensorValues[i]*i*1000;
 }

  //avg /=6;

  float linePos = avg/sum;
  Serial.println(linePos);
//  Serial.print();

  // Compute the error
  float error = SETPOINT - linePos;
  float adjust = error*KP + KD*(error - lastError);

  // Record the current error for the next iteration
  lastError = error;
  int count =0;
  
  // Adjust motors, one negatively and one positively
 
      analogWrite(L_MOTOR, constrain(SET_SPEED - adjust, MIN_SPEED, MAX_SPEED));
      analogWrite(R_MOTOR, constrain(SET_SPEED + adjust, MIN_SPEED, MAX_SPEED));
    
   
}
