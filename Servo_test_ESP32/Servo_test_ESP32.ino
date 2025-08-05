#include <ESP32Servo.h>

Servo myServo;             // Create servo object
int servoPin = 13;         // GPIO13 (you can change)

void setup() {
  Serial.begin(115200);
  myServo.setPeriodHertz(50); // Typical for servos
  myServo.attach(servoPin, 500, 2400); // PWM pulse width (µs) range
  Serial.println("Servo ready");
}

void loop() {
  for (int angle = 0; angle <= 180; angle += 10) {
    myServo.write(angle);
    Serial.print("Angle: ");
    Serial.println(angle);
    delay(500);
  }

  for (int angle = 180; angle >= 0; angle -= 10) {
    myServo.write(angle);
    Serial.print("Angle: ");
    Serial.println(angle);
    delay(500);
  }
}
