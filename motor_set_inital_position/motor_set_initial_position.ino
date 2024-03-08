#include <Wire.h>
#include "Adafruit_PWMServoDriver.h"

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
/* 
you can also call it with a different address you want
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
you can also call it with a different address and I2C interface
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);
*/

//https://www.savoxusa.com/products/savsw0231mgp-waterproof-std-digital#technical-details

//Some transformation to get length values, you can think the period as 1/frequency and proportion to length count
//Change values according to your uses
#define SERVOMIN  1000 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  2000 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  1000 // This is the rounded 'minimum' microsecond 
#define USMAX  2000 // This is the rounded 'maximum' microsecond 
#define SERVO_FREQ 240 // Frequency of servo
#define pi 3.1415926535897932384626433832795




int dc_OffsetVec[12] = {1240,1360,1500,1500,1500,1500,1500,1500,1500,1500,1500,1500}; //This array is representing our initial position, adjust the value according to your motor position.
int i = 0;
int amplitude = 500;



void motor(int number, float phase){                          
  float a = (phase * pi) / 24;                                   // Change this section according to your uses, I prefer to divide phase to 24
  int dc_Offset = dc_OffsetVec[number];
  float length = dc_Offset + amplitude*sin(((i*pi)/100 + a));    //Some transformation again for pulse length count
  pwm.setPWM(number, 0, length);                                 //This function comes from library
}

void motor_set(int number){
  pwm.setPWM(number, 0, dc_OffsetVec[number]);

}



void setup() {
  Serial.begin(9600);
  Serial.println("Servos started");
  
  pwm.begin();                                               
  pwm.setOscillatorFrequency(27000000);                  
  pwm.setPWMFreq(SERVO_FREQ);                                
  delay(10);
}


void loop() {

    motor_set(0);
    motor_set(1);
    motor_set(2);
    motor_set(3);
    motor_set(4);
    motor_set(5);
    motor_set(6);
    motor_set(7);
    motor_set(8);
    motor_set(9);
    motor_set(10);
    motor_set(11);
  

    
    
}
