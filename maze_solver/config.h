#define MAX_PATH 64
char path[MAX_PATH];
int currentIndex = 0;

float CNYread[5];       // array of IR sensor
//================================
#define mot1_speed 3    // pin speed of motor1
#define mot1_dir1 4     // direction 1 for motor 1
#define mot1_dir2 5     // direction 2 for motor 1
#define mot2_speed 6    // pin speed of motor2
#define mot2_dir1 7     // direction 1 for motor 2
#define mot2_dir2 8     // direction 2 for motor 2
//================================
#define maxspeedSTR 70  // max start speed value
#define maxspeed 100    // max speed value
#define Kp 0.7
#define Kd 0.4
//================================
float error = 0;
float last_error = 0;
float line_pos = 0;
float initalpos = 390;  // 390 is start line pos
float correction = 0;
//================================
#define LEFT_MOST_IR A5
#define LEFT_CENTER_IR A4
#define CENTER_IR A3
#define RIGHT_CENTER_IR A2
#define RIGHT_MOST_IR A1
//================================
#define IR_THRESHOLD 400
//================================
#define LEFT_FORK 0b11100
#define RIGHT_FORK 0b00111
#define NO_PATH 0b00000
#define ONLY_FORWARD 0b00100
#define ONLY_RIGHT 0b00111
#define T_FORK 0b11111
//================================
#define LEFT_MOST_BIT 4
#define LEFT_CENTER_BIT 3
#define CENTER_BIT 2
#define RIGHT_MOST_BIT 1
#define RIGHT_CENTER_BIT 0
//================================
#define DEBUG_IR 1
#define DEBUG_DECISION 1
//================================
