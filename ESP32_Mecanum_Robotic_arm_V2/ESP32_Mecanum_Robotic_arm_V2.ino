#include <Bluepad32.h>

ControllerPtr myControllers[BP32_MAX_GAMEPADS];

#include <ESP32Servo.h>

const int L_Shoulder = 15; // Left_Shoulder
const int R_Shoulder = 4;  // Right_Shoulder
const int L_Hand = 5;      // Left_Hand
const int R_Hand = 18;     // Right_Hand
const int BASE = 19;       // Base/HIP

Servo servoL_Shoulder;
Servo servoR_Shoulder;
Servo servoL_Hand;
Servo servoR_Hand;
Servo servoBASE;

int baseAngle = 90;  // Initial neutral position
int step = 5;        // Degrees to move per press

uint16_t lastButtonState = 0;

#include <vector>  
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define UP_LEFT 5
#define UP_RIGHT 6
#define DOWN_LEFT 7
#define DOWN_RIGHT 8
#define TURN_LEFT 9
#define TURN_RIGHT 10
#define STOP 0

#define FRONT_RIGHT_MOTOR 0
#define BACK_RIGHT_MOTOR 1
#define FRONT_LEFT_MOTOR 2
#define BACK_LEFT_MOTOR 3

#define FORWARD 1
#define BACKWARD -1

struct MOTOR_PINS
{
  int pinIN1;
  int pinIN2;    
};

std::vector<MOTOR_PINS> motorPins = 
{
  {13, 12},  //FRONT_RIGHT_MOTOR
  {32, 33},  //BACK_RIGHT_MOTOR
  {27, 14},  //FRONT_LEFT_MOTOR
  {25, 26},  //BACK_LEFT_MOTOR   
};

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
  "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, L2: %4d, R2: %4d, "
  "misc: 0x%02x\n",
  ctl->index(),        // Controller Index
  ctl->dpad(),         // D-pad
  ctl->buttons(),      // bitmask of pressed buttons
  ctl->axisX(),        // (-511 - 512) left X Axis
  ctl->axisY(),        // (-511 - 512) left Y axis
  ctl->axisRX(),       // (-511 - 512) right X axis
  ctl->axisRY(),       // (-511 - 512) right Y axis
  ctl->brake(),        // (0 - 1023): brake button L2
  ctl->throttle(),     // (0 - 1023): throttle (AKA gas) button R2
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

void processCarMovement(int inputValue)
{
  switch(inputValue)
  {

    case UP:
      rotateMotor(FRONT_RIGHT_MOTOR, FORWARD);
      rotateMotor(BACK_RIGHT_MOTOR, FORWARD);
      rotateMotor(FRONT_LEFT_MOTOR, FORWARD);
      rotateMotor(BACK_LEFT_MOTOR, FORWARD);                 
      break;
  
    case DOWN:
      rotateMotor(FRONT_RIGHT_MOTOR, BACKWARD);
      rotateMotor(BACK_RIGHT_MOTOR, BACKWARD);
      rotateMotor(FRONT_LEFT_MOTOR, BACKWARD);
      rotateMotor(BACK_LEFT_MOTOR, BACKWARD);  
      break;
  
    case LEFT:
      rotateMotor(FRONT_RIGHT_MOTOR, FORWARD);
      rotateMotor(BACK_RIGHT_MOTOR, BACKWARD);
      rotateMotor(FRONT_LEFT_MOTOR, BACKWARD);
      rotateMotor(BACK_LEFT_MOTOR, FORWARD);  
      break;
  
    case RIGHT:
      rotateMotor(FRONT_RIGHT_MOTOR, BACKWARD);
      rotateMotor(BACK_RIGHT_MOTOR, FORWARD);
      rotateMotor(FRONT_LEFT_MOTOR, FORWARD);
      rotateMotor(BACK_LEFT_MOTOR, BACKWARD); 
      break;
  
    case UP_LEFT:
      rotateMotor(FRONT_RIGHT_MOTOR, FORWARD);
      rotateMotor(BACK_RIGHT_MOTOR, STOP);
      rotateMotor(FRONT_LEFT_MOTOR, STOP);
      rotateMotor(BACK_LEFT_MOTOR, FORWARD);  
      break;
  
    case UP_RIGHT:
      rotateMotor(FRONT_RIGHT_MOTOR, STOP);
      rotateMotor(BACK_RIGHT_MOTOR, FORWARD);
      rotateMotor(FRONT_LEFT_MOTOR, FORWARD);
      rotateMotor(BACK_LEFT_MOTOR, STOP);  
      break;
  
    case DOWN_LEFT:
      rotateMotor(FRONT_RIGHT_MOTOR, STOP);
      rotateMotor(BACK_RIGHT_MOTOR, BACKWARD);
      rotateMotor(FRONT_LEFT_MOTOR, BACKWARD);
      rotateMotor(BACK_LEFT_MOTOR, STOP);   
      break;
  
    case DOWN_RIGHT:
      rotateMotor(FRONT_RIGHT_MOTOR, BACKWARD);
      rotateMotor(BACK_RIGHT_MOTOR, STOP);
      rotateMotor(FRONT_LEFT_MOTOR, STOP);
      rotateMotor(BACK_LEFT_MOTOR, BACKWARD);   
      break;
  
    case TURN_LEFT:
      rotateMotor(FRONT_RIGHT_MOTOR, FORWARD); // used a wrong logic to work on bot
      rotateMotor(BACK_RIGHT_MOTOR, BACKWARD);
      rotateMotor(FRONT_LEFT_MOTOR, FORWARD);
      rotateMotor(BACK_LEFT_MOTOR, BACKWARD);
      break;
  
    case TURN_RIGHT:
      rotateMotor(FRONT_RIGHT_MOTOR, BACKWARD); // used a wrong logic to work on bot
      rotateMotor(BACK_RIGHT_MOTOR, FORWARD);
      rotateMotor(FRONT_LEFT_MOTOR, BACKWARD);
      rotateMotor(BACK_LEFT_MOTOR, FORWARD);    
      break;
  
    case STOP:
      rotateMotor(FRONT_RIGHT_MOTOR, STOP);
      rotateMotor(BACK_RIGHT_MOTOR, STOP);
      rotateMotor(FRONT_LEFT_MOTOR, STOP);
      rotateMotor(BACK_LEFT_MOTOR, STOP);   
      break;
  
    default:
      rotateMotor(FRONT_RIGHT_MOTOR, STOP);
      rotateMotor(BACK_RIGHT_MOTOR, STOP);
      rotateMotor(FRONT_LEFT_MOTOR, STOP);
      rotateMotor(BACK_LEFT_MOTOR, STOP);    
      break;
  }
}

void setUpPinModes()
{
  for (int i = 0; i < motorPins.size(); i++)
  {
    pinMode(motorPins[i].pinIN1, OUTPUT);
    pinMode(motorPins[i].pinIN2, OUTPUT);  
    rotateMotor(i, STOP);  
  }
}

void processGamepad(ControllerPtr ctl) {

  if ( ctl->axisX() < -75 && ctl->axisY() > 75)
  {
    processCarMovement(DOWN_LEFT);    
  }
  else if ( ctl->axisX() > 75 && ctl->axisY() > 75)
  {
    processCarMovement(DOWN_RIGHT);    
  } 
  else if ( ctl->axisX() < -75 && ctl->axisY() < -75)
  {
    processCarMovement(UP_LEFT);    
  }
  else if ( ctl->axisX() > 75 && ctl->axisY() < -75)
  {
    processCarMovement(UP_RIGHT);    
  }  
  else if (ctl->axisRX() > 75)
  { 
    processCarMovement(TURN_RIGHT);
    ctl->playDualRumble(0 /* delayedStartMs */, 500 /* durationMs */, 0x10 /* weakMagnitude */,
                            0x10 /* strongMagnitude */);
  }
  else if (ctl->axisRX() < -75)
  {
    processCarMovement(TURN_LEFT);
    ctl->playDualRumble(0 /* delayedStartMs */, 500 /* durationMs */, 0x10 /* weakMagnitude */,
                            0x10 /* strongMagnitude */);
  }
  else if (ctl->axisY() > 75)
  {
    processCarMovement(DOWN);  
  }
  else if (ctl->axisY() < -75)
  {
    processCarMovement(UP);     
  }
  else if (ctl->axisX() > 75)
  {
    processCarMovement(RIGHT);   
  }

  else if (ctl->axisX() < -75)
  {
    processCarMovement(LEFT);    
  } 
  else
  {
    processCarMovement(STOP);     
  }

  uint16_t currentButtonState = ctl->buttons();

  if (ctl->buttons()==0x0010) {
    baseAngle += step;
    if (baseAngle > 180) baseAngle = 180; // Limit max angle
     servoL_Shoulder.write(baseAngle);
    delay(100); // Debounce delay
  }
  if (ctl->buttons()==0x0020) {
    baseAngle += step;
    if (baseAngle > 180) baseAngle = 180; // Limit max angle
     servoL_Shoulder.write(baseAngle);
    delay(100); // Debounce delay
  }


  if (ctl->dpad() == 0x01) {
    baseAngle += step;
    if (baseAngle > 180) baseAngle = 180; // Limit max angle
    servoL_Hand.write(baseAngle);
    delay(100); // Debounce delay
  }
  if (ctl->dpad() == 0x02) {
    baseAngle -= step;
    if (baseAngle > 180) baseAngle = 180; // Limit max angle
    servoL_Hand.write(baseAngle);
    delay(100); // Debounce delay
  }

  if (ctl->dpad() == 0x08) {
    baseAngle += step;
    if (baseAngle > 180) baseAngle = 180; // Limit max angle
    servoR_Shoulder.write(baseAngle);
    delay(100); // Debounce delay
  }
  if (ctl->dpad() == 0x04) {
    baseAngle -= step;
    if (baseAngle > 180) baseAngle = 180; // Limit max angle
    servoR_Shoulder.write(baseAngle);
    delay(100); // Debounce delay
  }




  // Rotate hip left/right with X/B buttons (same as before) controlling index 4

  if (ctl->x()) {
    baseAngle += step;
    if (baseAngle > 180) baseAngle = 180;
    servoBASE.write(baseAngle);

    delay(100);
  }
  if (ctl->b()) {
    baseAngle -= step;
    if (baseAngle < 0) baseAngle = 0;
    servoBASE.write(baseAngle);
    delay(100);
  }
  
   if (ctl->a()) {
    baseAngle += step;
    if (baseAngle > 180) baseAngle = 180;
    servoR_Hand.write(baseAngle);
    delay(100);
  }
  if (ctl->y()) {
    baseAngle -= step;
    if (baseAngle < 0) baseAngle = 0;
    servoR_Hand.write(baseAngle);
    delay(100);
  }

  dumpGamepad(ctl);
}

void onConnect()
{
  Serial.println("Connected!.");
}

void onDisConnect()
{
  processCarMovement(STOP);
  Serial.println("Disconnected!.");    
}

void setup() 
{
  setUpPinModes();
  servoL_Shoulder.attach(L_Shoulder);
  servoR_Shoulder.attach(R_Shoulder);
  servoL_Hand.attach(L_Hand);
  servoR_Hand.attach(R_Hand);
  servoBASE.attach(BASE);

  
  Serial.begin(115200);
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  // Setup the Bluepad32 callbacks
  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.enableVirtualDevice(false);
}

void loop() 
{
  bool dataUpdated = BP32.update();
  if (dataUpdated){
    processControllers();
  }
}
