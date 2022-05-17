#include <QTRSensors.h>

#define SETPOINT    2500  // The goal for readLine (center)
#define KP           0.02  // The P value in PID
#define KD          0     // The D value in PID
#define L_MOTOR     6     // Left motor pin
#define R_MOTOR     3    // Right motor pin
#define MAX_SPEED   100   // The max speed to set motors to
#define SET_SPEED   100   // The goal speed to set motors to
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

// PID **************************************
int lastError = 0;  // For storing PID error

// SENSORS **********************************
// sensors 0 through 7 are connected to digital pins 3 through 10, respectively
//QTRSensorsRC qtrSensors((unsigned char[]) {A0, A1, A2, A3, A4, A5}, NUM_SENSORS, TIMEOUT, EMITTER_PIN);
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
  // Take a reading
  sensorValues[0]= analogRead(A0);
  sensorValues[1]= analogRead(A1);
  sensorValues[2]= analogRead(A2);
  sensorValues[3]= analogRead(A3);
  sensorValues[4]= analogRead(A4);
  sensorValues[5]= analogRead(A5);
  int count =0; 
// sensorValues[0] =; // 22
 sensorValues[1] *=2; //106
 sensorValues[1] = constrain(sensorValues[1], 100,800);
// sensorValues[2] =; //61
 sensorValues[3] *=0.4; //175
 sensorValues[4] *=1.20; //225
  sensorValues[4] = constrain(sensorValues[4], 100,800);
 sensorValues[5] *=0.5; //209
  
 Serial.print(sensorValues[0]);
 
 Serial.print("|");
 Serial.print(sensorValues[1]);

 
 Serial.print("|");

 Serial.print(sensorValues[2]);
 
 Serial.print("|");

 Serial.print(sensorValues[3]);
 
  Serial.print("|");

 Serial.print(sensorValues[4]);

 
 Serial.print("|");

 Serial.println(sensorValues[5]);
 

  for(int i=0;i<6;i++)
  {

      if(sensorValues[i]>200)
      {
        sensorValues[i]=0;
        count++;
      }
      else{
        sensorValues[i]=1;
        }
        
    
    
    }
    
  
  int sum=0,avg =0;
  for(int i=0;i<6;i++)
  {
    sum+= sensorValues[i];
    avg += sensorValues[i]*i*1000;
     //.print("sensorValues[0]");
// Serial.print(sensorValues[i]*i*1000);
// Serial.print("|");

    }
    
   // Serial.println("finish");


 
  unsigned int linePos = avg/sum;
  Serial.println(linePos);
  Serial.print("Error");

  // Compute the error
  int error = SETPOINT - linePos;
   Serial.println(error);
  // Compute the motor adjustment
  int adjust = error*KP + KD*(error - lastError);

  // Record the current error for the next iteration
  lastError = error;

  // Adjust motors, one negatively and one positively
  if(count<6){
      analogWrite(L_MOTOR, constrain(SET_SPEED - adjust, MIN_SPEED, MAX_SPEED));
  analogWrite(R_MOTOR, constrain(SET_SPEED + adjust, MIN_SPEED, MAX_SPEED));
    
    }
    else{
       analogWrite(L_MOTOR,0);
  analogWrite(R_MOTOR, 0);
      
      
      }

}
