// ===== ULTRASONIC SENSOR (HC-SR04) =====
#define TRIG_PIN 25
#define ECHO_PIN 26

long duration;
int distance;

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println("ESP32 Ultrasonic Sensor Test");
}

void loop() {

  // Trigger ultrasonic pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo time
  duration = pulseIn(ECHO_PIN, HIGH, 25000); // 25ms timeout

  // Calculate distance
  if (duration == 0) {
    Serial.println("Out of range");
  } else {
    distance = duration * 0.034 / 2;
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }

  delay(500);
}
