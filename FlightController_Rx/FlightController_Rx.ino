#include <WiFi.h>
#include <esp_now.h>
#include <ESP32Servo.h>

// Motor and servo pins
const int motorPin[4] = {1, 3, 4, 48};     // Change if UART conflicts
const int servoPin[2] = {8, 5};            // elevator, rudder

// Servo objects
Servo servos[2];

// Incoming data structure
typedef struct {
  uint16_t elevator;
  uint16_t rudder;
  uint16_t not_use;
  uint16_t throttle;
  uint16_t not_use1;
  uint16_t not_use2;
} struct_message;

struct_message incomingData;

// Map ADC value to servo pulse width
int mapRange(int x, int in_min = 0, int in_max = 4095, int out_min = 1000, int out_max = 2000) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// ESP-NOW receive callback
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingDataRaw, int len) {
  if (len != sizeof(incomingData)) return;

  memcpy(&incomingData, incomingDataRaw, sizeof(incomingData));

  // Move servos
  servos[0].writeMicroseconds(mapRange(incomingData.elevator));
  servos[1].writeMicroseconds(mapRange(incomingData.rudder));

  // Motor control
  if (incomingData.throttle > 2000) {
    int speed = map(incomingData.throttle, 2000, 4095, 0, 255);
    for (int i = 0; i < 4; i++) analogWrite(motorPin[i], speed);
  } else {
    for (int i = 0; i < 4; i++) analogWrite(motorPin[i], 0);
  }

  // Debug
  Serial.printf("Elevator=%u, Rudder=%u, Throttle=%u\n",
                incomingData.elevator, incomingData.rudder, incomingData.throttle);
}

void setup() {
  Serial.begin(115200);

  // Set motor pins
  for (int i = 0; i < 4; i++) pinMode(motorPin[i], OUTPUT);

  // Attach servos with RC limits
  for (int i = 0; i < 2; i++) {
    servos[i].setPeriodHertz(50);
    servos[i].attach(servoPin[i], 1000, 2000);
    servos[i].write(90); // Center
  }

  // WiFi in station mode
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ ESP-NOW init failed");
    while (true);
  }
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("✅ ESP-NOW receiver ready.");
}

void loop() {
  // Nothing here; handled by callback
}
