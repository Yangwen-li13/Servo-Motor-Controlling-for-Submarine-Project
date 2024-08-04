#include <Wire.h>
#include "Adafruit_PWMServoDriver.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  1000
#define SERVOMAX  2000
#define USMIN  1000
#define USMAX  2000
#define SERVO_FREQ 240
#define pi 3.1415926535897932384626433832795
#define NUM_MOTORS 12

int dc_OffsetVec[NUM_MOTORS] = {1532,1580,1527,1482,1494,1450,1530,1530,1500,1530,1535,1547};
int amplitude = 0;
int amplitude_final = 0;
int amplitude_value = 400;
float frequency_value = 0.5;
float phase_value;
float a = 0;                           //Exponential-envelope constant

void setup() {
  Serial.begin(9600);
  Serial.println("Process is starting");
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
  Serial.println("Initialization is completed");
  stop_motor();
  delay(10);
}

void loop() {
  if (Serial.available()) {
    String rc = Serial.readStringUntil('\n');
    if (rc == "test") {
      Serial.println("Test is starting"); 
      int amp = 0;
      int per = 0;
      float freq = 0;
      float exp_a = 0;
      int starting_amplitude = 0;
      Serial.println("Enter the values such that 'amp(amplitude)_value_per(percentage)_value_freq(frequency)_value_a(exponential envelope constant)_value'");
      Serial.println("For example: 'amp_400_per_25_freq_0.4_a_0_startAmp_200'");
      String message = "";
      while (message == "") {
        message = Serial.readStringUntil('\n');
      }
      Serial.println("your message is " + message);
      int firstUnderscore = message.indexOf('_');
      int secondUnderscore = message.indexOf('_', firstUnderscore + 1);
      int thirdUnderscore = message.indexOf('_', secondUnderscore + 1);
      int fourthUnderscore = message.indexOf('_', thirdUnderscore + 1);
      int fifthUnderscore = message.indexOf('_', fourthUnderscore + 1);
      int sixthUnderscore = message.indexOf('_', fifthUnderscore + 1);
      int seventhUnderscore = message.indexOf('_', sixthUnderscore + 1);
      int eightUnderscore = message.indexOf('_', seventhUnderscore + 1);
      int ninethUnderscore = message.indexOf('_', eightUnderscore + 1);
      amp = message.substring(firstUnderscore + 1, secondUnderscore).toInt();
      per = message.substring(thirdUnderscore + 1, fourthUnderscore).toInt();
      freq = message.substring(fifthUnderscore + 1, sixthUnderscore).toFloat();
      exp_a = message.substring(seventhUnderscore + 1, eightUnderscore).toFloat();
      starting_amplitude = message.substring(ninethUnderscore + 1).toInt();
      Serial.print("Amplitude: ");
      Serial.print(amp);
      Serial.print(", Period: ");
      Serial.print(per);
      Serial.print(", Frequency: ");
      Serial.print(freq);
      Serial.print(", Envelope constant: ");
      Serial.print(exp_a);
      Serial.print(", Initial Amplitude: ");
      Serial.println(starting_amplitude);
      delay(10);
      changeParameters(amp, per, freq, exp_a, starting_amplitude);
    }
  }
}

void motor(int number, float phase) {
  phase_value = ((13 - phase) * pi) / 24;
  int dc_Offset = dc_OffsetVec[number];
  int L = number + 1;
  float exponential_constant = exp(a * (13-L));
  float length = dc_Offset + amplitude * exponential_constant * sin((millis() * (2 * pi * frequency_value) / 1000 + phase_value));
  pwm.setPWM(number, 0, length);
}

void motorCont() {
  for (int i = 0; i < NUM_MOTORS; i++) {
    motor(i, i);
  }
}

void motor_set(int number) {
  pwm.setPWM(number, 0, dc_OffsetVec[number]);
}

void start_motor(int target_amplitude) {
  int steps = 1000;
  for (int i = 0; i <= steps; i++) {
    for (int j = 0; j < NUM_MOTORS; j++) {
      motor(j, j);
    }
    amplitude = target_amplitude * sin(i * pi / (2 * steps));
    delay(10);
  }
  amplitude = target_amplitude;
  amplitude_final = amplitude;
  delay(10);
}

void stop_motor() {
  int steps = 500;
  for (int i = steps; i >= 0; i--) {
    for (int j = 0; j < NUM_MOTORS; j++) {
      motor(j, j);
    }
    amplitude = amplitude_final * sin(i * pi / (2 * steps));
    delay(10);
  }
  amplitude = 0;
  amplitude_final = 0;
  delay(10);
}

void changeParameters(int max_amplitude, int percentage, float new_frequency, float new_a, int startingAmplitude) {
  float constantA_copy = max_amplitude * (percentage / 100.0);
  int constantA = max_amplitude * (percentage / 100.0);
  int amplitude_a = startingAmplitude;
  int durationStop = 5000;
  int durationWorking = 30000;
  int testPeriod = 0;
  unsigned long timeVector[4];

  a = new_a;
  frequency_value = new_frequency;

  Serial.println("Test began");
  while (amplitude_a < max_amplitude && amplitude_a >= 0) {
    if (startingAmplitude + constantA > 400) {
      Serial.println("Starting amplitude is much big!");
      break;
    }
    if (constantA != constantA_copy) {
      Serial.println("Percentage cannot divide amplitude as integer!");
      break;
    }
    if (0 > max_amplitude || 400 < max_amplitude) {
      Serial.println("Amplitude is wrong! Check maximum(400) and minimum(0) values!");
      break;
    }
    if (new_frequency > 0.6 || new_frequency < 0.1) {
      Serial.println("Frequency is wrong! Check maximum(0.6) and minimum(0.1) values!");
      break;
    }
    if (new_a < -0.2 || new_a > 0) {
      Serial.println("Exponential constant is wrong! Check maximum(0) and minimum(-0.2) values!");
      break;
    }

    amplitude_a += constantA;
    testPeriod += 1;
    Serial.println("Test Period is:");
    Serial.println(testPeriod);
    Serial.println(amplitude_a);
    timeVector[0] = millis();
    start_motor(amplitude_a);
    timeVector[1] = millis();
    unsigned long startTime = millis();
    while ((millis() - startTime) < durationWorking) {
      motorCont();
    }
    timeVector[2] = millis();
    stop_motor();
    timeVector[3] = millis();
    delay(durationStop);

    for (int i = 0; i < 4; i++) {
      Serial.print("timeVector[");
      Serial.print(i);
      Serial.println("]: ");
      Serial.println(timeVector[i]);
    }
  }
  stop_motor();
  Serial.println("Test has been done! Write new command");
  amplitude_final = amplitude_a;
}
