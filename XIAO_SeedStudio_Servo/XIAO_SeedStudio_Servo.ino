// ===== ESP32-S3 Servo using NEW LEDC API (Core 3.x) =====

const int servoPin = D8;    // D8 → GPIO 8 (XIAO ESP32-S3)

// 50 Hz servo PWM, 16-bit resolution
const int pwmFreq = 50;
const int pwmResolution = 16;

// Duty values (16-bit)
const uint32_t duty0   = 1638;  // ~0°
const uint32_t duty90  = 4915;  // ~90°
const uint32_t duty180 = 8192;  // ~180°

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("ESP32-S3 Servo Test (NEW LEDC)");

  // NEW API (ESP32 core 3.x)
  ledcAttach(servoPin, pwmFreq, pwmResolution);

  Serial.print("Servo attached to GPIO ");
  Serial.println(servoPin);
}

void loop() {
  Serial.println("Angle = 0");
  ledcWrite(servoPin, duty0);
  delay(500);

  Serial.println("Angle = 90");
  ledcWrite(servoPin, duty90);
  delay(500);

  Serial.println("Angle = 180");
  ledcWrite(servoPin, duty180);
  delay(500);
}
