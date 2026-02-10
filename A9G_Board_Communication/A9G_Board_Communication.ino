#include <Arduino.h>

HardwareSerial A9G(1);   // Use UART1

void setup() {
  Serial.begin(115200);      // USB Serial Monitor
  delay(1000);

  // UART1: RX=16, TX=17
  A9G.begin(115200, SERIAL_8N1, 16, 17);

  Serial.println("A9G AT Console Ready");
  Serial.println("Type AT commands below:");
}

void loop() {
  // PC → A9G
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    A9G.print(cmd);
    A9G.print("\r\n");
  }

  // A9G → PC
  while (A9G.available()) {
    Serial.write(A9G.read());
  }
}
