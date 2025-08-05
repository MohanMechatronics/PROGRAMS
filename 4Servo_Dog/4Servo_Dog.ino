#include <ESP32Servo.h>

Servo servo1; // front left 
Servo servo2; // rear left
Servo servo3; // front right
Servo servo4; // rear right

int middle = 90;
int endPoint = 80;
int stepDelay = 400;

void setup() {
  // Use any available GPIO pins on the ESP32
  servo1.setPeriodHertz(50); // Standard servo frequency
  servo2.setPeriodHertz(50);
  servo3.setPeriodHertz(50);
  servo4.setPeriodHertz(50);

  servo1.attach(13);  // front left  - GPIO 13
  servo2.attach(12);  // rear left   - GPIO 12
  servo3.attach(14);  // front right - GPIO 14
  servo4.attach(27);  // rear right  - GPIO 27
}

void loop() {
  servo1.write(middle);
  servo2.write(middle);
  servo3.write(middle);
  servo4.write(middle);
  delay(stepDelay);

  servo1.write(middle - endPoint);
  delay(stepDelay);

  servo3.write(middle + endPoint);
  delay(stepDelay);

  servo2.write(middle + endPoint);
  delay(stepDelay);

  servo4.write(middle - endPoint);
  delay(stepDelay);
}
