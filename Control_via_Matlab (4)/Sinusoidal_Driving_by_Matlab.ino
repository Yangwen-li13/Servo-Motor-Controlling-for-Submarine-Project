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


int dc_OffsetVec[NUM_MOTORS] = {1532,1580,1527,1482,1494,1450,1530,1530,1500,1530,1535,1547}; //This array is representing our initial position, adjust the value according to your motor position.

int amplitude = 0;
float phase = 0;
float frequency = 0;
int amplitude_final = 400;


int amplitude_value = 400;                        //This is approximately maximum value
float frequency_value = 0.5;                      //This is fixed frequency value, the maximum can be 0.6 and the minimum can be 0.1 
float phase_value;

 

void motor(int number, float phase){                          
  phase_value = (phase * pi) / 24;                                                                             // Change this section according to your uses, I prefer to divide phase to 24
  int dc_Offset = dc_OffsetVec[number];
  float length = dc_Offset + amplitude*sin((millis() *(2 * pi * frequency_value) / 1000 + phase_value));    //Some transformation again for pulse length count
  pwm.setPWM(number, 0, length);                                                                               //This function comes from library
}                                                                         //This function is essential to give PWM to motors

void motorCont() {
  for (int i = 0; i < NUM_MOTORS; i++) {
    motor(i, i);
  }
}                                                                         //This function provides to turn motors with constant phase difference in the loop                                                                                     
                                                                      
void motor_set(int number){
  pwm.setPWM(number, 0, dc_OffsetVec[number]);
}                                                                         //This function is to turn motors initial positions

void stop_motor() {
  int steps = 500;
  for (int i = steps; i >= 0; i--) {
    for (int j = 0; j < NUM_MOTORS; j++) {
      motor(j, j);
    }
    amplitude = amplitude_final * sin(i * pi / (2 * steps)); // Sine-based ramp down
    delay(10);
  }
  amplitude = 0;
  amplitude_final = 0;
  delay(10);
}                                                                         //This function is to stop motors

void start_motor(int target_amplitude) {
  int steps = 1000;
  for (int i = 0; i <= steps; i++) {
    for (int j = 0; j < NUM_MOTORS; j++) {
      motor(j, j);
    }
    amplitude = target_amplitude * sin(i * pi / (2 * steps)); // Sine-based ramp up
    delay(10);
  }
  amplitude = target_amplitude;
  amplitude_final = amplitude;
  delay(10);
}                                                                         //This function is to start motors.             

void changeParameters(int max_amplitude, int percentage, float new_frequency){         
  float constantA_copy = max_amplitude * (percentage / 100.0);
  int constantA = max_amplitude * (percentage / 100.0);
  int amplitude_a = 0;
  int durationStop = 5000;
  int durationWorking = 10000;


  while(amplitude_a < max_amplitude){
    
    if(constantA != constantA_copy){
      Serial.println("Percentage cannot divide amplitude as integer!");
      break;
    }
    
    if(0 > max_amplitude or 400 < max_amplitude){
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
    amplitude_a += constantA;

    Serial.println("Amplitude is :");
    Serial.println(amplitude_a);
    start_motor(amplitude_a);
    
    unsigned long startTime = millis();
    while ( (millis() - startTime) < durationWorking) {
      motorCont();
    }
  }
  Serial.println("Test has been done!");
  amplitude_final = amplitude_a;
}                                                                           




void setup() {
  Serial.begin(9600);
  Serial.println("Servos started");
  
  pwm.begin();                                               
  pwm.setOscillatorFrequency(27000000);                  
  pwm.setPWMFreq(SERVO_FREQ);                                
  delay(10);

  start_motor(amplitude_value);
  Serial.println("Started!");
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
      start_motor(amplitude_final);
    }
    
    if (rc == 't') {
      Serial.println("Changing is Started");
      int amp = 0;
      int per = 0;
      float freq = 0;
      stop_motor();

      Serial.println("Enter the values such that 'amp(amplitude)_value_per(percentage)_value_freq(frequency)_value'");
      Serial.println("For example: 'amp_400_per_25_freq_0.4' ");
      
      String message = "" ;
      
      while(message == ""){
        message = Serial.readStringUntil('\n');
      }

      Serial.println("your message is " + message);
      int firstUnderscore = message.indexOf('_');
      int secondUnderscore = message.indexOf('_', firstUnderscore + 1);
      int thirdUnderscore = message.indexOf('_', secondUnderscore + 1);
      int fourthUnderscore = message.indexOf('_', thirdUnderscore + 1);
      int fifthUnderscore = message.indexOf('_', fourthUnderscore + 1);

      // Extract the values
      amp = message.substring(firstUnderscore + 1, secondUnderscore).toInt();
      per = message.substring(thirdUnderscore + 1, fourthUnderscore).toInt();
      freq = message.substring(fifthUnderscore + 1).toFloat();
    
      // Print the values
      Serial.print("Amplitude: ");
      Serial.print(amp);
      Serial.print(", Period: ");
      Serial.print(per);
      Serial.print(", Frequency: ");
      Serial.println(freq);
      
      delay(10);

      changeParameters(amp, per, freq);
      
  
    }
  }
  
  motorCont();  
}