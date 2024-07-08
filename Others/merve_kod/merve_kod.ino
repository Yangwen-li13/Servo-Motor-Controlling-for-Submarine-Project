//Slave Arduino 2

#include <Wire.h>
#include <math.h>
#include <Servo.h>
//#include <SoftwareSerial.h> 
#include "I2C_Anything.hpp"
#define PI 3.1415926535897932384626433832795
#define motor_nr 3
#define s_address 10

Servo S6, S7, S8;

int origin = 90;
double phazeDiffInDegrees = 40; //kanatların hareketi sırasında aralarında kaç derece açı farkı olduğunu belirliyor
double phazeDiff = 2*PI*phazeDiffInDegrees/360; // derece cinsinden açı farkını radyana çeviriyor
int radius = 90; //kanatların kaç merkezden kaç birim saptığını belirliyor
int periodInMS = 5000 ; // bir periyodunu (aşağı yukarı tam hareketini) kaç msde tamamladığı
double piangle;

double mposition[motor_nr];

int count = 3;

void setup() {

  Wire.begin(s_address);
  Wire.onReceive(receiveEvent);
  //Serial.begin(9600);
  S6.attach(5);
  S7.attach(6);
  S8.attach(10);

}

volatile boolean haveData = false;

void loop() {
  
  if(haveData){

    for(int i =  (s_address-8) * motor_nr; i< ((s_address-8)+1) * motor_nr; i++){
      mposition[i-(s_address-8) * motor_nr] = sin(piangle+phazeDiff*i)*radius+origin;
    }

    S6.write(mposition[0]);
    delay(5);
    S7.write(mposition[1]);
    delay(5);
    S8.write(mposition[2]);
    delay(5);
  }

}

void receiveEvent (int howMany)
{
 if (howMany >= (sizeof piangle)){
   I2C_readAnything (piangle);
   haveData = true;
   }  // end if have enough data
}
