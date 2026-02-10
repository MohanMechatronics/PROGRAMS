#include <Arduino.h>

#define A9G_PON   13
#define A9G_LOWP  12

String SOS_NUM = "+917358289559";

void Send_SMS(String message);

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 16, 17);

  pinMode(A9G_PON, OUTPUT);
  pinMode(A9G_LOWP, OUTPUT);

  // Power ON A9G
  digitalWrite(A9G_LOWP, HIGH);
  digitalWrite(A9G_PON, HIGH);
  delay(1000);
  digitalWrite(A9G_PON, LOW);
  delay(15000);

  // SMS text mode
  Serial1.println("AT+CMGF=1");
  delay(1000);
}

void loop() {
  Send_SMS("hello");
  delay(5000);   // send SMS every 
}

void Send_SMS(String message) {
  Serial1.println("AT+CMGS=\"" + SOS_NUM + "\"");
  delay(1000);
  Serial1.println(message);
  delay(500);
  Serial1.write(26);   // CTRL+Z
  delay(3000);
}
