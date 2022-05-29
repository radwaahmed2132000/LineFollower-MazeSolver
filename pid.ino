#include <util/delay.h>


#define DEBUG


int Sensors[5];
#define mot1_speed 3
#define mot1_dir1 4
#define ncounts 90
#define mot1_dir2 5
#define mot2_speed 6
#define mot2_dir1 7
#define mot2_dir2 8
#define maxspeedSTR 50
// #define Kp 0.07///0.7//0.7
// #define Kd 0.04 //0.0//0.5

#define Kp  0.05f
#define Ki  0.005f
#define Kd  0.05f

int  opt_reading[5]= {970,970,970,945,945};
/*________________*/

float error_curr=0;
float error_prev=0;
float time_curr=0;
float time_prev=0;


/*________________*/
float integral;
float diff;
float prop;


float time_elapsed;


typedef struct {
  float delta;
  int index;
} error_info_t;


float pid_control(){
  time_curr = millis();
  time_elapsed = time_curr - time_prev;
  time_prev = time_curr;

  integral += Ki * (time_elapsed * error_curr);
  diff = Kd * (error_curr - error_prev) / time_elapsed;
  prop = Kp*error_curr;
  
  error_prev = error_curr;

  return integral+diff+prop; 
}


#pragma region Motion


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
#pragma endregion


error_info_t calculate_error(){
  float differences[5];
  for(int i = 0;i<5;i++)
  {
    differences[i] = Sensors[i] - opt_reading[i];
  }
  float max_diff = abs(differences [0]);
  int max_diff_index = 0;
  for(int i=1;i<5;i++){
    max_diff =  max_diff < abs(differences[i])  ? (max_diff_index = i , abs(differences[i])):max_diff;
  }
  return {differences[max_diff_index], max_diff_index};

}


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
 time_prev = millis();
}

void loop(){
 Sensors[0]=analogRead(A1);
 Sensors[1]=analogRead(A2);
 Sensors[2]=analogRead(A3);
 Sensors[3]=analogRead(A4);
 Sensors[4]=analogRead(A5);

 error_info_t delta_index = calculate_error();
 error_curr = delta_index.delta; 
 float correction = pid_control();

// #undef DEBUG
#ifdef DEBUG
Serial.print(Sensors[0]);
Serial.print("|");
Serial.print(Sensors[1]);
Serial.print("|");
Serial.print(Sensors[2]);
Serial.print("|");
Serial.print(Sensors[3]);
Serial.print("|");
Serial.println(Sensors[4]);
Serial.print(delta_index.delta);
Serial.print(" | ");
Serial.print(delta_index.index);
Serial.print(" | ");
Serial.println(correction);
#endif

 
}
