
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

#define SERVOMIN  1000 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  1800 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  1000 // This is the rounded 'minimum' microsecond 
#define USMAX  2000 // This is the rounded 'maximum' microsecond 
#define SERVO_FREQ 240 // Frequency of servo


int k = (-SERVOMIN + SERVOMAX)/2; 
int i = 0;

int n(int phase){
  return (SERVOMIN + SERVOMAX)/2 + k*sin((i*3.14/400 + phase));
}

float picounter(float n) {
  return n * 3.14 / 12;
}

void motor(int number, float phase){
  float a = picounter(phase);
  int p = n(a);
  pwm.setPWM(number, 0, p);
}


void setup() {
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ); 
  delay(10);
}



void loop() {
  
  motor(0,0);
  motor(1,1);
  motor(2,2);
  motor(3,3);
  motor(5,4);
  motor(6,5);
    
  
  
  i++;

      
  
  //pwm.setPWM(0, 0, n(3.14*0/12));
}
