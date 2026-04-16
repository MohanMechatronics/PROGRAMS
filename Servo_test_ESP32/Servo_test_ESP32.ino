#include <Servo.h>

// --- Create servo object ---
Servo servo1;

const int servoPin1 = 5;

void setup() {
  Serial.begin(115200);
  delay(1000);   // Give Serial Monitor time to open

  Serial.println("ESP32 Servo Test Started");

  // Attach servo
  servo1.attach(servoPin1);
  Serial.print("Servo attached to pin: ");
  Serial.println(servoPin1);
}


void loop() {
  servo1.write(0);
  Serial.println("Angle = 0");
  delay(1000);

  servo1.write(90);
  Serial.println("Angle = 90");
  delay(1000);

  servo1.write(180);
  Serial.println("Angle = 180");
  delay(1000);
}


// ---- Function to move one servo up and down ----
// void waveServo(Servo &servo, const char *name, int stepDelay) {
//   for (int pos = 90; pos >= 0; pos--) {
//     servo.write(pos);
//     Serial.print(name); Serial.print(" Angle: "); Serial.println(pos);
//     delay(stepDelay);
//   }
//   for (int pos = 0; pos <= 90; pos++) {
//     servo.write(pos);
//     Serial.print(name); Serial.print(" Angle: "); Serial.println(pos);
//     delay(stepDelay);
//   }
// }

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


