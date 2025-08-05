#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mySerial(4, 5); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

const int ldrPin = A7;       // LDR analog pin
const int threshold = 700;   // Adjust based on lighting conditions
bool greeted = false;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("DFPlayer not found");
    while (true);
  }
  myDFPlayer.volume(25);  // Volume: 0–30
  Serial.println("Welcome greeter ready");
}

void loop() {
  int ldrValue = analogRead(ldrPin);
  Serial.println(ldrValue);

  if (ldrValue < threshold && !greeted) {
    myDFPlayer.play(1);   // Plays 0001.mp3
    greeted = true;
    delay(3000);          // Prevent re-triggering
  }

  if (ldrValue > threshold) {
    greeted = false;      // Reset greeter once laser is restored
  }

  delay(100);
}