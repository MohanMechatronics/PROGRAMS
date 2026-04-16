#include <ESP32Servo.h>
#include <math.h>

#if defined(ESP32)
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;
#endif

// =====================================================
// CONFIGURATION
// =====================================================
#define SERVOMIN     1000
#define SERVOMAX     2000
#define CENTER       1500
#define NUM_SERVOS   8
#define DELAY_TIME   5
#define SPEED_FACTOR 0.5f
#define MAX_STEP     20
#define MOVE_STEPS   40
#define PLOT_MODE    true

// ================= SERVO PINS =================
#define SERVO0_PIN  13
#define SERVO1_PIN  12
#define SERVO2_PIN  14
#define SERVO3_PIN  27
#define SERVO4_PIN  26
#define SERVO5_PIN  25
#define SERVO6_PIN  33
#define SERVO7_PIN  32
#define NECK_PIN    15

// =====================================================
// SERVO OBJECTS
// =====================================================
Servo servos[NUM_SERVOS];
Servo neckServo;

// =====================================================
// GAIT PARAMETERS
// =====================================================
const float FREQUENCY = 0.006f;
const float AMPLITUDE = 200.0f;

const float off_set_walk[NUM_SERVOS] = { 0, -100, 0, 100, -50, -50, 50, 50 };

const float phase[NUM_SERVOS] = {
  PI + PI/8, PI/2 + PI + PI/8,
  PI/8, PI/2 + PI/8,
  -PI/8, PI/2 - PI/8,
  PI - PI/8, PI/2 + PI - PI/8
};

unsigned long currentMillis = 0;
unsigned long oldMillis = 0;
unsigned long innerTime = 0;

// =====================================================
// GLOBAL STATE
// =====================================================
float currentPos[NUM_SERVOS];
float targetPos[NUM_SERVOS];
float lastPulse[NUM_SERVOS];

enum Mode {
  MODE_IDLE,
  MODE_WALK_FORWARD,
  MODE_WALK_LEFT,
  MODE_WALK_RIGHT,
  MODE_SPOT_LEFT,
  MODE_SPOT_RIGHT
};
Mode currentMode = MODE_IDLE;

// =====================================================
// WRITE SERVO
// =====================================================
void writeServo(int i, float pulse) {
  pulse = constrain(pulse, SERVOMIN, SERVOMAX);
  servos[i].writeMicroseconds((int)pulse);
  lastPulse[i] = pulse;
}

// =====================================================
// INTERPOLATION MOVE
// =====================================================
void moveUntilReachedAll() {
  float startPos[NUM_SERVOS];
  for (int i = 0; i < NUM_SERVOS; i++)
    startPos[i] = currentPos[i];

  for (int step = 1; step <= MOVE_STEPS; step++) {
    float t = (float)step / MOVE_STEPS;

    for (int i = 0; i < NUM_SERVOS; i++) {
      currentPos[i] = startPos[i] + t * (targetPos[i] - startPos[i]);
      writeServo(i, currentPos[i]);
    }
    delay(DELAY_TIME);
  }

  for (int i = 0; i < NUM_SERVOS; i++) {
    currentPos[i] = targetPos[i];
    writeServo(i, currentPos[i]);
  }
}

// =====================================================
// IDLE UPDATE
// =====================================================
void updateAllServos() {
  for (int i = 0; i < NUM_SERVOS; i++) {
    float diff = targetPos[i] - currentPos[i];
    if (fabs(diff) > 1) {
      currentPos[i] += constrain(diff * SPEED_FACTOR, -MAX_STEP, MAX_STEP);
      writeServo(i, currentPos[i]);
    }
  }
  delay(DELAY_TIME);
}

// =====================================================
// WALK ENGINE
// =====================================================
void runWalkSequence(const float ampScale[NUM_SERVOS]) {
  oldMillis = currentMillis;
  currentMillis = millis();
  innerTime += currentMillis - oldMillis;

  float t = FREQUENCY * innerTime;

  for (int s = 0; s < NUM_SERVOS; s += 2) {
    float pulse = CENTER + off_set_walk[s]
      + ampScale[s] * AMPLITUDE * cos(t + phase[s]);
    writeServo(s, pulse);
  }

  for (int s = 1; s < NUM_SERVOS; s += 2) {
    float c = max(0.0f, cos(t + phase[s]));
    float pulse = CENTER + off_set_walk[s]
      + ampScale[s] * AMPLITUDE * c;
    writeServo(s, pulse);
  }

  delay(DELAY_TIME);
}

void runWalkForward() {
  static const float a[NUM_SERVOS] =
  { 1,1,-1,-1,1,1,-1,-1 };
  runWalkSequence(a);
}

// =====================================================
// SETUP
// =====================================================
void setup() {
  Serial.begin(115200);

#if defined(ESP32)
  SerialBT.begin("ESP32_RobotDog");
#endif

  // Attach servos
  int pins[NUM_SERVOS] = {
    SERVO0_PIN, SERVO1_PIN, SERVO2_PIN, SERVO3_PIN,
    SERVO4_PIN, SERVO5_PIN, SERVO6_PIN, SERVO7_PIN
  };

  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].setPeriodHertz(50);
    servos[i].attach(pins[i], SERVOMIN, SERVOMAX);
    currentPos[i] = CENTER;
    targetPos[i]  = CENTER;
    writeServo(i, CENTER);
  }

  neckServo.setPeriodHertz(50);
  neckServo.attach(NECK_PIN, SERVOMIN, SERVOMAX);
  neckServo.writeMicroseconds(CENTER);

  Serial.println("ESP32 Servo Robot Ready");
}

// =====================================================
// LOOP
// =====================================================
void loop() {

  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();

    if (cmd == "WALK") currentMode = MODE_WALK_FORWARD;
    else if (cmd == "STOP") currentMode = MODE_IDLE;
  }

  switch (currentMode) {
    case MODE_WALK_FORWARD: runWalkForward(); break;
    default: updateAllServos(); break;
  }
}