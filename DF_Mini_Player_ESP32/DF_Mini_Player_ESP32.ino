#include "DFRobotDFPlayerMini.h"

// Use UART2 on ESP32
HardwareSerial dfSerial(2);
DFRobotDFPlayerMini player;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // RX, TX pins (ESP32)
  dfSerial.begin(9600, SERIAL_8N1, 18, 19);

  Serial.println("Initializing DFPlayer...");

  if (!player.begin(dfSerial)) {
    Serial.println("❌ DFPlayer not detected");
    Serial.println("Check wiring, SD card, power");
    while (true);
  }

  Serial.println("✅ DFPlayer ready");

  player.volume(40);     // 0–30
  player.play(4);        // plays 0001.mp3
}

void loop() {
}
