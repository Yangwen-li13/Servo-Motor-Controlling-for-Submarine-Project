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
#define NUM_MOTORS 12
#define UPDATE_INTERVAL 200


int dc_OffsetVec[NUM_MOTORS] = {1532,1580,1527,1482,1494,1450,1530,1530,1500,1530,1535,1547}; //This array is representing our initial position, adjust the value according to your motor position.

int amplitude0 = 0;
float phase0 = 0;
float frequency0 = 0;


int amplitude_value = 400;                        //This is approximately maximum value
float frequency_value = 0.5;                      //This is fixed frequency value, the maximum can be 0.6 and the minimum can be 0.1 
float phase_value;
int test_amplitude;
int test_phase;
int test_frequency;

 
void motor(int number, float phase){                          
  phase_value = (phase * pi) / 24;                                                                             // Change this section according to your uses, I prefer to divide phase to 24
  int dc_Offset = dc_OffsetVec[number];
  float length = dc_Offset + amplitude0*sin((millis() *(2 * pi * frequency_value) / 1000 + phase_value));    //Some transformation again for pulse length count
  pwm.setPWM(number, 0, length);                                                                               //This function comes from library
}     //This function is essential to give PWM to motors


void motorCont() {
  for (int i = 0; i < NUM_MOTORS; i++) {
    motor(i, i);
  }
}                                                                          //This function provides to turn motors with constant phase difference in the loop                                                                                     

                                                                      
void motor_set(int number){
  pwm.setPWM(number, 0, dc_OffsetVec[number]);
}                                                                           //This function is to turn motors initial positions

void stop_motor() {
    while (0 < amplitude0) {
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
    if(millis() % 200 < 1.2) {
      amplitude0 -= 50;
    if (0 > amplitude0){
        amplitude0 = 0;
        
    }
  }
  delay(10);
  }
}                                                                            //This function is to stop motors


void start_motor(int target_amplitude) {
  while (amplitude0 < target_amplitude) {
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
    if(millis() % 200 < 1.5) {
      amplitude0 += 25;
    if(amplitude0 > target_amplitude){
      amplitude0 = target_amplitude;
    }
  }
  delay(10);
  }
}                                                                            //This function is to start motors.             


void changeParameters(int max_amplitude0, int percentage, float new_frequency){          //Don't forget to stop motors before using this function!!!!
  float constantA_copy = max_amplitude0 * (percentage / 100.0);
  int constantA = max_amplitude0 * (percentage / 100.0);
  int amplitude0_a = 0;
  int durationStop = 5000;
  int durationWorking = 10000;


  while(amplitude0_a < max_amplitude0){
    
    if(constantA != constantA_copy){
      Serial.println("Percentage cannot divide amplitude as integer!");
      break;
    }
    
    if(0 > max_amplitude0 or 400 < max_amplitude0){
      Serial.println("Amplitude is wrong! Check maximum(400) and minimum(0) values!");
      break;
    }
    if(new_frequency > 0.6 or new_frequency < 0.1){
      Serial.println("Frequency is wrong! Check maximum(0.6) and minimum(0.1) values!");
      break;
    }

    stop_motor();
    delay(durationStop);
    frequency_value = new_frequency;
    amplitude0_a = amplitude0_a + constantA;
    //amplitude0 = amplitude0_a;
    Serial.println("Amplitude is :");
    Serial.print(amplitude0_a);
    start_motor(amplitude0_a);
    
    float Time1 = millis();
    float Time2 = millis();
    while ( (Time2 - Time1) < durationWorking) {
      motorCont();
      Time2 = millis();
    }
  }
  Serial.println("Test has been done!");
}                                                                           




void setup() {
  Serial.begin(9600);
  Serial.println("Servos started");
  
  pwm.begin();                                               
  pwm.setOscillatorFrequency(27000000);                  
  pwm.setPWMFreq(SERVO_FREQ);                                
  delay(10);

  start_motor(amplitude_value);
  Serial.print("Started!");
}


void loop() {
  
  char rc;
  if (Serial.available()) {
    rc = Serial.read();
    Serial.println(rc);

    if (rc == 's') {
      stop_motor();
    }                           //Stops motor
    
    if(rc == 'c'){
      start_motor(amplitude0_a);
    }
    
    if (rc == 'a') {
      Serial.println("Changing is Started");
      stop_motor();
      amplitude0 = 0; 
      frequency0 = 0;
      int percentage = 0;
      Serial.println("Enter the amplitude value");
      while(amplitude0 == 0){
        amplitude0 = Serial.parseInt();
      }
      delay(10);
      Serial.println("Enter the percentage of amplitude value");
      while(percentage == 0){
        percentage = Serial.parseInt();
      }
      delay(10);
      Serial.println("Enter the frequency value");
      while(frequency0 == 0){
        frequency0 = Serial.parseFloat();
      }
      delay(10);

      changeParameters(amplitude0, percentage, frequency0);
      
  
    }
  }
  motorCont();
  
}
