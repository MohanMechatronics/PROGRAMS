#include <Arduino.h>

// -------- A9G Pins --------
#define A9G_PON   13
#define A9G_LOWP  12

// -------- ESP32 ↔ A9G UART --------
// RX = GPIO16 , TX = GPIO17
// Most A9G boards default to 9600 baud

void setup() {
  // USB debug
  Serial.begin(115200);
  delay(1000);
  Serial.println("A9G Speaker Test Start");

  // A9G UART
  Serial1.begin(9600, SERIAL_8N1, 16, 17);
  delay(500);

  // A9G control pins
  pinMode(A9G_PON, OUTPUT);
  pinMode(A9G_LOWP, OUTPUT);

  // ---------- POWER ON A9G ----------
  digitalWrite(A9G_LOWP, HIGH);   // wake
  delay(100);

  digitalWrite(A9G_PON, HIGH);
  delay(600);
  digitalWrite(A9G_PON, LOW);

  Serial.println("Waiting for A9G boot...");
  delay(20000);                   // IMPORTANT

  // ---------- BASIC AT TEST ----------
  Serial1.println("AT");
  delay(1000);
  while (Serial1.available()) {
    Serial.write(Serial1.read());
  }

  // ---------- ROUTE AUDIO TO SPEAKER ----------
  Serial.println("\nSetting speaker output...");
  Serial1.println("AT+SNFS=2");   // speaker
  delay(1000);

  Serial1.println("AT+CLVL=15");   // max volume
  delay(500);

  Serial1.println("AT+CRSL=15");   // ring volume
  delay(500);

  Serial.println("Setup done. Speaker should ring.");
}

void loop() {
  Serial.println("RING ON");
  Serial1.println("AT+RING");   // make sound
  delay(3000);                  // sound for 3 sec

  Serial.println("RING OFF");
  Serial1.println("AT+CHUP");   // stop sound
  delay(7000);
}
