#include <ESP32Servo.h>

const int motorPin[4] = {1, 3, 4, 48};
const int servoPin[3] = {5, 8, 38};

Servo servos[3];

void setup() {
  Serial.begin(115200);
  Serial.println("Motor Speed Control with DAC + Random Servo Movement");

  // Set motor pins as output
  for (int i = 0; i < 4; i++) {
    pinMode(motorPin[i], OUTPUT);
  }

  // Attach servos to pins
  for (int i = 0; i < 3; i++) {
    servos[i].attach(servoPin[i]);
    servos[i].write(90); // center position initially
  }

  randomSeed(analogRead(0)); // Seed random with analog input
}

void loop() {
  // Increase motor speed
  for (int speed = 0; speed <= 255; speed += 10) {
    for (int j = 0; j < 4; j++) {
      analogWrite(motorPin[j], speed);
      Serial.print("Increasing speed on pin ");
      Serial.print(motorPin[j]);
      Serial.print(": ");
      Serial.println(speed);
    }

    moveServosRandom(); // Move servos randomly during this time
    delay(1000);
  }

  delay(1000);

  // Decrease motor speed
  for (int speed = 255; speed >= 0; speed -= 10) {
    for (int j = 0; j < 4; j++) {
      analogWrite(motorPin[j], speed);
      Serial.print("Decreasing speed on pin ");
      Serial.print(motorPin[j]);
      Serial.print(": ");
      Serial.println(speed);
    }

    moveServosRandom(); // Move servos randomly during this time
    delay(1000);
  }

  delay(1000);
}

// Function to move all servos to random positions
void moveServosRandom() {
  for (int i = 0; i < 3; i++) {
    int angle = random(0, 181); // 0 to 180 degrees
    servos[i].write(angle);
    Serial.print("Moving servo on pin ");
    Serial.print(servoPin[i]);
    Serial.print(" to ");
    Serial.print(angle);
    Serial.println(" degrees");
  }
}
