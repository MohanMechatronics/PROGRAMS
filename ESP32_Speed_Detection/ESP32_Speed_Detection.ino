#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// IR Sensor Pins
#define IR1 26
#define IR2 27

// Distance between sensors (in meters)
float sensorDistance = 0.20;  // 20 cm = 0.20 meters

unsigned long startTime = 0;
unsigned long endTime = 0;

bool objectDetected = false;

void setup() {
  Wire.begin(13, 14);
  lcd.init();
  lcd.backlight();

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  lcd.setCursor(0, 0);
  lcd.print("Speed Detector");
  delay(2000);
  lcd.clear();
}

void loop() {

  // Object crosses first sensor
  if (digitalRead(IR1) == LOW && !objectDetected) {
    startTime = millis();
    objectDetected = true;
  }

  // Object crosses second sensor
  if (digitalRead(IR2) == LOW && objectDetected) {
    endTime = millis();

    unsigned long timeTaken = endTime - startTime;  // in milliseconds

    float timeSeconds = timeTaken / 1000.0;
    float speed = sensorDistance / timeSeconds;  // m/s

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Speed:");

    lcd.setCursor(0, 1);
    lcd.print(speed, 2);
    lcd.print(" m/s");

    delay(3000);
    lcd.clear();

    objectDetected = false;
  }
}
