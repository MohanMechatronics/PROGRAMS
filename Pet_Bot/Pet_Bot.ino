//                Back
//  ----- --------     --------- -----
//    14 |  B_LEFT |   | B_RIGHT | 27
//       | SERVO 3 |   | SERVO 4 |
//        --------     --------
//       | F_LEFT  |  | F_RIGHT  |
//    13 | SERVO 1 |  |  SERVO 2 | 12
//  ----- --------    --------- -----
//                Front

// OLD PIN SETUP //
// IO1 - servo 1
// IO2 - servo 2
// IO3 - servo 3
// IO4 - servo 4

// NEW PIN SETUP //
// IO1 - servo 2
// IO2 - servo 4
// IO3 - servo 1
// IO4 - servo 3




#include <ESP32Servo.h>

// Servo declarations
Servo servo1;  // Right leg 13
Servo servo2;  // Left leg 12
Servo servo3;  // Right arm 14
Servo servo4;  // Left arm 27

// Home angles
int home1 = 120;
int home2 = 60;
int home3 = 60;
int home4 = 120;
int home = 90;

void standhome() {
  servo1.write(home1);
  servo2.write(home2);
  servo3.write(home3);
  servo4.write(home4);
}

void sithome() {
  servo1.write(180);
  servo2.write(0);
  servo3.write(0);
  servo4.write(180);
}

// ---------- MOVEMENTS ----------
void walkForward(unsigned int steps) {
  for (unsigned int i = 0; i < steps; i++) {
    servo1.write(home1 + 20);
    servo4.write(home4 - 20);
    delay(100);
    servo1.write(home1 + 40);
    servo4.write(home4 - 40);
    servo2.write(home2 + 20);
    servo3.write(home3 - 20);
    delay(100);
    standhome();
    delay(100);
    servo2.write(home2 - 20);
    servo3.write(home3 + 20);
    delay(100);
    servo1.write(home1 - 20);
    servo4.write(home4 + 20);
    servo2.write(home2 - 40);
    servo3.write(home3 + 40);
    delay(100);
    standhome();
  }
}

void walkBackward(unsigned int steps) {
  for (unsigned int i = 0; i < steps; i++) {
    servo1.write(home1 - 20);
    servo4.write(home4 + 20);
    delay(100);
    servo1.write(home1 - 40);
    servo4.write(home4 + 40);
    servo2.write(home2 - 20);
    servo3.write(home3 + 20);
    delay(100);
    standhome();
    delay(100);
    servo2.write(home2 + 20);
    servo3.write(home3 - 20);
    delay(100);
    servo1.write(home1 + 20);
    servo4.write(home4 - 20);
    servo2.write(home2 + 40);
    servo3.write(home3 - 40);
    delay(100);
    standhome();
  }
}

void turnLeft(unsigned int steps) {
for (unsigned int i = 0; i < steps; i++) {
    servo1.write(home + 20);
    servo4.write(home + 20);
    delay(100);
    servo1.write(home + 40);
    servo4.write(home + 40);
    servo2.write(home - 20);
    servo3.write(home - 20);
    delay(100);
    servo1.write(home + 20);
    servo4.write(home + 20);
    servo2.write(home - 20);
    servo3.write(home - 20);
    delay(100);
    standhome();
    delay(100);
    servo2.write(home + 20);
    servo3.write(home + 20);
    delay(100);
    servo1.write(home - 20);
    servo4.write(home - 20);
    servo2.write(home + 40);
    servo3.write(home + 40);
    delay(100);
    servo1.write(home - 20);
    servo4.write(home - 20);
    servo2.write(home + 20);
    servo3.write(home + 20);
    delay(100);
    standhome();
  }
}

void turnRight(unsigned int steps) {
for (unsigned int i = 0; i < steps; i++) {
    servo1.write(home - 20);
    servo4.write(home - 20);
    delay(100);
    servo1.write(home - 40);
    servo4.write(home - 40);
    servo2.write(home + 20);
    servo3.write(home + 20);
    delay(100);
    servo1.write(home - 20);
    servo4.write(home - 20);
    servo2.write(home + 20);
    servo3.write(home + 20);
    delay(100);
    standhome();
    delay(100);
    servo2.write(home - 20);
    servo3.write(home - 20);
    delay(100);
    servo1.write(home + 20);
    servo4.write(home + 20);
    servo2.write(home - 40);
    servo3.write(home - 40);
    delay(100);
    servo1.write(home + 20);
    servo4.write(home + 20);
    servo2.write(home - 20);
    servo3.write(home - 20);
    delay(100);
    standhome();
  }
}

void wink(unsigned int times) {
  servo3.write(0);
  delay(200);
  for (unsigned int i = 0; i < times; i++) {
    servo2.write(home - 90);
    delay(200);
    servo2.write(home - 30);
    delay(200);
  }
  standhome();
}

void handshake(unsigned int times) {
  servo4.write(180);
  delay(200);
  for (unsigned int i = 0; i < times; i++) {
    servo1.write(home + 90);
    delay(200);
    servo1.write(home + 30);
    delay(200);
  }
  standhome();
}

void twist() {
  for (int i = 0; i < 2; i++) {
    servo1.write(home1 + 20);
    servo2.write(home2 - 20);
    delay(150);
    servo1.write(home1 - 20);
    servo2.write(home2 + 20);
    delay(150);
  }
  standhome();
}

void shakeL(unsigned int steps) {
  for (unsigned int i = 0; i < steps; i++) {
    servo1.write(home1 + 40);
    servo2.write(home2 + 40);
    servo3.write(home3 + 40);
    servo4.write(home4 + 40);
    delay(100);
    standhome();
    delay(100);
  }
}

void shakeR(unsigned int steps) {
  for (unsigned int i = 0; i < steps; i++) {
    servo1.write(home1 - 40);
    servo2.write(home2 - 40);
    servo3.write(home3 - 40);
    servo4.write(home4 - 40);
    delay(100);
    standhome();
    delay(100);
  }
}

void downaction(unsigned int step) {
  while (step-- > 0){
  sithome();
  delay(100);
  standhome();
  delay(100);
  }
}

void RUN(unsigned int steps) {
for (unsigned int i = 0; i < steps; i++) {
    servo1.write(home1 + 60);
    servo4.write(home4 - 60);
    servo2.write(home2 - 60);
    servo3.write(home3 + 60);
    standhome();
    delay(100);
    servo1.write(home1 - 60);
    servo4.write(home4 + 60);
    servo2.write(home2 + 60);
    servo3.write(home3 - 60);
    standhome();
    servo1.write(home1 + 60);
    servo4.write(home4 - 60);
    servo2.write(home2 - 60);
    servo3.write(home3 + 60);
    delay(100);
    standhome();
   
  }
}


void RUNBACK(unsigned int steps) {
for (unsigned int i = 0; i < steps; i++) {
    servo1.write(home1 - 60);
    servo4.write(home4 + 60);
    servo2.write(home2 + 60);
    servo3.write(home3 - 60);
    standhome();
    delay(100);
    servo1.write(home1 + 60);
    servo4.write(home4 - 60);
    servo2.write(home2 - 60);
    servo3.write(home3 + 60);
    standhome();
    servo1.write(home1 - 60);
    servo4.write(home4 + 60);
    servo2.write(home2 + 60);
    servo3.write(home3 - 60);
    delay(100);
    standhome();
  }
}


void setup() {
  Serial.begin(115200);
  servo1.attach(1);  // Right leg
  servo2.attach(2);  // Left leg
  servo3.attach(5);  // Right arm
  servo4.attach(4);  // Left arm
  standhome();
  delay(1000);
  Serial.println("COBOT DEMO START");
}

void loop() {
  standhome();
  delay(4000);

  walkForward(5);
  delay(2000);

  walkBackward(5);
  delay(2000);

  turnLeft(5);
  delay(2000);

  turnRight(5);
  delay(2000);

  wink(4);
  delay(2000);

  handshake(4);
  delay(2000);

  twist();
  delay(2000);

  shakeL(4);
  delay(2000);

  shakeR(4);
  delay(2000);

  sithome();
  delay(2000);

  standhome();
  delay(2000);
}

