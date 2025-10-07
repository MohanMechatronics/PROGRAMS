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

#include <vector>  
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define I2C_SDA 21
#define I2C_SCL 22
TwoWire I2Cbus = TwoWire(0);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2Cbus, OLED_RESET);

#include "eyes.h"

int xp=16;
int mood=1;


void processGamepad(ControllerPtr ctl) {

  if (ctl->axisY() > 75)
  {
    RobotWalkBackward();  
  }
  else if (ctl->axisY() < -75)
  {
    RobotWalkForward();
    mood = 3;     
  }
  else if (ctl->axisX() > 75)
  {
    RobotWalkRight();
  }
  else if (ctl->axisX() < -75)
  {
    RobotWalkLeft(); 
  }
  else if (ctl->dpad()== 01)
  {
    RobotWalkForward(); 
  } 
  else if (ctl->dpad()== 02)
  {
    RobotWalkBackward(); 
  } 
  else if (ctl->dpad()== 8)
  {
    RobotWalkLeft();  
  } 
  else if (ctl->dpad()== 04)
  {
    RobotWalkRight();  
  } 
  else
  {
    RobotWalkStop();  
    mood = 1; 
  }





  if (ctl->axisRX() > 75)
  {
    RobotRollLeft();
  }
  else if (ctl->axisRX() < -75)
  {
    RobotRollRight();
  }
  else if (ctl->axisRY() > 75)
  {
    RobotRollBackward();
  }
  else if (ctl->axisRY() < -75)
  {
    RobotRollForward();
  }
  else
  {
    RobotRollStop(); 
    mood = 1; 
  }

  if (ctl->a()) { // x
        RobotSetRoll();
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
  
  if (ctl->x())  RobotRollStop(); // Square
  if (ctl->b())  HomePosition(); //cirle
  if (ctl->y())  PUNCHUP(); // tringle


  if (ctl->brake()>10)
  {
    ctl->playDualRumble(0 /* delayedStartMs */, 500 /* durationMs */, 0x10 /* weakMagnitude */,
                            0x10 /* strongMagnitude */);

    int brakeValue = ctl->brake();
    int servoPos = map(brakeValue, 0, 1024, 0, 180);
    servoPos = constrain(servoPos, 0, 180);
    myservoLeftArm.write(servoPos);

    mood = 5;  
  }

  if (ctl->throttle()>10)
  {
    ctl->playDualRumble(0 /* delayedStartMs */, 500 /* durationMs */, 0x10 /* weakMagnitude */,
                            0x10 /* strongMagnitude */);   
    RightPunch();
    mood = 4;
  }
  
  if (ctl->buttons()==0x0010) { 

  }
  
  if (ctl->buttons()==0x0020) { 
    
  }
  

  dumpGamepad(ctl);
}
//RED
// int ServoLeftFootPin   = 19;     
// int ServoLeftAnklePin  = 13;     
// int ServoRightFootPin  = 23;      
// int ServoRightAnklePin = 12;      
// int ServoLeftArmPin    = 14;     
// int ServoRightArmPin   = 5;     
// int ServoHeadPin       = 18;      

//BLUE
int ServoLeftFootPin   = 32;     
int ServoLeftAnklePin  = 12;     
int ServoRightFootPin  = 33;      
int ServoRightAnklePin = 26;      
int ServoLeftArmPin    = 14;     
int ServoRightArmPin   = 27;     
int ServoHeadPin       = 13;  


Servo myservoLeftFoot;
Servo myservoLeftAnkle;
Servo myservoRightFoot;
Servo myservoRightAnkle;
Servo myservoLeftArm;
Servo myservoRightArm;
Servo myservoHead;


///////////// CALIBRATION SETTINGS: /////////////////


////////// 1: Forward walking rotation Speed

// Left foot forward walking rotation Speed
int LFFWRS= 20;    // 0 = Slowest  90 = Fastest    Default = 20

// Right foot forward walking rotation Speed
int RFFWRS= 20;   // 0 = Slowest   90 = Fastest    Default = 20



////////// 2: Backward walking rotation Speed

// Left foot Backward walking rotation Speed
int LFBWRS= 20;   // 0 = Slowest   90 = Fastest    Default = 20

// Right foot Backward walking rotation Speed
int RFBWRS= 20;   // 0 = Slowest   90 = Fastest    Default = 20



////////// 3: Ankle standing Position

// Left Ankle standing Position
int LA0= 60;    // 0 = Full Tilt Right   180 = Full Tilt Left    Default = 60

// Right Ankle standing position
int RA0= 120;   // 0 = Full Tilt Right   180 = Full Tilt Left    Default = 120



////////// 4: Ankle roll Position

// Left Ankle roll Position
int LA1= 180;   // 0 = Full Tilt Right   180 = Full Tilt Left    Default = 170

// Right Ankle roll position
int RA1= 0;    // 0 = Full Tilt Right   180 = Full Tilt Left     Default = 10



////////// 5: Ankle tilt left walking position

// Left Ankle tilt left walking position
int LATL= 90;   // 0 = Full Tilt Right   180 = Full Tilt Left    Default = 90   

// Right Ankle tilt left walking position
int RATL= 180;  // 0 = Full Tilt Right   180 = Full Tilt Left    Default = 180   



////////// 6: Ankle tilt right walking position

// Left Ankle tilt right walking position
int LATR= 0;   // 0 = Full Tilt Right   180 = Full Tilt Left     Default = 0   

// Right Ankle tilt right walking position
int RATR= 90;  // 0 = Full Tilt Right   180 = Full Tilt Left     Default = 90  




/////////////////////////////////////////////////////////////////////////






void setup()
{
  Serial.begin(115200);
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  // Setup the Bluepad32 callbacks
  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.enableVirtualDevice(false);

  HomePosition();

  I2Cbus.begin(I2C_SDA, I2C_SCL, 100000);
  Serial.println("Booting");

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
}




void loop() 
{
  bool dataUpdated = BP32.update();
  if (dataUpdated){
    processControllers();
  }

  int n;
  static int xd=0;
  static int espera=0;
  static int step=0;
  int x1,x2;
  if (espera>0) {
    espera--;
    delay(1);
  } else {
    x1=   xd+ (xp>16? (16+2*(xp-16)):xp);
    x2=64+xd+ (xp<16? (-16+(xp*2))  :xp);
    switch (step){
      case 0:
       display.clearDisplay(); // Clear the display buffer
       if (xp<6) { 
               display.drawBitmap(x1, 8, peyes[mood][2][0], 32, 32, WHITE);
               display.drawBitmap(x2, 8, peyes[mood][1][1], 32, 32, WHITE);
       } else if (xp<26) {
               display.drawBitmap(x1, 8, peyes[mood][0][0], 32, 32, WHITE);
               display.drawBitmap(x2, 8, peyes[mood][0][1], 32, 32, WHITE); 
       } else {
               display.drawBitmap(x1, 8, peyes[mood][1][0], 32, 32, WHITE); 
               display.drawBitmap(x2, 8, peyes[mood][2][1], 32, 32, WHITE);
       }
       display.display();
       espera=random(250, 1000);
       n=random(0,7);
       if (n==6) {
          step=1;
       } else {
          step=2;
       }
       break;
      case 1:
       display.clearDisplay(); // Clear the display buffer
       display.drawBitmap(x1, 8, eye0, 32, 32, WHITE);
       display.drawBitmap(x2, 8, eye0, 32, 32, WHITE);
       display.display();
       espera=100;
       step=0;
       break;
      case 2:
       n=random(0,10);
       if (n<5) xd--;
       if (n>5) xd++;
       if (xd<-4) xd=-3;
       if (xd>4) xd=3; 
       espera=0;
       step=0;
       break;
    }
  }

}


void HomePosition()
{
    myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400);
    myservoRightFoot.attach(ServoRightFootPin, 544, 2400);  
    myservoLeftAnkle.attach(ServoLeftAnklePin, 544, 2400);
    myservoRightAnkle.attach(ServoRightAnklePin, 544, 2400); 
    myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
    myservoRightArm.attach(ServoRightArmPin, 544, 2400);
    myservoHead.attach(ServoHeadPin, 544, 2400);
  
    myservoLeftFoot.write(90);
    myservoRightFoot.write(90);  
    myservoLeftAnkle.write(60);
    myservoRightAnkle.write(120); 
    myservoLeftArm.write(180);
    myservoRightArm.write(0); 
    myservoHead.write(90);
    
    delay(500);
    
    myservoLeftFoot.detach();
    myservoRightFoot.detach();  
    myservoLeftAnkle.detach();
    myservoRightAnkle.detach();
    myservoLeftArm.detach();
    myservoRightArm.detach();
    myservoHead.detach();
}




void RobotSetWalk()
{    
    myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
    myservoRightArm.attach(ServoRightArmPin, 544, 2400);
    myservoLeftArm.write(90);
    myservoRightArm.write(90); 
    delay(200);
    myservoLeftArm.detach();
    myservoRightArm.detach();

    
    myservoLeftAnkle.attach(ServoLeftAnklePin, 544, 2400);
    myservoRightAnkle.attach(ServoRightAnklePin, 544, 2400);                                                                          
    myservoLeftAnkle.write(LA0); 
    myservoRightAnkle.write(RA0); 
    delay(300);
    myservoLeftAnkle.detach();
    myservoRightAnkle.detach();

    myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
    myservoRightArm.attach(ServoRightArmPin, 544, 2400);
    myservoLeftArm.write(180);
    myservoRightArm.write(0); 
    delay(200);
    myservoLeftArm.detach();
    myservoRightArm.detach();
}   






void RobotSetRoll()
{  
    myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
    myservoRightArm.attach(ServoRightArmPin, 544, 2400);
    myservoLeftArm.write(90);
    myservoRightArm.write(90); 
    delay(200);
    myservoLeftArm.detach();
    myservoRightArm.detach();
    
    myservoLeftAnkle.attach(ServoLeftAnklePin, 544, 2400);
    myservoRightAnkle.attach(ServoRightAnklePin, 544, 2400);                                                                          
    myservoLeftAnkle.write(LA1); 
    myservoRightAnkle.write(RA1); 
    delay(300);
    myservoLeftAnkle.detach();
    myservoRightAnkle.detach();
    
    myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
    myservoRightArm.attach(ServoRightArmPin, 544, 2400);
    myservoLeftArm.write(180);
    myservoRightArm.write(0); 
    delay(200);
    myservoLeftArm.detach();
    myservoRightArm.detach();      
}






void RobotWalkStop()
{
    myservoLeftFoot.write(90);
    myservoRightFoot.write(90);  
    myservoLeftAnkle.write(LA0); 
    myservoRightAnkle.write(RA0); 
  
}


void RobotRollStop()
{
    myservoLeftFoot.write(90);
    myservoRightFoot.write(90);  
    myservoLeftFoot.detach();
    myservoRightFoot.detach();  
}


void RobotWalkForward()
{
   myservoLeftAnkle.attach(ServoLeftAnklePin, 544, 2400);
   myservoRightAnkle.attach(ServoRightAnklePin, 544, 2400);
   
   myservoLeftAnkle.write(LATR); 
   myservoRightAnkle.write(RATR);
   delay(300);
   
   myservoRightFoot.attach(ServoRightFootPin, 544, 2400);  
   myservoRightFoot.write(90-RFFWRS);
   delay(300);
   myservoRightFoot.detach();
   
   myservoLeftAnkle.write(LATL); 
   myservoRightAnkle.write(RATL);
   delay(300);
   
   myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400); 
   myservoLeftFoot.write(90+LFFWRS);      
   delay(300);
   myservoLeftFoot.detach();  
}


void RobotWalkBackward()
{
   myservoLeftAnkle.attach(ServoLeftAnklePin, 544, 2400);
   myservoRightAnkle.attach(ServoRightAnklePin, 544, 2400);
   
   myservoLeftAnkle.write(LATR); 
   myservoRightAnkle.write(RATR);
   delay(300);
   
   myservoRightFoot.attach(ServoRightFootPin, 544, 2400);  
   myservoRightFoot.write(90+RFBWRS);
   delay(300);
   myservoRightFoot.detach();
   
   myservoLeftAnkle.write(LATL); 
   myservoRightAnkle.write(RATL);
   delay(300);
   
   myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400); 
   myservoLeftFoot.write(90-LFBWRS);      
   delay(300);
   myservoLeftFoot.detach();  
}








void RobotWalkLeft()
{
   myservoLeftAnkle.attach(ServoLeftAnklePin, 544, 2400);
   myservoRightAnkle.attach(ServoRightAnklePin, 544, 2400);
   
   myservoLeftAnkle.write(LATR); 
   myservoRightAnkle.write(RATR);
   delay(300);
   
   myservoRightFoot.attach(ServoRightFootPin, 544, 2400);  
   myservoRightFoot.write(90-RFFWRS);
   delay(100);
   myservoRightFoot.detach();
   
   myservoLeftAnkle.write(LATL); 
   myservoRightAnkle.write(RATL);
   delay(300);
   
   myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400); 
   myservoLeftFoot.write(90+LFFWRS);      
   delay(500);
   myservoLeftFoot.detach();  
}








void RobotWalkRight()
{
   myservoLeftAnkle.attach(ServoLeftAnklePin, 544, 2400);
   myservoRightAnkle.attach(ServoRightAnklePin, 544, 2400);
   
   myservoLeftAnkle.write(LATR); 
   myservoRightAnkle.write(RATR);
   delay(300);
   
   myservoRightFoot.attach(ServoRightFootPin, 544, 2400);  
   myservoRightFoot.write(90-RFFWRS);
   delay(500);
   myservoRightFoot.detach();
   
   myservoLeftAnkle.write(LATL); 
   myservoRightAnkle.write(RATL);
   delay(300);
   
   myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400); 
   myservoLeftFoot.write(90+LFFWRS);      
   delay(100);
   myservoLeftFoot.detach();  
}








void RobotRollForward()
{ 
  myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400);
  myservoRightFoot.attach(ServoRightFootPin, 544, 2400);  
    
  myservoLeftFoot.write(180);
  myservoRightFoot.write(0);   
}   








void RobotRollBackward()
{ 
  myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400);
  myservoRightFoot.attach(ServoRightFootPin, 544, 2400);  
    
  myservoLeftFoot.write(0);
  myservoRightFoot.write(180);   
}         




void RobotRollLeft()
{ 
  myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400);
  myservoRightFoot.attach(ServoRightFootPin, 544, 2400);  
    
  myservoLeftFoot.write(0);
  myservoRightFoot.write(0);   
}   




void RobotRollRight()
{ 
  myservoLeftFoot.attach(ServoLeftFootPin, 544, 2400);
  myservoRightFoot.attach(ServoRightFootPin, 544, 2400);  
    
  myservoLeftFoot.write(180);
  myservoRightFoot.write(180);   
}  




void LeftPunch()
{
  myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
      
  myservoLeftArm.write(0);   
  delay(300);    
  myservoLeftArm.write(180);   
  delay(300);
  
  myservoLeftArm.detach();
}     



void RightPunch()
{
  myservoRightArm.attach(ServoRightArmPin, 544, 2400);
      
  myservoRightArm.write(180);   
  delay(300);    
  myservoRightArm.write(0);   
  delay(300);
  
  myservoRightArm.detach();
}     

void PUNCHUP()
{
  myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
  myservoRightArm.attach(ServoRightArmPin, 544, 2400);
  myservoLeftArm.write(90);
  myservoRightArm.write(90); 
  delay(200);
  myservoLeftArm.detach();
  myservoRightArm.detach();

  myservoLeftArm.attach(ServoLeftArmPin, 544, 2400);
  myservoRightArm.attach(ServoRightArmPin, 544, 2400);
  myservoLeftArm.write(180);
  myservoRightArm.write(0); 
  delay(200);
  myservoLeftArm.detach();
  myservoRightArm.detach();

}
