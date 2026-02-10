const int switchPin = 14;   // Safe GPIO on XIAO ESP32-S3

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(switchPin, INPUT_PULLUP);

  Serial.println("Switch State Test Started");
}

void loop() {
  int switchState = digitalRead(switchPin);

  if (switchState == LOW) {
    Serial.println("Switch PRESSED");
  } else {
    Serial.println("Switch RELEASED");
  }

  delay(200);  // debounce delay
}
