#include <Arduino.h>
#include <map>  // required for map used in analogWrite wrapper

// Define output pins
const int motor1A = 41;
const int motor1B = 42;
const int motor2A = 42;
const int motor2B = 25;

// Custom analogWrite() function for ESP32
void analogWrite(int pin, int value) {
  static int channel = 0;
  static std::map<int, int> pinChannelMap;

  if (pinChannelMap.find(pin) == pinChannelMap.end()) {
    ledcSetup(channel, 2000, 8);       // 2kHz, 8-bit resolution
    ledcAttachPin(pin, channel);
    pinChannelMap[pin] = channel;
    channel++;
  }

  int assignedChannel = pinChannelMap[pin];
  ledcWrite(assignedChannel, value);
}

void setup() {
  // Nothing needed, channels assigned dynamically
}

void loop() {
  // Move forward
  analogWrite(motor1A, 255);
  analogWrite(motor1B, 0);
  analogWrite(motor2A, 255);
  analogWrite(motor2B, 0);
  delay(2000);

  // Turn right
  analogWrite(motor1A, 150);
  analogWrite(motor1B, 0);
  analogWrite(motor2A, 0);
  analogWrite(motor2B, 150);
  delay(600);
}
