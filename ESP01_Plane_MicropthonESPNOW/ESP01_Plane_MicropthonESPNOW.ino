#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Servo.h>

// Define output pins
#define SERVO_PIN1 0   // GPIO0
#define SERVO_PIN2 3   // GPIO2
#define MOTOR_PWM1 2   // virtual, use GPIO1 instead
#define MOTOR_PWM2 1   // GPIO1 (TX) - use carefully
// ESP-01 has only GPIO0 and GPIO2 available as safe GPIOs.

Servo servo1;
Servo servo2;

uint8_t incomingMac[6];

// Define the received data structure
struct __attribute__((packed)) DataPacket {
  uint16_t elevator;
  uint16_t rudder;
  uint16_t not_use;
  uint16_t trottel;
  uint16_t not_use1;
  uint16_t not_use2;
};

DataPacket receivedData;

// Map range from 0–4095 to 1000–2000 (microseconds)
int mapRange(uint16_t x, int in_min = 0, int in_max = 4095, int out_min = 1000, int out_max = 2000) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Callback when data is received
void onDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));

  // Servo Control
  int duty_us1 = mapRange(receivedData.elevator);
  servo1.writeMicroseconds(duty_us1);

  int duty_us2 = mapRange(receivedData.rudder);
  servo2.writeMicroseconds(duty_us2);

  // Motor PWM Control
  if (receivedData.trottel > 2000) {
    int pwmVal = map(receivedData.trottel, 2000, 4095, 0, 1023);
    analogWrite(MOTOR_PWM1, pwmVal);
    analogWrite(MOTOR_PWM2, pwmVal);
  } else {
    analogWrite(MOTOR_PWM1, 0);
    analogWrite(MOTOR_PWM2, 0);
  }

  // Debug output
  Serial.print("elevator = "); Serial.print(receivedData.elevator);
  Serial.print(", rudder = "); Serial.print(receivedData.rudder);
  Serial.print(", trottel = "); Serial.println(receivedData.trottel);
}

void setup() {
  Serial.begin(115200);

  // Initialize WiFi in Station mode
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(onDataRecv);

  // Attach servos
  servo1.attach(SERVO_PIN1);
  servo2.attach(SERVO_PIN2);

  // Setup PWM pins for motor (ESP8266 only has GPIO0 and GPIO2 as usable)
  pinMode(MOTOR_PWM1, OUTPUT);
  pinMode(MOTOR_PWM2, OUTPUT);
}

void loop() {
  // Nothing here; everything is handled in the callback
}
