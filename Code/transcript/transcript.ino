#include <util/delay.h>
#include <stdio.h>
#include <string.h>


float CNYread[5]; // array of IR sensor
#define mot1_speed 3 // pin speed of motor1
#define mot1_dir1 4 // direction 1 for motor 1
#define mot1_dir2 5  // direction 2 for motor 1 
#define mot2_speed 6  // pin speed of motor2
#define mot2_dir1 7 // direction 1 for motor 2
#define mot2_dir2 8 // direction 2 for motor 2
#define maxspeedSTR 70 // max start speed value
#define maxspeed 100 // max speed value
#define Kp 0.7 
#define Kd 0.4
float error=0;
float last_error=0;
float line_pos=0;
float initalpos =390; // 390 is start line pos
float correction =0;
  /*
  * setup input & output
  * @return void
  */
void setup() {
  // sensor pins
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(A4,INPUT);
  pinMode(A5,INPUT);
  // motor pins
  pinMode(mot1_speed,OUTPUT);
  pinMode(mot1_dir1,OUTPUT);
  pinMode(mot1_dir2,OUTPUT);
  pinMode(mot2_speed,OUTPUT);
  pinMode(mot2_dir1,OUTPUT);
  pinMode(mot2_dir2,OUTPUT);
  // Serial begin
  Serial.begin(9600);
}
  /*
  * set speed of frist motor
  * @param speed int
  * @return void
  */
void setMot1Speed(int speed){
  if (speed>maxspeed) speed=maxspeed;
  if(speed>0){
    digitalWrite(mot1_dir1,HIGH);
    digitalWrite(mot1_dir2,LOW);
    analogWrite(mot1_speed,speed);
  }else{
    digitalWrite(mot1_dir1,LOW);
    digitalWrite(mot1_dir2,HIGH);
    analogWrite(mot1_speed,0);
  }
}
  /*
  * set speed of second motor
  * @param speed int
  * @return void
  */
void setMot2Speed(int speed){
  if (speed>maxspeed) speed=maxspeed ;
  if(speed>0){
    digitalWrite(mot2_dir1,HIGH);
    digitalWrite(mot2_dir2,LOW);
    analogWrite(mot2_speed,speed);
  }else{
    digitalWrite(mot2_dir1,LOW);
    digitalWrite(mot2_dir2,HIGH);
    analogWrite(mot2_speed,0);
  }
}
  /*
  * read IR sensor value
  * @return void
  */
void readSensor()
{
   // read ir sensor & add weights for it
   CNYread[0]=(1024-analogRead(A1))/10*1;
   CNYread[1]=(1024-analogRead(A2))/10*2;
   CNYread[2]=(1024-analogRead(A3))/10*3;
   CNYread[3]=(1024-analogRead(A4))/10*4;
   CNYread[4]=(1024-analogRead(A5))/10*5;
   // end of analog
  
  }
  /*
  *  calculate correction value for PD controller
  * @return void
  */
  void calculateCorrection()
  {
    // sum values to get line pos
    line_pos=(CNYread[0]+CNYread[1]+CNYread[2]+CNYread[3]+CNYread[4]);
    // store previous error
    last_error = error;
    error =line_pos-initalpos;
    correction=Kp*error+Kd*(error-last_error);
    }
void loop() {

   readSensor();
   calculateCorrection();
   setMot1Speed(maxspeedSTR-correction);
   setMot2Speed(maxspeedSTR+correction);


}
