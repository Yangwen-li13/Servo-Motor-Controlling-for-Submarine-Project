String message;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    message = Serial.readStringUntil('\n'); // Correct terminator
    Serial.println(message);
    if (message == "s") {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("High");
    } else if (message == "d") {
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("Low");
    }
  }
}
