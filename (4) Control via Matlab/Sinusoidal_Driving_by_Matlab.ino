#include <Wire.h>
#include "Adafruit_PWMServoDriver.h"
#include <math.h>

// Create an instance of the Adafruit_PWMServoDriver with the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Constants
#define SERVOMIN  1000 // Minimum pulse length count (out of 4096)
#define SERVOMAX  2000 // Maximum pulse length count (out of 4096)
#define USMIN  1000    // Minimum microsecond pulse length
#define USMAX  2000    // Maximum microsecond pulse length
#define SERVO_FREQ 240 // Frequency of the servo
#define PI 3.1415926535897932384626433832795
#define NUM_MOTORS 12  // Number of motors

// Initial positions of the motors
int dc_OffsetVec[NUM_MOTORS] = {1532, 1580, 1527, 1482, 1494, 1450, 1530, 1530, 1500, 1530, 1535, 1547};

int amplitude = 0;
int amplitude_final = 400;
int amplitude_value = 400; // Maximum amplitude value

float frequency_value = 0.5; // Fixed frequency value (range 0.1 to 0.6)
float phase_value;

void setup() {
  Serial.begin(9600);
  Serial.println("Process is starting");
  
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
  Serial.println("Initialization is completed");
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
      stop_motor();

      Serial.println("Enter the values in the format 'amp_400_per_25_freq_0.4'");
      String message = "";
      while (message == "") {
        message = Serial.readStringUntil('\n');
      }

      Serial.println("Your message: " + message);
      int firstUnderscore = message.indexOf('_');
      int secondUnderscore = message.indexOf('_', firstUnderscore + 1);
      int thirdUnderscore = message.indexOf('_', secondUnderscore + 1);
      int fourthUnderscore = message.indexOf('_', thirdUnderscore + 1);
      int fifthUnderscore = message.indexOf('_', fourthUnderscore + 1);

      amp = message.substring(firstUnderscore + 1, secondUnderscore).toInt();
      per = message.substring(thirdUnderscore + 1, fourthUnderscore).toInt();
      freq = message.substring(fifthUnderscore + 1).toFloat();

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
}

// Functions

void motor(int number, float phase) {
  phase_value = (phase * PI) / 24;
  int dc_Offset = dc_OffsetVec[number];

  float a = (-2 * log(2) / 11);
  float k = 1 / exp(a);
  float exponential_constant = k * exp(a * number);

  float length = dc_Offset + amplitude * sin((millis() * (2 * PI * frequency_value) / 1000 + phase_value));
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
    amplitude = target_amplitude * sin(i * PI / (2 * steps));
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
    amplitude = amplitude_final * sin(i * PI / (2 * steps));
    delay(10);
  }
  amplitude = 0;
  amplitude_final = 0;
  delay(10);
}

void changeParameters(int max_amplitude, int percentage, float new_frequency) {
  float constantA_copy = max_amplitude * (percentage / 100.0);
  int constantA = max_amplitude * (percentage / 100.0);
  int amplitude_a = 0;
  int durationStop = 5000;
  int durationWorking = 10000;

  Serial.println("Test began");
  while (amplitude_a < max_amplitude) {
    if (constantA != constantA_copy) {
      Serial.println("Percentage cannot divide amplitude as integer!");
      break;
    }
    if (max_amplitude < 100 || max_amplitude > 400) {
      Serial.println("Amplitude is wrong! Check maximum (400) and minimum (100) values!");
      break;
    }
    if (new_frequency < 0.1 || new_frequency > 0.6) {
      Serial.println("Frequency is wrong! Check maximum (0.6) and minimum (0.1) values!");
      break;
    }

    stop_motor();
    delay(durationStop);
    frequency_value = new_frequency;
    amplitude_a += constantA;

    Serial.print("Max Amplitude: ");
    Serial.println(amplitude_a);
    start_motor(amplitude_a);

    unsigned long startTime = millis();
    while ((millis() - startTime) < durationWorking) {
      motorCont();
    }
  }
  stop_motor();
  Serial.println("Test has been done! Write new command");
  amplitude_final = amplitude_a;
}
