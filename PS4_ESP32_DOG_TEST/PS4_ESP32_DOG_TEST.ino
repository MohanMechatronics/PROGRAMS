#include <ESP32Servo.h>

// Servo objects
Servo servoFL; // Front Left
Servo servoFR; // Front Right
Servo servoBL; // Back Left
Servo servoBR; // Back Right

// Pin mapping (modify these pins for your ESP32 setup)
const int pinFL = 13;
const int pinFR = 12;
const int pinBL = 14;
const int pinBR = 27;

// Angles for gait
const int angleForward = 60;
const int angleDown    = 90;
const int angleBackward = 120;

// Time between steps
const int stepDelay = 400;

void setup() {
  servoFL.attach(pinFL);
  servoFR.attach(pinFR);
  servoBL.attach(pinBL);
  servoBR.attach(pinBR);

  // Initial standing pose
  standStill();
  delay(1000);
}

void loop() {
  // Step 1: FL forward, BL backward
  servoFL.write(angleForward);
  servoFR.write(angleDown);
  servoBL.write(angleBackward);
  servoBR.write(angleDown);
  delay(stepDelay);

  // Step 2: FR forward, BR backward
  servoFL.write(angleDown);
  servoFR.write(angleForward);
  servoBL.write(angleDown);
  servoBR.write(angleBackward);
  delay(stepDelay);

  // Step 3: FL backward, BL forward
  servoFL.write(angleBackward);
  servoFR.write(angleDown);
  servoBL.write(angleForward);
  servoBR.write(angleDown);
  delay(stepDelay);

  // Step 4: FR backward, BR forward
  servoFL.write(angleDown);
  servoFR.write(angleBackward);
  servoBL.write(angleDown);
  servoBR.write(angleForward);
  delay(stepDelay);
}

// Optional: Function to set robot to standing pose
void standStill() {
  servoFL.write(angleDown);
  servoFR.write(angleDown);
  servoBL.write(angleDown);
  servoBR.write(angleDown);
}
