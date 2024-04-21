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



int dc_OffsetVec[12] = {1512,1535,1532,1495,1490,1530,1520,1522,1527,1525,1555,1547}; //This array is representing our initial position, adjust the value according to your motor position.
int i = 0;
int amplitude = 0;
int amplitude_values = 500;




void motor(int number, float phase){                          
  float a = (phase * pi) / 24;                                   // Change this section according to your uses, I prefer to divide phase to 24
  int dc_Offset = dc_OffsetVec[number];
  float length = dc_Offset + amplitude*sin((millis()*pi/1000 + a));    //Some transformation again for pulse length count
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

  while (amplitude< amplitude_values ) {

    motor(0,0);
    motor(1,1);
    motor(2,2);
    motor(3,3);
    motor(4,4);
    motor(5,5);
    motor(6,6);
    motor(7,7);
    motor(8,8);
    motor(9,9);
    motor(10,10); 
    motor(11,11);
    if(millis() % 200 < 0.5) {
      amplitude += 100;
    }
    delay(10);
    
  }
}


void loop() {
  
  char rc;
  if (Serial.available()) {
    rc = Serial.read();
    Serial.println(rc);

    if (rc == 's') {
    while (0 < amplitude ) {

      motor(0,0);
      motor(1,1);
      motor(2,2);
      motor(3,3);
      motor(4,4);
      motor(5,5);
      motor(6,6);
      motor(7,7);
      motor(8,8);
      motor(9,9);
      motor(10,10); 
      motor(11,11);
    if(millis() % 200 < 0.5) {
      amplitude -= 100;
    }
    delay(10);
    
  }
    }

    if (rc >= '0' && rc <= '9') {
      Serial.print("Amplitude is set to ");
      Serial.println(rc);
      amplitude = rc - '0';
    }
  }

  motor(0, 0);
  motor(1, 1);
  motor(2, 2);
  motor(3, 3);
  motor(4, 4);
  motor(5, 5);
  motor(6, 6);
  motor(7, 7);
  motor(8, 8);
  motor(9, 9);
  motor(10, 10);
  motor(11, 11);

  i++;



    
}
