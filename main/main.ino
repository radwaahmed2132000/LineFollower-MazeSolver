#include <util/delay.h>
#include <stdio.h>
#include <string.h>

float CNYread[6];
int countwhite =0;
#define mot1_speed 3
#define mot1_dir1 4
#define ncounts 90
#define mot1_dir2 5
#define mot2_speed 6
#define mot2_dir1 7
#define mot2_dir2 8
#define maxspeedSTR 50
#define Kp 0.07///0.7//0.7
#define Kd 0.04 //0.0//0.5
float error=0;
float last_error=0;
float line_pos=0;
void setup() {
  // put your setup code here, to run once:
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
  Serial.begin(9600);
}
void set_mot1_speed(int speed){
  if (speed>100) speed=100;
  if(speed>0){
    digitalWrite(mot1_dir1,HIGH);
    digitalWrite(mot1_dir2,LOW);
    analogWrite(mot1_speed,speed);
  }else{
    digitalWrite(mot1_dir1,LOW);
    digitalWrite(mot1_dir2,HIGH);
    analogWrite(mot1_speed,speed);
  }
}
void set_mot2_speed(int speed){
  if (speed>100) speed=100    ;
  if(speed>0){
    digitalWrite(mot2_dir1,HIGH);
    digitalWrite(mot2_dir2,LOW);
    analogWrite(mot2_speed,speed);
  }else{
    digitalWrite(mot2_dir1,LOW);
    digitalWrite(mot2_dir2,HIGH);
    analogWrite(mot2_speed,speed);
  }
}
void loop() {
  // put your main code here, to r  un repeatedly:
  // ---this is to make it digital IR
 CNYread[0]=analogRead(A1);
 CNYread[1]=analogRead(A2);
 CNYread[2]=analogRead(A3);
 CNYread[3]=analogRead(A4);
 CNYread[4]=analogRead(A5);
  int count =0;
//  for(int i=0;i<5;i++)
//  {
//    
//     if(CNYread[i]>=600) // threshold for white & black
//    {
//      count ++; // this counter for analog & digital to check if all of IR are white so stop 
//      CNYread[i]=;
//    }
//    else // i+1 is weight
//    CNYread[i]=100*(i+1);
//    }
  
// to  make it analog
  // Serial.print(analogRead(A1));
  // Serial.print("|");
  // Serial.print(analogRead(A2));
  // Serial.print("|");
  // Serial.print(analogRead(A3));
  // Serial.print("|");
  // Serial.print(analogRead(A4));
  // Serial.print("|");
  // Serial.println(analogRead(A5));
  
  int minIndex =0;
  int secondMinIndex=0; 
  for (int i=1;i<5;i++){
    if (CNYread[minIndex]>CNYread[i]){
      secondMinIndex=minIndex;
      minIndex=i;
    }
  }

  CNYread[0]=(1024-analogRead(A1))/100;
  CNYread[1]=(1024-analogRead(A2))/100;
  CNYread[2]=(1024-analogRead(A3))/100;
  CNYread[3]=(1024-analogRead(A4))/100;
  CNYread[4]=(1024-analogRead(A5))/100;
// end of analog 
float linePos= (minIndex+secondMinIndex)/2 - 1.5;

// Serial.println(line_pos);
last_error = error;
error = linePos * (CNYread[minIndex]+CNYread[secondMinIndex])/2;
//Serial.print("|");
Serial.println(error);
//Serial.print("|");
//Serial.print(
float correction=Kp*error+Kd*(error-last_error);
//Serial.println(correction);
//  Serial.println(correction);
//  Serial.println("\n");
//Serial.println("|");
//  correction=0;
// count white is how many times i found all irs are white 
if(count<5)
 countwhite =0;
if(count ==5)
countwhite++;
if(countwhite <20 ){ // after 40 times of all ir are white stop
   set_mot1_speed(maxspeedSTR+correction);
   set_mot2_speed(maxspeedSTR-correction);
}
else{
     set_mot1_speed(0);
    set_mot2_speed(0);
   
  }

}
