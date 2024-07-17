#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno1 = Adafruit_BNO055(1, 0x28);
Adafruit_BNO055 bno2 = Adafruit_BNO055(2, 0x29);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");

  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  digitalWrite(10, LOW);
  digitalWrite(11, HIGH);

 if (!bno1.begin()) {
   Serial.println("BNO055 #1 not detected... Check your wiring or I2C ADDR!");
    while (1);
 }
  Serial.println("BNO055 number 1");
  delay(1000);

  digitalWrite(10, HIGH);
  digitalWrite(11, LOW);

  if (!bno2.begin()) {
    Serial.println("BNO055 #2 not detected... Check your wiring or I2C ADDR!");
    while (1);
  }
  Serial.println("BNO055 number 2");
  delay(1000);
}

void loop() {
  // İlk sensörden veri oku
  digitalWrite(10, LOW);
  digitalWrite(11, HIGH);
  delay(8);
  sensors_event_t orientationData1;
  bno1.getEvent(&orientationData1);
  printEvent(1, &orientationData1);
  delay(8);

  // İkinci sensörden veri oku
  digitalWrite(10, HIGH);
  digitalWrite(11, LOW);
  delay(8);
  sensors_event_t orientationData2;
  bno2.getEvent(&orientationData2);
  printEvent(2, &orientationData2);
  delay(8);

  Serial.println(); // Satır sonu karakteri ekler
}

void printEvent(int sensorId, sensors_event_t* event) {


  Serial.print(sensorId);
  Serial.print(",");
  Serial.print(event->orientation.x);
  Serial.print(",");
  Serial.print(event->orientation.y);
  Serial.print(",");
  Serial.print(event->orientation.z);
  Serial.println(); // Satır sonu karakteri ekler
}
