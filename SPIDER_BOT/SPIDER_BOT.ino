//  --------                 --------
// |  D9    |               |  D7    |
// | joint9 |               | joint7 |
//  ----- --------     --------- -----
//       |  D8    |   |  D6    |
//       | joint8 |   | joint6 |
//        --------     --------
//       |  D2    |  |   D4    |
//       | joint2 |  |  joint4 |
//  ----- --------    --------- -----
// |  D3    |               |  D5    |
// | joint3 |               | joint5 |
//  --------                 --------
//                Front

  // joint2.attach(4);  IO1
  // joint3.attach(39); IO2

  // joint4.attach(13); IO3
  // joint5.attach(38); IO4

  // joint6.attach(14); IO5
  // joint7.attach(48); IO6

  // joint8.attach(42); IO7
  // joint9.attach(5);  IO8


#include <ESP32Servo.h>

// Servo declarations
Servo joint2;  // Right front hip
Servo joint3;  // Right front leg
Servo joint4;  // Left front hip
Servo joint5;  // Left front leg
Servo joint6;  // Left back hip
Servo joint7;  // Left back leg
Servo joint8;  // Right back hip
Servo joint9;  // Right back leg

// Home positions
int home_joint2 = 170; 
int home_joint3 = 0;
int home_joint4 = 10;
int home_joint5 = 50;
int home_joint6 = 150;
int home_joint7 = 150;
int home_joint8 = 60;
int home_joint9 = 180;

// ----------------------
// Basic Movements
// ----------------------
void idle(){
  delay(100);
}

void standhome(){
  joint2.write(home_joint2);
  joint3.write(home_joint3);
  joint4.write(home_joint4);
  joint5.write(home_joint5);
  joint6.write(home_joint6);
  joint7.write(home_joint7);
  joint8.write(home_joint8);
  joint9.write(home_joint9);
}

void sithome(){
  joint2.write(home_joint2);
  joint3.write(65);
  joint4.write(home_joint4);
  joint5.write(110);
  joint6.write(home_joint6);
  joint7.write(60);
  joint8.write(home_joint8);
  joint9.write(100);
}

void twist(){
  joint3.write(home_joint3);
  joint5.write(home_joint5);
  joint7.write(home_joint7);
  joint9.write(home_joint9);
  
  for(int right=90; right<170; right++){
      joint2.write(right);
      joint6.write(right);
      joint4.write(right-90);
      joint8.write(right-90);
      delay(10);
  }

  for(int right=170; right>90; right--){
      joint2.write(right);
      joint6.write(right);
      joint4.write(right-90);
      joint8.write(right-90);
      delay(10);
  }
}

void wink(unsigned int step){
  standhome();
  joint9.write(home_joint9-80);
  delay(200);
  joint7.write(20);

  while (step-- > 0){
    joint4.write(home_joint4 + 60);
    delay(300);
    joint4.write(home_joint4 - 60);
    delay(300);
  }
}

// Forward
void forward(unsigned int step){
  while (step-- > 0){
    joint3.write(home_joint3+30);
    joint7.write(home_joint7+30);
    delay(100);
    joint2.write(home_joint2+30);
    joint8.write(home_joint8-30);
    joint4.write(home_joint4);
    joint6.write(home_joint6);
    delay(100);
    joint3.write(home_joint3);
    joint7.write(home_joint7);
    idle();
    
    joint5.write(home_joint5-30);
    joint9.write(home_joint9-30);
    delay(100);
    joint2.write(home_joint2);
    joint8.write(home_joint8);
    joint4.write(home_joint4-30);
    joint6.write(home_joint6+30);
    delay(100);
    joint5.write(home_joint5);
    joint9.write(home_joint9);
    idle();
  }
}

// Backward
void backward(unsigned int step){
  while (step-- > 0){
    joint3.write(home_joint3+30);
    joint7.write(home_joint7+30);
    delay(100);
    joint2.write(home_joint2);
    joint8.write(home_joint8);
    joint4.write(home_joint4-30);
    joint6.write(home_joint6+30);
    delay(100);
    joint3.write(home_joint3);
    joint7.write(home_joint7);
    idle();
    
    joint5.write(home_joint5-30);
    joint9.write(home_joint9-30);
    delay(100);
    joint2.write(home_joint2+30);
    joint8.write(home_joint8-30);
    joint4.write(home_joint4);
    joint6.write(home_joint6);
    delay(100);
    joint5.write(home_joint5);
    joint9.write(home_joint9);
    idle();
  }
}

// Right turn
void rightturn(unsigned int step){
  while (step-- > 0){
    joint5.write(home_joint5-30);
    joint9.write(home_joint9-30);
    delay(100);
    joint2.write(home_joint2+30);
    joint8.write(home_joint8-30);
    joint4.write(home_joint4-30);
    joint6.write(home_joint6+30);
    delay(100);
    joint5.write(home_joint5);
    joint9.write(home_joint9);
    idle();
    
    joint3.write(home_joint3+30);
    joint7.write(home_joint7+30);
    delay(100);
    joint2.write(home_joint2);
    joint8.write(home_joint8);
    joint4.write(home_joint4);
    joint6.write(home_joint6);
    delay(100);
    joint3.write(home_joint3);
    joint7.write(home_joint7);
    idle();
  }
}

void downaction(unsigned int step){
  while (step-- > 0){
  sithome();
  delay(100);
  standhome();
  delay(100);
  }
}

// Left turn
void leftturn(unsigned int step){
  while (step-- > 0){
    joint3.write(home_joint3+30);
    joint7.write(home_joint7+30);
    delay(100);
    joint2.write(home_joint2+30);
    joint8.write(home_joint8-30);
    joint4.write(home_joint4-30);
    joint6.write(home_joint6+30);
    delay(100);
    joint3.write(home_joint3);
    joint7.write(home_joint7);
    idle();
    
    joint5.write(home_joint5-30);
    joint9.write(home_joint9-30);
    delay(100);
    joint2.write(home_joint2);
    joint8.write(home_joint8);
    joint4.write(home_joint4);
    joint6.write(home_joint6);
    delay(100);
    joint5.write(home_joint5);
    joint9.write(home_joint9);
    idle();
  }
}

// ----------------------
// Setup
// ----------------------
void setup() {
  Serial.begin(115200);

  joint2.attach(4);
  joint3.attach(39);
  joint4.attach(13);
  joint5.attach(48);
  joint6.attach(14);
  joint7.attach(38);
  joint8.attach(42);
  joint9.attach(5);

  standhome();
}

// ----------------------
// Main Loop
// ----------------------
void loop() {
  // Every 3 seconds, run each function

  standhome();
  delay(3000);
  
  downaction(2);
  delay(3000);

  twist();
  delay(3000);

  wink(4);
  delay(3000);

  sithome();
  delay(3000);

  forward(4);
  delay(3000);

  backward(4);
  delay(3000);

  leftturn(4);
  delay(3000);

  rightturn(4);
  delay(3000);

  standhome();
  delay(3000);
}

