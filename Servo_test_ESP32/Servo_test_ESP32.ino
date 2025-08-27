// #include <ESP32Servo.h>

// // --- Create 3 servo objects ---
// Servo servo1;
// Servo servo2;
// Servo servo3;

// const int servoPin1 = 13;
// const int servoPin2 = 12;
// const int servoPin3 = 14;

// void setup() {
//   Serial.begin(115200);   // Start Serial Monitor

//   // Attach all 3 servos
//   servo1.attach(servoPin1);
//   servo2.attach(servoPin2);
//   servo3.attach(servoPin3);

//   // Initialize all at 0°
//   servo1.write(0);
//   servo2.write(0);
//   servo3.write(0);
// }

// void loop() {
//   // Sweep all 3 servos together to 0°
//   sweepServos(0, 10);
//   delay(2000);

//   // Sweep all 3 servos together to 90°
//   sweepServos(50, 10);
//   delay(2000);

//   // Sweep all 3 servos back to 0°
//   sweepServos(0, 10);
//   delay(2000);
// }

// // --- Function to sweep ALL servos slowly and in sync ---
// void sweepServos(int targetAngle, int stepDelay) {
//   int current1 = servo1.read();
//   int current2 = servo2.read();
//   int current3 = servo3.read();

//   // Assuming all move same direction
//   if (current1 < targetAngle) {
//     for (int pos = current1; pos <= targetAngle; pos++) {
//       servo1.write(pos);
//       servo2.write(pos);
//       servo3.write(pos);

//       Serial.print("Servo1: "); Serial.print(pos);
//       Serial.print("  Servo2: "); Serial.print(pos);
//       Serial.print("  Servo3: "); Serial.println(pos);

//       delay(stepDelay);
//     }
//   } else {
//     for (int pos = current1; pos >= targetAngle; pos--) {
//       servo1.write(pos);
//       servo2.write(pos);
//       servo3.write(pos);

//       Serial.print("Servo1: "); Serial.print(pos);
//       Serial.print("  Servo2: "); Serial.print(pos);
//       Serial.print("  Servo3: "); Serial.println(pos);

//       delay(stepDelay);
//     }
//   }
// }

#include <ESP32Servo.h>

// --- Create 3 servo objects ---
Servo servo1;
Servo servo2;
Servo servo3;

const int servoPin1 = 13;
const int servoPin2 = 12;
const int servoPin3 = 14;

void setup() {
  Serial.begin(115200);

  // Attach servos
  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
  servo3.attach(servoPin3);

  // Start all at 0°
  servo1.write(90);
  servo2.write(90);
  servo3.write(90);
}

// ---- Function to move one servo up and down ----
void waveServo(Servo &servo, const char *name, int stepDelay) {
  for (int pos = 90; pos >= 0; pos--) {
    servo.write(pos);
    Serial.print(name); Serial.print(" Angle: "); Serial.println(pos);
    delay(stepDelay);
  }
  for (int pos = 0; pos <= 90; pos++) {
    servo.write(pos);
    Serial.print(name); Serial.print(" Angle: "); Serial.println(pos);
    delay(stepDelay);
  }
}

void loop() {
  // Servo1 does wave
  waveServo(servo1, "Servo1", 10);  // 50ms delay per step = very slow

  // Servo2 starts after Servo1 peaks (during Servo1 decrease)
  waveServo(servo2, "Servo2", 10);

  // Servo3 starts after Servo2 peaks (during Servo2 decrease)
  waveServo(servo3, "Servo3", 10);

  // Then it cycles back automatically
}
