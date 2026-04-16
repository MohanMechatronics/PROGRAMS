//                Back
//  ----- --------     --------- -----
//    14 |  B_LEFT |   | B_RIGHT | 27
//       | SERVO 3 |   | SERVO 4 |
//        --------     --------
//       | F_LEFT  |  | F_RIGHT  |
//    13 | SERVO 1 |  |  SERVO 2 | 12
//  ----- --------    --------- -----
//                Front

#include <Bluepad32.h>
#include <ESP32Servo.h>


ControllerPtr myControllers[BP32_MAX_GAMEPADS];

void onConnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("CALLBACK: Controller connected, index=%d\n", i);
      ControllerProperties properties = ctl->getProperties();
      Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n",
                    ctl->getModelName().c_str(), properties.vendor_id, properties.product_id);
      myControllers[i] = ctl;
      break;
    }
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
      myControllers[i] = nullptr;
      break;
    }
  }
}

// Servo declarations
Servo servo1;  // Right leg
Servo servo2;  // Left leg
Servo servo3;  // Right arm
Servo servo4;  // Left arm

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

// Improved walking using 4 servos
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
    servo1.write(home1 + 20);
    servo4.write(home4 - 20);
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
    servo1.write(home1 - 20);
    servo4.write(home4 + 20);
    servo2.write(home2 - 20);
    servo3.write(home3 + 20);
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
    servo1.write(home1 - 20);
    servo4.write(home4 + 20);
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
    servo1.write(home1 + 20);
    servo4.write(home4 - 20);
    servo2.write(home2 + 20);
    servo3.write(home3 - 20);
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
    servo2.write(home2 - 90);
    delay(200);
    servo2.write(home2 - 30);
    delay(200);
  }
  standhome();
}

void handshake(unsigned int times) {
  servo4.write(180);
  delay(200);
  for (unsigned int i = 0; i < times; i++) {
    servo1.write(home1 + 90);
    delay(200);
    servo1.write(home1 + 30);
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

void downaction(unsigned int step) {
  while (step-- > 0){
  sithome();
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

    // Step 2: Return to center
    standhome();
    delay(100);
  }
}

void shakeL(unsigned int steps) {
  for (unsigned int i = 0; i < steps; i++) {
    servo1.write(home1 + 40);
    servo2.write(home2 + 40);
    servo3.write(home3 + 40);  
    servo4.write(home4 + 40);  
    delay(100);

    // Step 2: Return to center
    standhome();
    delay(100);
    
  }
}

void sithome() {
  servo1.write(180);
  servo2.write(0);
  servo3.write(0);
  servo4.write(180);
}

void dumpGamepad(ControllerPtr ctl) {
  Serial.printf(
    "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, misc: 0x%02x\n",
    ctl->index(), ctl->dpad(), ctl->buttons(), ctl->axisX(), ctl->axisY(),
    ctl->axisRX(), ctl->axisRY(), ctl->brake(), ctl->throttle(), ctl->miscButtons());
}

// Gamepad input handler
void processGamepad(ControllerPtr ctl) {
  if (ctl->axisY() > 75) {
    walkBackward(1);
  }
  else if (ctl->axisY() < -75) {
    walkForward(1);
  }
  else if (ctl->axisX() > 75) {
    turnRight(1);
  }
  else if (ctl->axisX() < -75) {
    turnLeft(1);
  }

  else if (ctl->axisRY() < -75)
  {
    RUN(1);
  }

  else if (ctl->axisRY() > 75)
  {
    RUNBACK(1);
  }

  else if (ctl->dpad() == 0x01) {
    walkForward(1);
  }
  else if (ctl->dpad() == 0x02) {
    walkBackward(1);
  }
  else if (ctl->dpad() == 0x08) {
    turnLeft(1);
  }
  else if (ctl->dpad() == 0x04) {
    turnRight(1);
  }

  if (ctl->a()) sithome(); // X
  if (ctl->x()) twist();   // Square
  if (ctl->b()) standhome(); //cirle
  if (ctl->y()) downaction(2); // tringle
  if (ctl->brake() > 10){
    ctl->playDualRumble(0 /* delayedStartMs */, 500 /* durationMs */, 0x10 /* weakMagnitude */,
                            0x10 /* strongMagnitude */);
    wink(2);    
  } 
  if (ctl->throttle() > 10)
  {
    
    ctl->playDualRumble(0 /* delayedStartMs */, 500 /* durationMs */, 0x10 /* weakMagnitude */,
                            0x10 /* strongMagnitude */);
    handshake(2); 
  }
  o
  if (ctl->buttons()==0x0010) { 
    shakeL(1);
  }
  
  if (ctl->buttons()==0x0020) { 
    shakeR(1);
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
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n",
                addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.enableVirtualDevice(false);

  servo1.attach(13);  // Right leg
  servo2.attach(12);  // Left leg
  servo3.attach(14);  // Right arm
  servo4.attach(27);  // Left arm

  standhome();

}

void loop() {

  if (BP32.update()) {
    processControllers();
  }

}
