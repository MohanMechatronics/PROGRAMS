#include <Bluepad32.h>
#include <ESP32Servo.h>
#include <Arduino.h>
#include <vector>

#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1

#define FORWARD 1
#define BACKWARD -1
#define STOP 0

struct MOTOR_PINS {
  int pinIN1;
  int pinIN2;
};

MOTOR_PINS motorPins[] = {
  {6, 7},   // LEFT_MOTOR
  {21, 20}  // RIGHT_MOTOR
};

#define MOTOR_COUNT 2

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// ─── Forward declarations ───────────────────────────────────────────────────
void processGamepad(ControllerPtr ctl);
void rotateMotor(int motorNumber, int motorDirection);

// ─── Bluetooth Callbacks ─────────────────────────────────────────────────────

void onConnectedController(ControllerPtr ctl) {
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("CALLBACK: Controller connected, index=%d\n", i);
      ControllerProperties properties = ctl->getProperties();
      Serial.printf("Model: %s, VID=0x%04x, PID=0x%04x\n",
                    ctl->getModelName().c_str(),
                    properties.vendor_id,
                    properties.product_id);
      myControllers[i] = ctl;
      foundEmptySlot = true;
      break;
    }
  }
  if (!foundEmptySlot) {
    Serial.println("CALLBACK: Controller connected, but no empty slot found");
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  bool foundController = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("CALLBACK: Controller disconnected, index=%d\n", i);
      myControllers[i] = nullptr;
      foundController = true;

      // Safety stop on disconnect
      rotateMotor(LEFT_MOTOR, STOP);
      rotateMotor(RIGHT_MOTOR, STOP);
      break;
    }
  }
  if (!foundController) {
    Serial.println("CALLBACK: Controller disconnected, but not found in slot");
  }
}

// ─── Motor Control ────────────────────────────────────────────────────────────

void rotateMotor(int motorNumber, int motorDirection) {
  if (motorDirection == FORWARD) {
    digitalWrite(motorPins[motorNumber].pinIN1, HIGH);
    digitalWrite(motorPins[motorNumber].pinIN2, LOW);
  } else if (motorDirection == BACKWARD) {
    digitalWrite(motorPins[motorNumber].pinIN1, LOW);
    digitalWrite(motorPins[motorNumber].pinIN2, HIGH);
  } else {
    // STOP
    digitalWrite(motorPins[motorNumber].pinIN1, LOW);
    digitalWrite(motorPins[motorNumber].pinIN2, LOW);
  }
}

void setUpPinModes() {
  for (int i = 0; i < MOTOR_COUNT; i++) {
    pinMode(motorPins[i].pinIN1, OUTPUT);
    pinMode(motorPins[i].pinIN2, OUTPUT);
    rotateMotor(i, STOP);
  }
}

// ─── Gamepad Logic ────────────────────────────────────────────────────────────

void dumpGamepad(ControllerPtr ctl) {
  Serial.printf(
    "idx=%d, dpad=0x%02x, buttons=0x%04x, "
    "axisL: %4d,%4d, axisR: %4d,%4d, "
    "brake=%4d, throttle=%4d, misc=0x%02x\n",
    ctl->index(),
    ctl->dpad(),
    ctl->buttons(),
    ctl->axisX(),
    ctl->axisY(),
    ctl->axisRX(),   // Fixed: was axisX() before
    ctl->axisRY(),   // Fixed: was axisY() before
    ctl->brake(),
    ctl->throttle(),
    ctl->miscButtons()
  );
}

void processGamepad(ControllerPtr ctl) {

  // ── Movement (priority order: throttle/brake > axis > dpad) ──
  if (ctl->throttle() > 10) {
    rotateMotor(RIGHT_MOTOR, FORWARD);
    rotateMotor(LEFT_MOTOR, FORWARD);
    Serial.println("THROTTLE → FORWARD");

  } else if (ctl->brake() > 10) {
    rotateMotor(RIGHT_MOTOR, BACKWARD);
    rotateMotor(LEFT_MOTOR, BACKWARD);
    Serial.println("BRAKE → BACKWARD");

  } else if (ctl->axisY() > 75) {
    rotateMotor(RIGHT_MOTOR, BACKWARD);
    rotateMotor(LEFT_MOTOR, BACKWARD);
    Serial.println("AXIS → BACKWARD");

  } else if (ctl->axisY() < -75) {
    rotateMotor(RIGHT_MOTOR, FORWARD);
    rotateMotor(LEFT_MOTOR, FORWARD);
    Serial.println("AXIS → FORWARD");

  } else if (ctl->axisX() > 75) {
    rotateMotor(LEFT_MOTOR, FORWARD);
    rotateMotor(RIGHT_MOTOR, BACKWARD);
    Serial.println("AXIS → TURN LEFT");

  } else if (ctl->axisX() < -75) {
    rotateMotor(RIGHT_MOTOR, FORWARD);
    rotateMotor(LEFT_MOTOR, BACKWARD);
    Serial.println("AXIS → TURN RIGHT");

  } else if (ctl->dpad() == 0x01) {
    rotateMotor(RIGHT_MOTOR, FORWARD);
    rotateMotor(LEFT_MOTOR, FORWARD);
    Serial.println("DPAD → FORWARD");

  } else if (ctl->dpad() == 0x02) {
    rotateMotor(RIGHT_MOTOR, BACKWARD);
    rotateMotor(LEFT_MOTOR, BACKWARD);
    Serial.println("DPAD → BACKWARD");

  } else if (ctl->dpad() == 0x08) {
    rotateMotor(RIGHT_MOTOR, FORWARD);
    rotateMotor(LEFT_MOTOR, BACKWARD);
    Serial.println("DPAD → TURN RIGHT");

  } else if (ctl->dpad() == 0x04) {
    rotateMotor(LEFT_MOTOR, FORWARD);
    rotateMotor(RIGHT_MOTOR, BACKWARD);
    Serial.println("DPAD → TURN LEFT");

  } else {
    rotateMotor(RIGHT_MOTOR, STOP);
    rotateMotor(LEFT_MOTOR, STOP);
  }

  // ── Button A: Cycle LED color ──
  if (ctl->a()) {
    static int colorIdx = 0;
    switch (colorIdx % 3) {
      case 0: ctl->setColorLED(255, 0, 0); break;  // Red
      case 1: ctl->setColorLED(0, 255, 0); break;  // Green
      case 2: ctl->setColorLED(0, 0, 255); break;  // Blue
    }
    colorIdx++;
  }

  // ── Button X: Rumble ──
  if (ctl->x()) {
    ctl->playDualRumble(0, 500, 0x10, 0x10);
  }

  // ── Debug dump ──
  dumpGamepad(ctl);
}

// ─── Process All Controllers ──────────────────────────────────────────────────

void processControllers() {
  for (auto myController : myControllers) {
    if (myController && myController->isConnected()) {
      if (myController->isGamepad()) {
        processGamepad(myController);
      } else {
        Serial.println("Unsupported controller type");
      }
    }
  }
}

// ─── Setup & Loop ─────────────────────────────────────────────────────────────

void setup() {
  setUpPinModes();

  Serial.begin(115200);
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());

  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n",
                addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.enableVirtualDevice(false);

  // Uncomment the line below ONCE if controller won't pair, then comment it out again
  // BP32.forgetBluetoothKeys();
}

void loop() {
  bool dataUpdated = BP32.update();
  if (dataUpdated) {
    processControllers();
  }
  // Yields to FreeRTOS BT stack for 1ms — prevents disconnection from CPU starvation
  vTaskDelay(1);
}