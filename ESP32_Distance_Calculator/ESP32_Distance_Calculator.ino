#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Ultrasonic Pins
#define TRIG_PIN 26
#define ECHO_PIN 27

void setup() {
  Wire.begin(13, 14);  // SDA, SCL
  lcd.init();
  lcd.backlight();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  lcd.setCursor(0, 0);
  lcd.print("FIND DISTANCE");
  delay(2000);
  lcd.clear();
}

float measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout

  if (duration == 0) {
    return -1; // Out of range
  }

  float distance = duration * 0.0343 / 2;  // cm
  return distance;
}

void loop() {
  float distance = measureDistance();

  lcd.setCursor(0, 0);
  lcd.print("Distance:       ");

  lcd.setCursor(0, 1);


  if (distance < 0) {
    lcd.print("Out of range    ");
  } else {
    lcd.print(distance, 2);
    lcd.print(" cm      ");
  }

  delay(500);
}
