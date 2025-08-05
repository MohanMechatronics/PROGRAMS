#include <Bluepad32.h> // 78:EE:4C:00:53:86

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
  "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, "
  "misc: 0x%02x\n",
  ctl->index(),        // Controller Index
  ctl->dpad(),         // D-pad
  ctl->buttons(),      // bitmask of pressed buttons
  ctl->axisX(),        // (-511 - 512) left X Axis
  ctl->axisY(),        // (-511 - 512) left Y axis
  ctl->axisX(),       // (-511 - 512) right X axis
  ctl->axisY(),       // (-511 - 512) right Y axis
  ctl->brake(),        // (0 - 1023): brake button
  ctl->throttle(),     // (0 - 1023): throttle (AKA gas) button
  ctl->miscButtons()  // bitmask of pressed "misc" buttons
  );
}


void processControllers() {
  for (auto myController : myControllers) {
    if (myController && myController->isConnected() && myController->hasData()) {
      if (myController->isGamepad()) {
         processGamepad(myController);
      }
      else {
        Serial.println("Unsupported controller");
      }
    }
  }
}

// Define motor control pins
const int enableA = 21;
const int enableB = 25;

const int leftF = 18;
const int leftB = 19;
const int rightF = 23;
const int rightB = 22;

void processGamepad(ControllerPtr ctl) {

  if ( ctl->axisX() < -75 && ctl->axisY() > 75)
  {
    // processCarMovement(DOWN_LEFT);    
  }
  else if ( ctl->axisX() > 75 && ctl->axisY() > 75)
  {
    // processCarMovement(DOWN_RIGHT);    
  } 
  else if ( ctl->axisX() < -75 && ctl->axisY() < -75)
  {
    // processCarMovement(UP_LEFT);    
  }
  else if ( ctl->axisX() > 75 && ctl->axisY() < -75)
  {
    // processCarMovement(UP_RIGHT);    
  }  
  else if (ctl->axisRX() > 75)
  {
    TURN_LEFT();
    ctl->playDualRumble(0 /* delayedStartMs */, 500 /* durationMs */, 0x10 /* weakMagnitude */,
                            0x10 /* strongMagnitude */);
  }
  else if (ctl->axisRX() < -75)
  {
    TURN_RIGHT();
    ctl->playDualRumble(0 /* delayedStartMs */, 500 /* durationMs */, 0x10 /* weakMagnitude */,
                            0x10 /* strongMagnitude */);
  }
  else if (ctl->axisY() > 75)
  {
    backward();  
  }
  else if (ctl->axisY() < -75)
  {
    forward();     
  }
  else if (ctl->axisX() > 75)
  {
    driftRight();   
  }
  else if (ctl->axisX() < -75)
  {
    driftLeft();    
  } 
  else
  {
    stopCar();     
  }
  if (ctl->a()) {
        static int colorIdx = 0;
        // Some gamepads like DS4 and DualSense support changing the color LED.
        // It is possible to change it by calling:
        switch (colorIdx % 3) {
            case 0:
                // Red
                ctl->setColorLED(255, 0, 0);
                break;
            case 1:
                // Green
                ctl->setColorLED(0, 255, 0);
                break;
            case 2:
                // Blue
                ctl->setColorLED(0, 0, 255);
                break;
        }
        colorIdx++;
  }
  if (ctl->x()) {
        // Some gamepads like DS3, DS4, DualSense, Switch, Xbox One S, Stadia support rumble.
        // It is possible to set it by calling:
        // Some controllers have two motors: "strong motor", "weak motor".
        // It is possible to control them independently.
        ctl->playDualRumble(0 /* delayedStartMs */, 500 /* durationMs */, 0x10 /* weakMagnitude */,
                            0x10 /* strongMagnitude */);
  }
  dumpGamepad(ctl);
}

void onConnect()
{
  Serial.println("Connected!.");
}

void onDisConnect()
{
  stopCar();
  Serial.println("Disconnected!.");    
}

// Motor control functions
void forward() {
  digitalWrite(enableA, HIGH);
  digitalWrite(enableB, HIGH);
  digitalWrite(leftB, LOW);
  digitalWrite(rightB, LOW);
  digitalWrite(leftF, HIGH);
  digitalWrite(rightF, HIGH);
  Serial.println("Forward");
}

void backward() {
  digitalWrite(enableA, HIGH);
  digitalWrite(enableB, HIGH);
  digitalWrite(leftF, LOW);
  digitalWrite(rightF, LOW);
  digitalWrite(leftB, HIGH);
  digitalWrite(rightB, HIGH);
  Serial.println("Backward");
}

void stopCar() {
  digitalWrite(enableA, LOW);
  digitalWrite(enableB, LOW);
  digitalWrite(leftF, LOW);
  digitalWrite(rightF, LOW);
  digitalWrite(leftB, LOW);
  digitalWrite(rightB, LOW);
  Serial.println("Stopped");
}

void driftLeft() {
  digitalWrite(enableA, HIGH);
  digitalWrite(enableB, HIGH);
  digitalWrite(leftF, LOW);
  digitalWrite(rightB, LOW);
  digitalWrite(leftB, HIGH);
  digitalWrite(rightF, HIGH);
  Serial.println("Drift Left");
}


void driftRight() {
  digitalWrite(enableA, HIGH);
  digitalWrite(enableB, HIGH);
  digitalWrite(leftB, LOW);
  digitalWrite(rightF, LOW);
  digitalWrite(leftF, HIGH);
  digitalWrite(rightB, HIGH);
  Serial.println("Drift Right");
}

void TURN_LEFT() {
  digitalWrite(enableA, HIGH);
  digitalWrite(enableB, HIGH);
  digitalWrite(leftF, LOW);
  digitalWrite(rightB, LOW);
  digitalWrite(leftB, LOW);
  digitalWrite(rightF, HIGH);
  Serial.println("LEFT");
}

void TURN_RIGHT() {
  digitalWrite(enableA, HIGH);
  digitalWrite(enableB, HIGH);
  digitalWrite(leftF, HIGH);
  digitalWrite(rightB, LOW);
  digitalWrite(leftB, LOW);
  digitalWrite(rightF, LOW);
  Serial.println("RIGHT");
}


void setup() 
{
  Serial.begin(115200);
  Serial.println("ESP32 Bot Ready...");
  // Motor pin setup
  pinMode(enableA, OUTPUT);
  pinMode(enableB, OUTPUT);
  pinMode(leftF, OUTPUT);
  pinMode(leftB, OUTPUT);
  pinMode(rightF, OUTPUT);
  pinMode(rightB, OUTPUT);

  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  // Setup the Bluepad32 callbacks
  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.enableVirtualDevice(false);
  
  
  stopCar(); // Start in stopped state
 
}


void loop() 
{
  bool dataUpdated = BP32.update();
  if (dataUpdated){
    processControllers();
  }
}