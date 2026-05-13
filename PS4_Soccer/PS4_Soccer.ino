#include <Bluepad32.h>
#include <ESP32Servo.h>
#include <Arduino.h>
#include <vector>



#define UP 1
#define DOWN 2
#define TURN_LEFT 4
#define TURN_RIGHT 3
#define STOP 0

#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1


#define FORWARD 1
#define BACKWARD -1

struct MOTOR_PINS
{
  int pinIN1;
  int pinIN2;    
};

MOTOR_PINS motorPins[] =
{
  {26, 25},  // LEFT_MOTOR
  {14, 27}   // RIGHT_MOTOR
};

#define MOTOR_COUNT 2

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


void processGamepad(ControllerPtr ctl) {

  if (ctl->axisY() > 75)
  {  
    rotateMotor(RIGHT_MOTOR, BACKWARD);
    rotateMotor(LEFT_MOTOR, BACKWARD);
    Serial.println("DOWN");              
  }
  else if (ctl->axisY() < -75)
  {
    rotateMotor(RIGHT_MOTOR, FORWARD);
    rotateMotor(LEFT_MOTOR, FORWARD);
    Serial.println("UP");    
  }
  else if (ctl->axisX() > 75)
  {
    rotateMotor(LEFT_MOTOR, FORWARD);
    rotateMotor(RIGHT_MOTOR, BACKWARD);
    Serial.println("LEFT");  
  }
  else if (ctl->axisX() < -75)
  {
    rotateMotor(RIGHT_MOTOR, FORWARD);
    rotateMotor(LEFT_MOTOR, BACKWARD); 
    Serial.println("RIGHT"); 
  }


  else if (ctl->dpad()== 01)
  {
    rotateMotor(RIGHT_MOTOR, FORWARD);
    rotateMotor(LEFT_MOTOR, FORWARD);
    Serial.println("UP");    
  } 
  else if (ctl->dpad()== 02)
  {
    rotateMotor(RIGHT_MOTOR, BACKWARD);
    rotateMotor(LEFT_MOTOR, BACKWARD);
    Serial.println("DOWN");               
  } 
  else if (ctl->dpad()== 8)
  {
    rotateMotor(RIGHT_MOTOR, FORWARD);
    rotateMotor(LEFT_MOTOR, BACKWARD); 
    Serial.println("RIGHT");     
  } 
  else if (ctl->dpad()== 04)
  {
    rotateMotor(LEFT_MOTOR, FORWARD);
    rotateMotor(RIGHT_MOTOR, BACKWARD);
    Serial.println("LEFT"); 
  } 
  else
  {
    rotateMotor(RIGHT_MOTOR, STOP);
    rotateMotor(LEFT_MOTOR, STOP);      
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
  
  if (ctl->a()) { 

  }

  if (ctl->b()) { 

  }
  
  if (ctl->y()) { 

  }
  
  if (ctl->buttons()==0x0010) { 

  }
  
  if (ctl->buttons()==0x0020) { 

  }
  

  if (ctl->brake()>10)
  {
    ctl->playDualRumble(0 /* delayedStartMs */, 500 /* durationMs */, 0x10 /* weakMagnitude */,
                            0x10 /* strongMagnitude */);
    rotateMotor(RIGHT_MOTOR, BACKWARD);
    rotateMotor(LEFT_MOTOR, BACKWARD);
    Serial.println("DOWN");  
  }

  if (ctl->throttle()>10)
  {
    ctl->playDualRumble(0 /* delayedStartMs */, 500 /* durationMs */, 0x10 /* weakMagnitude */,
                            0x10 /* strongMagnitude */);
    rotateMotor(RIGHT_MOTOR, FORWARD);
    rotateMotor(LEFT_MOTOR, FORWARD);
    Serial.println("UP");     
  }
  dumpGamepad(ctl);
}

void rotateMotor(int motorNumber, int motorDirection)
{
  if (motorDirection == FORWARD)
  {
    digitalWrite(motorPins[motorNumber].pinIN1, HIGH);
    digitalWrite(motorPins[motorNumber].pinIN2, LOW);    
  }
  else if (motorDirection == BACKWARD)
  {
    digitalWrite(motorPins[motorNumber].pinIN1, LOW);
    digitalWrite(motorPins[motorNumber].pinIN2, HIGH);     
  }
  else
  {
    digitalWrite(motorPins[motorNumber].pinIN1, LOW);
    digitalWrite(motorPins[motorNumber].pinIN2, LOW);       
  }
}

void setUpPinModes()
{
  for (int i = 0; i < MOTOR_COUNT; i++)
  {
    pinMode(motorPins[i].pinIN1, OUTPUT);
    pinMode(motorPins[i].pinIN2, OUTPUT);  
    rotateMotor(i, STOP);  
  }
}


void setup() {

  setUpPinModes();
  Serial.begin(115200);
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  // Setup the Bluepad32 callbacks
  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.enableVirtualDevice(false);

}

void loop() {
  bool dataUpdated = BP32.update();
  if (dataUpdated){
    processControllers();
  }
}