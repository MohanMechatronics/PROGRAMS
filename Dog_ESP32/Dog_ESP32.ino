#include <ESP32Servo.h>
#include <Bluepad32.h>

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

Servo Leg1F;
Servo Leg1B;
Servo Leg2F;
Servo Leg2B;
Servo Leg3F;
Servo Leg3B;
Servo Leg4F;
Servo Leg4B;
Servo Headservo;

boolean stringComplete = false;
boolean stopcmd = false;
String inputString;

int selservo;
int rotate1;
int rotate2;
int rotate3;
int rotate4;

float LALeg1F;
float LALeg1B;
float LALeg2F;
float LALeg2B;
float LALeg3F;
float LALeg3B;
float LALeg4F;
float LALeg4B;
float LAHeadservo;

boolean smoothrun;
int smoothdelay;
int maxstep = 0;
int totalforwardsteps = 50;
int totalsidesteps = 25;
int moveforwardsteps = 0;
int movesidesteps = 0;
int forwardsteps = 0;

float stepLeg1F;
float stepLeg1B;
float stepLeg2F;
float stepLeg2B;
float stepLeg3F;
float stepLeg3B;
float stepLeg4F;
float stepLeg4B;

int TOLeg1F;
int TOLeg1B;
int TOLeg2F;
int TOLeg2B;
int TOLeg3F;
int TOLeg3B;
int TOLeg4F;
int TOLeg4B;
int TOHeadservo;


int walkF[][7] = { { 124, 146, 177, 150, 132, 115, 115 }, { 94, 132, 178, 139, 112, 84, 84 }, { 37, 112, 179, 139, 95, 42, 42 }, { 22, 95, 150, 115, 78, 30, 30 }, { 11, 78, 124, 92, 59, 13, 13 }, { 13, 59, 92, 58, 36, 2, 2 } };
int walkB[][7] = { { 3, 34, 56, 65, 48, 30, 30 }, { 2, 48, 86, 96, 68, 41, 41 }, { 1, 68, 143, 138, 85, 41, 41 }, { 30, 85, 158, 150, 102, 65, 65 }, { 56, 102, 169, 167, 121, 88, 88 }, { 88, 121, 167, 178, 144, 122, 122 } };

int walkH[] = { 3, 4, 5, 6, 7, 8 };
int Fheight;
int Bheight;
int heightchange;

int walkstep;
int walkstep2;

int Twalkstep;
int Twalkstep2;

String S1;
String S2;
String Splitstr;
int Sstr1;
int Sstr2;
int findcomma;
int previouscomma;
String reccmd;
String othercmd;

void dumpGamepad(ControllerPtr ctl) {
  Serial.printf(
    "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, misc: 0x%02x\n",
    ctl->index(), ctl->dpad(), ctl->buttons(), ctl->axisX(), ctl->axisY(),
    ctl->axisRX(), ctl->axisRY(), ctl->brake(), ctl->throttle(), ctl->miscButtons());
}

// Gamepad input handler
void processGamepad(ControllerPtr ctl) {
  if (ctl->axisY() > 75) {
    reccmd= "B";
  }
  else if (ctl->axisY() < -75) {
    reccmd = "F";
  }
  else if (ctl->axisX() > 75) {
    reccmd = "R";
  }
  else if (ctl->axisX() < -75) {
    reccmd = "L";
  }
  else if (ctl->dpad() == 0x01) {
   reccmd = "B";
  }
  else if (ctl->dpad() == 0x02) {
    reccmd= "F";
  }
  else if (ctl->dpad() == 0x08) {
    reccmd = "L";
  }
  else if (ctl->dpad() == 0x04) {
    reccmd = "R";
  }
  else{
    reccmd = "S";
    Leg1F.write(80);
    Leg1B.write(100);
    Leg2F.write(100);
    Leg2B.write(80);
    Leg3F.write(80);
    Leg3B.write(100);
    Leg4F.write(100);
    Leg4B.write(80);
  }

  if (ctl->a()) { 
    sithome();
  }                      // X 

  // if (ctl->x()) reccmd = "I"; // Square
  // if (ctl->b()) reccmd = "H";//cirle
  // if (ctl->y()) reccmd = "J";// tringle
  if (ctl->brake() > 10)  changeheight();
  if (ctl->throttle() > 10) sayhai();
  
  if (ctl->buttons()==0x0010) { 
      reccmd == "p";
  }
  
  if (ctl->buttons()==0x0020) { 
      othercmd == "O";
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

  LALeg1F = 80;
  LALeg1B = 100;
  LALeg2F = 100;
  LALeg2B = 80;
  LALeg3F = 80;
  LALeg3B = 100;
  LALeg4F = 100;
  LALeg4B = 80;
  LAHeadservo = 90;

  TOLeg1F = LALeg1F;
  TOLeg1B = LALeg1B;
  TOLeg2F = LALeg2F;
  TOLeg2B = LALeg2B;
  TOLeg3F = LALeg3F;
  TOLeg3B = LALeg3B;
  TOLeg4F = LALeg4F;
  TOLeg4B = LALeg4B;
  TOHeadservo = LAHeadservo;

  Leg1F.attach(13);
  Leg1B.attach(12);
  Leg2F.attach(14);
  Leg2B.attach(27);
  Leg3F.attach(26);
  Leg3B.attach(25);
  Leg4F.attach(33);
  Leg4B.attach(32);
  
  Headservo.attach(15);
  delay(1000);
  Servomovement();

  Serial.begin(115200);
  walkstep = 1;
  Fheight = 5;
  Bheight = 5;
  reccmd = "S";
  smoothrun = true;
  smoothdelay = 2;
  delay(2000);
}

void loop() {
  Serial.println(reccmd);
  bool dataUpdated = BP32.update();
  if (dataUpdated){
    processControllers();
  }
  if (reccmd == "F" || reccmd == "B" || reccmd == "L" || reccmd == "R" || reccmd == "G" || reccmd == "I" || reccmd == "H" || reccmd == "J") {
    if (reccmd == "F" || reccmd == "L" || reccmd == "G" || reccmd == "I") {
      walkstep = walkstep + 1;
      if (walkstep > 7) {
        walkstep = 1;
      }
      walkstep2 = walkstep + 3;
      if (walkstep2 > 7) {
        walkstep2 = walkstep2 - 7;
      }
    } else if (reccmd == "B" || reccmd == "R" || reccmd == "H" || reccmd == "J") {
      walkstep = walkstep - 1;
      if (walkstep < 1) {
        walkstep = 7;
      }
      walkstep2 = walkstep - 4;
      if (walkstep2 < 1) {
        walkstep2 = walkstep2 + 7;
      }
    }

    if (reccmd == "F" || reccmd == "B") {

      rotate1 = walkF[Fheight][walkstep - 1];
      rotate2 = walkB[Fheight][walkstep - 1];
      rotate3 = walkF[Bheight][walkstep - 1];
      rotate4 = walkB[Bheight][walkstep - 1];

      TOLeg1F = rotate1;
      TOLeg1B = rotate2;
      TOLeg4F = 180 - rotate3;
      TOLeg4B = 180 - rotate4;
      Servomovement();

      rotate1 = walkF[Fheight][walkstep2 - 1];
      rotate2 = walkB[Fheight][walkstep2 - 1];
      rotate3 = walkF[Bheight][walkstep2 - 1];
      rotate4 = walkB[Bheight][walkstep2 - 1];

      TOLeg2F = 180 - rotate1;
      TOLeg2B = 180 - rotate2;
      TOLeg3F = rotate3;
      TOLeg3B = rotate4;
      Servomovement();

    } else if (reccmd == "G" || reccmd == "I" || reccmd == "H" || reccmd == "J") {
      if (reccmd == "I" and walkstep >= 4) {
        rotate1 = walkF[Fheight][4];
        rotate2 = walkB[Fheight][4];
      } else {
        rotate1 = walkF[Fheight][walkstep - 1];
        rotate2 = walkB[Fheight][walkstep - 1];
      }

      if (reccmd == "H" and walkstep >= 4) {
        rotate3 = walkF[Bheight][4];
        rotate4 = walkB[Bheight][4];
      } else {
        rotate3 = walkF[Bheight][walkstep - 1];
        rotate4 = walkB[Bheight][walkstep - 1];
      }

      TOLeg1F = rotate1;
      TOLeg1B = rotate2;
      TOLeg4F = 180 - rotate3;
      TOLeg4B = 180 - rotate4;
      Servomovement();

      if (reccmd == "G" and walkstep2 >= 4) {
        rotate1 = walkF[Fheight][4];
        rotate2 = walkB[Fheight][4];
      } else {
        rotate1 = walkF[Fheight][walkstep2 - 1];
        rotate2 = walkB[Fheight][walkstep2 - 1];
      }

      if (reccmd == "J" and walkstep2 >= 4) {
        rotate3 = walkF[Bheight][4];
        rotate4 = walkB[Bheight][4];
      } else {
        rotate3 = walkF[Bheight][walkstep2 - 1];
        rotate4 = walkB[Bheight][walkstep2 - 1];
      }

      TOLeg2F = 180 - rotate1;
      TOLeg2B = 180 - rotate2;
      TOLeg3F = rotate3;
      TOLeg3B = rotate4;
      Servomovement();

    } else if (reccmd == "L" || reccmd == "R") {
      rotate1 = walkF[Fheight][walkstep - 1];
      rotate2 = walkB[Fheight][walkstep - 1];
      rotate3 = walkF[Bheight][walkstep - 1];
      rotate4 = walkB[Bheight][walkstep - 1];

      TOLeg1F = rotate1;
      TOLeg1B = rotate2;
      TOLeg4F = rotate4;
      TOLeg4B = rotate3;
      Servomovement();

      rotate1 = walkF[Fheight][(8 - walkstep) - 1];
      rotate2 = walkB[Fheight][(8 - walkstep) - 1];
      rotate3 = walkF[Bheight][(8 - walkstep) - 1];
      rotate4 = walkB[Bheight][(8 - walkstep) - 1];

      TOLeg2F = 180 - rotate1;
      TOLeg2B = 180 - rotate2;
      TOLeg3F = 180 - rotate4;
      TOLeg3B = 180 - rotate3;
      Servomovement();
    }

    delay(100);

    if (stopcmd == true) {
      if (walkstep == 4) {
        reccmd = "S";
        stopcmd = false;
      }
    }
    if (heightchange != 0 && walkstep == 4) {
      changeheight();
    }
  } else if (heightchange != 0) {
    changeheight();
  } else if (reccmd == "C") {
    smoothdelay = 8;
    selfcheck();
    smoothdelay = 2;
    reccmd = "S";
  } else if (reccmd == "V") {
    sayhai();
    reccmd = "S";
  }
}

void sithome(){
  Leg1F.write(80);
  Leg1B.write(100);
  Leg2F.write(100);
  Leg2B.write(80);
  Leg3F.write(80);
  Leg3B.write(100);
  Leg4F.write(100);
  Leg4B.write(80);
}

void sayhai() {
  TOLeg1F = 0;
  TOLeg1B = 180;
  TOLeg2F = 180;
  TOLeg2B = 0;
  TOLeg3F = 80;
  TOLeg3B = 100;
  TOLeg4F = 100;
  TOLeg4B = 80;
  TOHeadservo = 90;
  Servomovement();

  for (int i = 1; i <= 5; i++) {
    delay(500);
    TOLeg1F = 60;
    TOHeadservo = 135;
    Servomovement();

    delay(500);
    TOLeg1F = 100;
    TOHeadservo = 45;
    Servomovement();
  }
  TOLeg1F = 0;
  TOHeadservo = 90;
  Servomovement();
}

void selfcheck() {
  TOLeg1F = 0;
  TOLeg1B = 180;
  TOLeg2F = 180;
  TOLeg2B = 0;
  TOLeg3F = 0;
  TOLeg3B = 180;
  TOLeg4F = 180;
  TOLeg4B = 0;
  TOHeadservo = 90;
  Servomovement();

  delay(500);
  TOLeg1F = 180;
  TOLeg1B = 0;
  Servomovement();

  delay(500);
  TOLeg1F = 0;
  TOLeg1B = 180;
  Servomovement();

  delay(500);
  TOLeg2F = 0;
  TOLeg2B = 180;
  Servomovement();

  delay(500);
  TOLeg2F = 180;
  TOLeg2B = 0;
  Servomovement();

  delay(500);
  TOLeg3F = 180;
  TOLeg3B = 0;
  Servomovement();

  delay(500);
  TOLeg3F = 0;
  TOLeg3B = 180;
  Servomovement();

  delay(500);
  TOLeg4F = 0;
  TOLeg4B = 180;
  Servomovement();

  delay(500);
  TOLeg4F = 180;
  TOLeg4B = 0;
  Servomovement();

  TOHeadservo = 0;
  Servomovement();
  delay(100);
  TOHeadservo = 180;
  Servomovement();
  delay(100);
  TOHeadservo = 90;
  Servomovement();
  delay(100);
}

void changeheight() {
  if (othercmd == "O") {
    Fheight = 5;
    Bheight = 5;
  } else if (othercmd == "P") {
    Fheight = 0;
    Bheight = 0;
  }

  if (othercmd == "D" || othercmd == "U" || othercmd == "W" || othercmd == "Y") {
    Fheight = Fheight + heightchange;
    if (Fheight > 5) {
      Fheight = 5;
    }
    if (Fheight < 0) {
      Fheight = 0;
    }
  }
  if (othercmd == "D" || othercmd == "U" || othercmd == "X" || othercmd == "Z") {
    Bheight = Bheight + heightchange;
    if (Bheight > 5) {
      Bheight = 5;
    }
    if (Bheight < 0) {
      Bheight = 0;
    }
  }
  heightchange = 0;

  rotate1 = walkF[Fheight][4];
  rotate2 = walkB[Fheight][4];
  TOLeg1F = rotate1;
  TOLeg1B = rotate2;
  TOLeg2F = 180 - rotate1;
  TOLeg2B = 180 - rotate2;

  rotate1 = walkF[Bheight][4];
  rotate2 = walkB[Bheight][4];
  TOLeg3F = rotate1;
  TOLeg3B = rotate2;
  TOLeg4F = 180 - rotate1;
  TOLeg4B = 180 - rotate2;
  Servomovement();
}


void Servomovement() {
  if (smoothrun == true) {
    smoothmove();
  }

  LALeg1F = TOLeg1F;
  LALeg1B = TOLeg1B;
  LALeg2F = TOLeg2F;
  LALeg2B = TOLeg2B;
  LALeg3F = TOLeg3F;
  LALeg3B = TOLeg3B;
  LALeg4F = TOLeg4F;
  LALeg4B = TOLeg4B;
  LAHeadservo = TOHeadservo;

  Leg1F.write(TOLeg1F);
  Leg1B.write(TOLeg1B);
  Leg2F.write(TOLeg2F);
  Leg2B.write(TOLeg2B);
  Leg3F.write(TOLeg3F);
  Leg3B.write(TOLeg3B);
  Leg4F.write(TOLeg4F);
  Leg4B.write(TOLeg4B);
  Headservo.write(TOHeadservo);
}

void smoothmove() {
  maxstep = 0;
  if (abs(LALeg1F - TOLeg1F) > maxstep) {
    maxstep = abs(LALeg1F - TOLeg1F);
  }
  if (abs(LALeg1B - TOLeg1B) > maxstep) {
    maxstep = abs(LALeg1B - TOLeg1B);
  }
  if (abs(LALeg2F - TOLeg2F) > maxstep) {
    maxstep = abs(LALeg2F - TOLeg2F);
  }
  if (abs(LALeg2B - TOLeg2B) > maxstep) {
    maxstep = abs(LALeg2B - TOLeg2B);
  }
  if (abs(LALeg3F - TOLeg3F) > maxstep) {
    maxstep = abs(LALeg3F - TOLeg3F);
  }
  if (abs(LALeg3B - TOLeg3B) > maxstep) {
    maxstep = abs(LALeg3B - TOLeg3B);
  }
  if (abs(LALeg4F - TOLeg4F) > maxstep) {
    maxstep = abs(LALeg4F - TOLeg4F);
  }
  if (abs(LALeg4B - TOLeg4B) > maxstep) {
    maxstep = abs(LALeg4B - TOLeg4B);
  }
  if (maxstep > 0) {
    stepLeg1F = (TOLeg1F - LALeg1F) / maxstep;
    stepLeg1B = (TOLeg1B - LALeg1B) / maxstep;
    stepLeg2F = (TOLeg2F - LALeg2F) / maxstep;
    stepLeg2B = (TOLeg2B - LALeg2B) / maxstep;
    stepLeg3F = (TOLeg3F - LALeg3F) / maxstep;
    stepLeg3B = (TOLeg3B - LALeg3B) / maxstep;
    stepLeg4F = (TOLeg4F - LALeg4F) / maxstep;
    stepLeg4B = (TOLeg4B - LALeg4B) / maxstep;
    for (int i = 0; i <= maxstep; i += 1) {
      LALeg1F = LALeg1F + stepLeg1F;
      LALeg1B = LALeg1B + stepLeg1B;
      LALeg2F = LALeg2F + stepLeg2F;
      LALeg2B = LALeg2B + stepLeg2B;
      LALeg3F = LALeg3F + stepLeg3F;
      LALeg3B = LALeg3B + stepLeg3B;
      LALeg4F = LALeg4F + stepLeg4F;
      LALeg4B = LALeg4B + stepLeg4B;
      Leg1F.write(LALeg1F);
      Leg1B.write(LALeg1B);
      Leg2F.write(LALeg2F);
      Leg2B.write(LALeg2B);
      Leg3F.write(LALeg3F);
      Leg3B.write(LALeg3B);
      Leg4F.write(LALeg4F);
      Leg4B.write(LALeg4B);
      delay(smoothdelay);
    }
  }

  if (LAHeadservo > TOHeadservo) {
    for (int i = LAHeadservo; i >= TOHeadservo; i -= 1) {
      LAHeadservo = i;
      Headservo.write(LAHeadservo);
      delay(smoothdelay);
    }
  } else {
    for (int i = LAHeadservo; i <= TOHeadservo; i += 1) {
      LAHeadservo = i;
      Headservo.write(LAHeadservo);
      delay(smoothdelay);
    }
  }
}

