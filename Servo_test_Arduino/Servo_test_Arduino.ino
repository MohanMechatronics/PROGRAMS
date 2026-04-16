#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

void setup() {
  servo1.attach(8);
  servo2.attach(9);
  servo3.attach(10);
  servo4.attach(11);
}

void loop() {
  // Sweep from 0 to 180
  for (int pos = 0; pos <= 180; pos++) {
    servo1.write(pos);
    servo2.write(pos);
    servo3.write(pos);
    servo4.write(pos);
    delay(15);
  }

  // Sweep back from 180 to 0
  for (int pos = 180; pos >= 0; pos--) {
    servo1.write(pos);
    servo2.write(pos);
    servo3.write(pos);
    servo4.write(pos);
    delay(15);
  }
}