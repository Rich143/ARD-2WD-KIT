// Created for Abra's Robot using various sources
// Created on April 8th, 2014
// Created by Oscar and Subash
// Use this code to test your Adruino Robot with the Arduino board.
// Modified Date: 10/04/2014
#include <NewPing.h>            // Header for ping function - to test for the distance from the object
#define TRIGGER_PIN  4          // Trigger Pin on the Arduino Board for ultrasonic range finder
#define ECHO_PIN  2             // Echo Pin the Arduino Board for ultrasonic range finder
#define MAXIMUM_DISTANCE  250   // Maximum distance (range) for the purposes of Ping in centimeters
NewPing sonar (TRIGGER_PIN, ECHO_PIN, MAXIMUM_DISTANCE); // Setup sonar elements
float Thresholddistance = 20.0;  // Maximum detection range of the closest object in front, to the left or to // the right, in centimeters

#include <Servo.h>              // Header for the servo motor
#define pinServo 5              // Arduino Pin 5 for the Servo motor
//---------------------------------------------------------------------------- Set up Servo Angles
#define SERVOLEFT 180           // Left - Direction of the servo
#define SERVORIGHT 0            // Right - Direction of the servo
#define SERVOCENTER 90          // Center - Direction of the servo
Servo pingServo;                  // Create servo object to control a servo
int angleCur = 0;               // Initial direction of the servo
int speed = 255;                // Speed of the motors

// --------------------------------------------------------------------------- Motors
int motor_left[] = {3,9, 8};    // Initialize the pins for Left Motor connected on Arduino UNO board //Enable1 Input1 Input2
int motor_right[] = {11,7, 6};  // Initialize the pins for Right Motor connected on Arduino UNO board - //Enable2 Input3 Input4

// --------------------------------------------------------------------------- Setup Module
void setup() {
    Serial.begin(9600);            // Sets the data rate in bits per second (baud) for serial data transmission.
    myServo.attach(pinServo);      // Pin 5 for the Servo motor
    // Setup motors
    // Loop to setup the pinModes
    for(int i = 0; i < 3; i++)
    {        
        pinMode(motor_left[i], OUTPUT); // Initialize the pins as output for the left motor
        pinMode(motor_right[i], OUTPUT); // Initialize the pins as output for the right motor
    }
    // position servo to center
    servoAngle(SERVOCENTER);
}

// --------------------------------------------------------------------------- Loop Module
void loop() { 
    float forwardDistance = ping();
    drive_forward();
    if (forwardDistance > Thresholddistance)
    {
        drive_forward();
    }
    else
    {
        brake();
        servoAngle(SERVOLEFT);         // check for obstacles to the left
        float leftDistance=ping(); 
        servoAngle(SERVORIGHT);      // check for obstacles to the right
        float rightDistance=ping();  // find the distance to the right
        servoAngle(SERVOCENTER); 
        if (leftDistance > rightDistance && leftDistance > Thresholddistance){
            turn_left();
        }
        else if(rightDistance > leftDistance && rightDistance > Thresholddistance){
            turn_right();
        } 
        else {
            turn_around();
        }
        delay(1000);
    }
}

// --------------------------------------------------------------------------- Drive
// Following Module is for the Robot to stop for 6 seconds
void motor_stop(){
    int i;
    for(i = 0; i < 3; i++)
    {
        digitalWrite(motor_left[i], LOW);
        digitalWrite(motor_right[i], LOW);
        delay(6000);
    }
}
// Following Module is for the Robot to slow down
void brake()
{
    digitalWrite(motor_left[0], HIGH);   
    digitalWrite(motor_left[1], LOW); 
    digitalWrite(motor_left[2], LOW); 
    digitalWrite(motor_right[0], HIGH);
    digitalWrite(motor_right[1], LOW);
    digitalWrite(motor_right[2], LOW);
    delay(25);
}
// Following Module is for the Robot to go forward
void drive_forward()
{
    digitalWrite(motor_left[0], HIGH);   
    digitalWrite(motor_left[1], HIGH);
    digitalWrite(motor_left[2], LOW); 
    digitalWrite(motor_right[0], HIGH); 
    digitalWrite(motor_right[1], LOW);
    digitalWrite(motor_right[2], HIGH); 
}
// Following Module is for the Robot to go backwards
void drive_backward()
{
    digitalWrite(motor_left[0], HIGH); 
    digitalWrite(motor_left[1], LOW);
    digitalWrite(motor_left[2], HIGH); 
    digitalWrite(motor_right[0], HIGH); 
    digitalWrite(motor_right[1], HIGH); 
    digitalWrite(motor_right[2], LOW);
}
// Following Module is for the Robot to turn left
void turn_left()
{
    digitalWrite(motor_left[0], HIGH); 
    digitalWrite(motor_left[1], HIGH); 
    digitalWrite(motor_left[2], LOW); 
    digitalWrite(motor_right[0], HIGH);
    digitalWrite(motor_right[1], HIGH);
    digitalWrite(motor_right[2], LOW);
    delay(300);
}
// Following Module is for the Robot to turn right
void turn_right()
{
    digitalWrite(motor_left[0], HIGH); 
    digitalWrite(motor_left[1], LOW); 
    digitalWrite(motor_left[2], HIGH);
    digitalWrite(motor_right[0], HIGH);
    digitalWrite(motor_right[1], LOW);
    digitalWrite(motor_right[2], HIGH);
    delay(300);
}
// Following Module is to make a uturn if there are obstacles to the right, to the left and in front
void turn_around()
{
    digitalWrite(motor_left[0], HIGH); 
    digitalWrite(motor_left[1], LOW); 
    digitalWrite(motor_left[2], HIGH);
    digitalWrite(motor_right[0], HIGH);
    digitalWrite(motor_right[1], LOW);
    digitalWrite(motor_right[2], HIGH);
    delay(600);
}

// Module for the Servo
void servoAngle(int angleNew)
{
    // change direction when limits are reached
    if (angleNew > angleCur)
    {
        for (int angle = angleCur; angle<angleNew;angle +=1)
        {
            myServo.write(angle);
            delay(20);
        }
        angleCur=angleNew;
    }
    else if (angleNew < angleCur)
    {
        for (int angle = angleCur; angle<angleNew;angle -=1)
        {
            myServo.write(angle);
            delay(20);
        }
        angleCur=angleNew;
    }
}

// Following Module is to test for ping using ultrasonic range finder
float ping()
{
  delay(50);
  unsigned int uS = sonar.ping();
  if (uS == 0)
 {
   return MAXIMUM_DISTANCE;
 }
  else
    return (uS /US_ROUNDTRIP_CM);
}

