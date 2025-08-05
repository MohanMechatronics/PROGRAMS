#include <Bluepad32.h>
#include <ESP32Servo.h>

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedController(ControllerPtr ctl) {
  
  bool foundEmptySlot = false;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
      // Additionally, you can get certain gamepad properties like:
      // Model, VID, PID, BTAddr, flags, etc.
      ControllerProperties properties = ctl->getProperties();
      Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id, properties.product_id);
      myControllers[i] = ctl;
      foundEmptySlot = true;
      break;
      }
    }

    if (!foundEmptySlot) {
      Serial.println("CALLBACK: Controller connected, but could not found empty slot");
    }
}

void onDisconnectedController(ControllerPtr ctl) {
  bool foundController = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
      myControllers[i] = nullptr;
      foundController = true;
      break;
    }
  }

    if (!foundController) {
      Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
    }
}

void dumpGamepad(ControllerPtr ctl) {
  Serial.printf(
    "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, misc: 0x%02x\n",
    ctl->index(), ctl->dpad(), ctl->buttons(), ctl->axisX(), ctl->axisY(),
    ctl->axisRX(), ctl->axisRY(), ctl->brake(), ctl->throttle(), ctl->miscButtons());
}

// Servo declarations (4 DOF robot)
Servo servo1;  // Right leg
Servo servo2;  // Left leg
Servo servo3;  // Right arm
Servo servo4;  // Left arm

// Neutral/home positions
int home1 = 90;
int home2 = 90;
int home3 = 90;
int home4 = 90;

// Movement functions
void standhome() {
  servo1.write(home1);
  servo2.write(home2);
  servo3.write(home3);
  servo4.write(home4);
}

void idle() {
  delay(100);
}

void forward(unsigned int step) {
  while (step-- > 0) {
    servo1.write(home1 + 30);
    servo2.write(home2 - 30);
    delay(200);
    servo1.write(home1);
    servo2.write(home2);
    delay(200);
  }
}

void backward(unsigned int step) {
  while (step-- > 0) {
    servo1.write(home1 - 30);
    servo2.write(home2 + 30);
    delay(200);
    servo1.write(home1);
    servo2.write(home2);
    delay(200);
  }
}

void leftturn(unsigned int step) {
  while (step-- > 0) {
    servo3.write(home3 - 30);
    servo4.write(home4 + 30);
    delay(200);
    servo3.write(home3);
    servo4.write(home4);
    delay(200);
  }
}

void rightturn(unsigned int step) {
  while (step-- > 0) {
    servo3.write(home3 + 30);
    servo4.write(home4 - 30);
    delay(200);
    servo3.write(home3);
    servo4.write(home4);
    delay(200);
  }
}

void wink(unsigned int step) {
  while (step-- > 0) {
    servo3.write(home3 - 45);
    delay(200);
    servo3.write(home3 + 45);
    delay(200);
  }
  servo3.write(home3);
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

void downaction(unsigned int step) {
  while (step-- > 0) {
    servo1.write(home1 + 20);
    servo2.write(home2 + 20);
    delay(200);
    servo1.write(home1 - 20);
    servo2.write(home2 - 20);
    delay(200);
  }
  standhome();
}

void sithome() {
  servo1.write(90);
  servo2.write(90);
  servo3.write(60);
  servo4.write(60);
}

// Controller input handler
void processGamepad(ControllerPtr ctl) {
  if (ctl->axisY() > 75) {
    backward(1);
  }
  else if (ctl->axisY() < -75) {
    forward(1);
  }
  else if (ctl->axisX() > 75) {
    rightturn(1);
  }
  else if (ctl->axisX() < -75) {
    leftturn(1);
  }
  else if (ctl->dpad() == 0x01) {
    forward(1);
  }
  else if (ctl->dpad() == 0x02) {
    backward(1);
  }
  else if (ctl->dpad() == 0x08) {
    leftturn(1);
  }
  else if (ctl->dpad() == 0x04) {
    rightturn(1);
  }
  else {
    idle();
  }

  if (ctl->a()) {
    wink(2);
  }

  if (ctl->x()) {
    twist();
  }

  if (ctl->b()) {
    standhome();
  }

  if (ctl->y()) {
    sithome();
  }

  if (ctl->brake() > 10) {
    wink(3);
  }

  if (ctl->throttle() > 10) {
    downaction(2);
  }

  dumpGamepad(ctl);
}

void processControllers() {
  for (auto ctl : myControllers) {
    if (ctl && ctl->isConnected() && ctl->hasData()) {
      if (ctl->isGamepad()) {
        processGamepad(ctl);
      } else {
        Serial.println("Unsupported controller");
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.enableVirtualDevice(false);

  // Attach servos to pins
  servo1.attach(13); // Right leg
  servo2.attach(12); // Left leg
  servo3.attach(14); // Right arm
  servo4.attach(27); // Left arm

  standhome(); // Set neutral position
}

void loop() {
  bool dataUpdated = BP32.update();
  if (dataUpdated){
    processControllers();
  }
}
