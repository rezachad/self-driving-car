

/*
 The Project name:Jarvis
 This is property of: waylezz
Self driving REV 3.0 alspha basic structure left turn and right turn and moving forward and stopping 
5/2/19
-Reverse has been added
-more knobs have been added such as full speed and half speed and closing distance.
- restructured switch statements
- Drive loagic has been revidsed and updated
- turning and movement has been declared as functions for easier understanding
- adding closing distance capture to set colosing distances for left right and front and back 

trigger pins = pin 5
echo pins = 2 for front 
            7 for right 
            8 for left
            12 for rear
servo for steering is set to pin 11 
sensor servo is set to pin 10
*/
//Libraries included
#include <Servo.h>
//servo allocation 
Servo steering; 

// tuning knobs
 const int Center_angle = 90; // center steering angle in degrees threshold
 const int Left_angle = 155; //to turn left angle in degrees threshold
 const int Right_angle = 50; //to turn right angle in degrees threshold
 int closing_distance = 0; // closing distance threshhold front back
 int closing_distance_LR = 0; //closing distance threshhold left right  
 const int half_speed = 100; // 50% duty cycle
 const int full_speed = 127; // 100% duty cycle 
 const int stopped = 0; // shuts off electric motor
 const int setDelay = 50;// delays
 const int between_sensor_delay = 10; // delay for indidual sensor bring up
 const int sensor_timer_on = 60; //ultra sonic sensor timer
 const int sensor_timer_off = 60; //ultra sonic sensor timer 
//pin alocation
 static const int velo = 3; //motor speed using duty cycle
 static const int dir = 4; // forward or reverse  
 static const int trig = 5; // ultrasonic sensor trigger (pwm) front of car
 static const int front_echo = 2; // ultrasonic echo front of car
 static const int right_echo = 7; // ultrasonic echo front of car
 static const int left_echo = 8; // ultrasonic echo  of car
 static const int rear_echo = 12; // ultrasonic echo rear of car
 // sensor and state variables 
 int front_distance_cm = 0; // front of car distance in cm readout
 int right_distance_cm = 0; // right of car distance in cm readout
 int left_distance_cm = 0; //left of car distance in cm readout
 int rear_distance_cm = 0; // rear of car distance in cm readout
 int last_servo_state; // stores steering angle
 bool last_motor_state;// setting motor either high for forward or low for reverse 



void setup() //pin mode settings
{
  //pin setup
Serial.begin(9600);
steering.attach(11);
pinMode(velo,OUTPUT);
pinMode(dir,OUTPUT);
pinMode(trig,OUTPUT);
pinMode(front_echo,INPUT);
pinMode(right_echo,INPUT);
pinMode(left_echo,INPUT);
pinMode(rear_echo,INPUT);
last_servo_state = Center_angle;
last_motor_state = HIGH;
closing_distance_capture();

}

void loop() 
{
steering.write(last_servo_state);
digitalWrite(dir,last_motor_state);
analogWrite(velo,full_speed);
forward_reverse();
delay(setDelay);

}//end main
/*******************************************************/
 void sensor_call()
{

  
      front_sensor();
      delay(between_sensor_delay);  
      left_sensor();
      delay(between_sensor_delay);
      right_sensor(); 
      delay(between_sensor_delay); 
      rear_sensor(); 
      delay(between_sensor_delay);
  
}


/*****************sensor Setup***********/
void front_sensor() //front sensor setup
 {

long duration; // ultrasonic value storage
  
digitalWrite(trig,LOW);
delay(sensor_timer_off);
digitalWrite(trig,HIGH);
delay(sensor_timer_on);
digitalWrite(trig,LOW);

duration = pulseIn(front_echo,HIGH);
//delay(sensor_timer_off);
front_distance_cm = (duration/2)/29.1;
  Serial.print("front sensor: "); //for sensor debug
  Serial.print(front_distance_cm);
  Serial.print("cm   ");
  Serial.println();
 }
/*******************************************************/
void right_sensor() //right sensor setup
 {

long duration; // ultrasonic value storage
 
digitalWrite(trig,LOW);
delay(sensor_timer_off);
digitalWrite(trig,HIGH);
delay(sensor_timer_on);
digitalWrite(trig,LOW);

duration = pulseIn(right_echo,HIGH);
//delay(sensor_timer_off);

right_distance_cm = (duration/2)/29.1;
  Serial.print("right sensor: "); //for sensor debug
  Serial.print(right_distance_cm);
  Serial.print("cm   ");
  Serial.println();
 }
 /*******************************************************/
void left_sensor() //left sensor setup
 {

long duration; // ultrasonic value storage
  
digitalWrite(trig,LOW);
delay(sensor_timer_off);
digitalWrite(trig,HIGH);
delay(sensor_timer_on);
digitalWrite(trig,LOW);

duration = pulseIn(left_echo,HIGH);
//delay(sensor_timer_off);

left_distance_cm = (duration/2)/29.1;
  Serial.print("left sensor: "); //for sensor debug
  Serial.print(left_distance_cm);
  Serial.print("cm   ");
  Serial.println();
 }
/*******************************************************/
void rear_sensor() //rear sensor setup
 {

long duration; // ultrasonic value storage
  // front sonic_sensor test
digitalWrite(trig,LOW);
delay(sensor_timer_off);
digitalWrite(trig,HIGH);
delay(sensor_timer_on);
digitalWrite(trig,LOW);


duration = pulseIn(rear_echo,HIGH);
//delay(sensor_timer_off);

rear_distance_cm = (duration/2)/29.1;
  Serial.print("rear sensor: "); //for sensor debug
  Serial.print(rear_distance_cm);
  Serial.print("cm   ");
  Serial.println();
 }
 
void turn_logic()
 {
 
 sensor_call();
 delay(setDelay); 
 if (left_distance_cm <= closing_distance_LR ||  (right_distance_cm > left_distance_cm && front_distance_cm <= closing_distance)|| (right_distance_cm > left_distance_cm && rear_distance_cm <= closing_distance) || right_distance_cm > left_distance_cm )
      {
        last_servo_state = Right_angle;
        delay(setDelay);
        Serial.print("Turing Right");
        Serial.println();
        return;
      }
  else if (right_distance_cm <= closing_distance_LR || (right_distance_cm < left_distance_cm && front_distance_cm <= closing_distance)|| (right_distance_cm < left_distance_cm && rear_distance_cm <= closing_distance) || right_distance_cm < left_distance_cm)
      {
       last_servo_state = Left_angle;
       Serial.print("Turing Left");
       Serial.println();
       return;
      }
  else
      {    
       last_servo_state = Center_angle;
       Serial.print("Moving forward");
       Serial.println(); 
       }
 return;
 }

void forward_reverse()
{
  sensor_call();
  delay(setDelay);
  turn_logic();
  if ((front_distance_cm <= closing_distance && rear_distance_cm > closing_distance) || rear_distance_cm > front_distance_cm)
  {
    last_motor_state = LOW;
    return;
  }
  else if ((front_distance_cm > closing_distance || rear_distance_cm <= closing_distance))
  {
    last_motor_state = HIGH;
  }
  else 
  {
    last_motor_state = HIGH;
    return;
  }
}

void closing_distance_capture()
{
sensor_call();
delay(setDelay);

closing_distance = (front_distance_cm + rear_distance_cm )/2;
Serial.print("setting front rear sensor to: ");
Serial.print(closing_distance);
Serial.println();
       
closing_distance_LR = (right_distance_cm + left_distance_cm )/2;
Serial.print("setting left right  sensor to: ");
Serial.print(closing_distance_LR);
Serial.println();

if (closing_distance_LR > 100)
  {
    closing_distance_LR = 100;
    Serial.print("setting left right sensor to: ");
    Serial.print(closing_distance_LR);
    Serial.println();
  } 
else if ( closing_distance_LR < 10)
  {
    closing_distance_LR = 10;
    Serial.print("setting left right sensor to: ");
    Serial.print(closing_distance_LR);
    Serial.println();
  }
else if (closing_distance > 100)
  {
    closing_distance = 100; 
    Serial.print("setting front rear sensor to: ");
    Serial.print(closing_distance);
    Serial.println();
  }
else if ( closing_distance < 10)
  {
    closing_distance = 10; 
    Serial.print("setting front rear sensor to: ");
    Serial.print(closing_distance);
    Serial.println();
  }
  

}
