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




void motor(int number, float phase){                          
  int k = (-SERVOMIN + SERVOMAX)/2; 
  float a = phase * 3.14 / 24;                                   // Change this section according to your uses, I prefer to divide phase to 24
  length = (SERVOMIN + SERVOMAX)/2 + k*sin((i*3.14/400 + a));    //Some transformation again for pulse length count
  pwm.setPWM(number, 0, length);                                 //This function comes from library
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

  for (uint16_t i = 0; i < 400; i++) {
    motor(0,0);
    motor(1,1);
    motor(2,2);
    motor(3,3);
    motor(4,4);
    motor(5,5);
    motor(6,6);                                    // Left number repsresent which motor will work.
    motor(8,8);                                    // Right number indicates phase haw many n*pi/400.
    motor(10,10);
    motor(11,11);
    motor(13,13);
    motor(14,14);
    }
    

  /*for (uint16_t i = 400; i > 0; i--) {
    motor(0,0);
    motor(1,1);
    motor(2,2);
    motor(3,3);
    motor(4,4);
    motor(5,5);                                   // This part is for reverse direction.
    motor(6,6);
    motor(8,8);
    motor(10,10);
    motor(11,11);
    motor(13,13);
    motor(14,14);
    }
  */

      
  
  //pwm.setPWM(0, 0, n(3.14*0/12));              // This code comes from the library.
}
