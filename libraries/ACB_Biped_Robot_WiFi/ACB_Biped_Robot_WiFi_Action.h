/*
         ---------------
        |     O   O     |
        |---------------|
YR 18==> |             | <== YL 5   180-0   180-0
         ---------------
            ||     ||
            ||     ||
RR 17==> ------   ------ <== RL 16  180-0  180-0
         |-----   -----|
*/
#include <ESP32Servo.h>

Servo servo_16; //右下
Servo servo_17; //左下
Servo servo_18; //左上
Servo servo_5;  //右上

const int PWMRES_Min = 0; // PWM Resolution 1
const int PWMRES_Max = 180; // PWM Resolution 180

const int SERVOMIN = 500; // 400
const int SERVOMAX = 2500; // 2400

const int ALLMATRIX = 5; // GPIO16 + GPI17 + GPI18 + GPIO2 + Run Time
const int ALLSERVOS = 4; // GPIO16 + GPI17 + GPI18 + GPIO2
// Backup servo value
int Running_Servo_POS [ALLMATRIX];
// Servo delay base time
int BASEDELAYTIME = 10; // 10 ms

void Set_PWM_to_Servo(int iServo, int iValue);


const int Servo_Act_0 [ ] PROGMEM = {   90,  90,  90,  90,  500  };   // YL RL YR RR

// Stop
const int Servo_Prg_0_Step = 1;
const int Servo_Prg_0 [][ALLMATRIX] PROGMEM = {
  {90, 90, 90, 90, 300},    // YL RL YR RR
};

// Forward
const int Servo_Prg_1_Step = 4;
const int Servo_Prg_1 [][ALLMATRIX] PROGMEM = {

  {70, 110, 70, 120, 300},
  {60, 90, 60, 90, 300},

  {110, 50, 110, 60, 300},
  {105, 90, 105, 90, 300},


};

// Backward 
const int Servo_Prg_2_Step = 4;
const int Servo_Prg_2 [][ALLMATRIX] PROGMEM = {

  {120, 75, 110, 110, 300},
  {90, 100, 110, 75, 300},

  {60, 75, 65, 90, 300},   
  {60, 90, 60, 90, 300}, 

};

// Leftward 
const int Servo_Prg_3_Step = 6;
const int Servo_Prg_3 [][ALLMATRIX] PROGMEM = {

  {90, 60, 125, 90, 300},
  {60, 60, 125, 90, 300},
  {60, 90, 125, 90, 300},

  {60, 120, 55, 120, 300},
  {60, 90, 55, 90, 300},

  {80, 90, 90, 90, 150},

  // {70, 110, 70, 120, 300},
  // {60, 90, 60, 90, 300},

  // {110, 60, 140, 60, 300},
  // {105, 90, 135, 90, 300},
  

};

// Rightward 
const int Servo_Prg_4_Step = 6;
const int Servo_Prg_4 [][ALLMATRIX] PROGMEM = {
  {50, 90, 90, 120, 300}, 
  {50, 90, 130, 120, 300}, 
  {50, 90, 130, 90, 300},

  {120, 50, 130, 60, 300},
  {120, 90, 130, 90, 300},

  {90, 90, 80, 90, 150},

  // {110, 70, 130, 70, 300},
  // {120, 90, 140, 90, 300},

  // {60, 120, 110, 130, 300},
  // {65, 90, 100, 90, 300},
};

// shadows_step
const int Servo_Prg_10_Step = 12;
const int Servo_Prg_10 [][ALLMATRIX] PROGMEM = {
  {75, 105, 68, 110, 600},
  {75, 90, 60, 90, 600},

  {75, 68, 110, 68, 600},
  {100, 90, 110, 90, 600},

  {60, 120, 60, 120, 200},
  {60, 90, 60, 90, 200},

  {110, 60, 110, 60, 200},
  {110, 90, 110, 90, 200},

  {60, 110, 60, 110, 200},
  {60, 90, 60, 90, 200},

  {110, 60, 110, 60, 200},
  {110, 90, 110, 90, 200},
};

// rub 
const int Servo_Prg_11_Step = 4;
const int Servo_Prg_11 [][ALLMATRIX] PROGMEM = {
  // {120, 90, 120, 90, 500},
  // {60, 90, 60, 90, 500},

  // {120, 90, 120, 90, 500},
  // {60, 90, 60, 90, 500},

  {90, 65, 90, 95, 500},
  {40, 65, 90, 95, 300},
  {150, 65, 90, 95, 300},

  {90, 90, 90, 90, 300},



  // {90, 80, 90, 100, 500},
  // {90, 80, 150, 100, 300},
  // {90, 80, 40, 100, 300},

  // {90, 90, 90, 90, 300},
  
};

// swing  
const int Servo_Prg_12_Step = 4;
const int Servo_Prg_12 [][ALLMATRIX] PROGMEM = {
  // {90, 90, 90, 120, 500},
  // {90, 90, 90, 90, 500},

  // {90, 65, 90, 90, 500},
  // {90, 90, 90, 90, 500},

  // {90, 90, 90, 120, 500},
  // {90, 90, 90, 90, 500},

  // {90, 65, 90, 90, 500},
  // {90, 90, 90, 90, 500},

  {90, 80, 90, 100, 500},
  {90, 80, 150, 100, 300},
  {90, 80, 40, 100, 300},

  {90, 90, 90, 90, 300},


};

// left_tilt 
const int Servo_Prg_13_Step = 10;
const int Servo_Prg_13 [][ALLMATRIX] PROGMEM = {

  {90, 90, 82, 70, 500},

  {90, 135, 80, 105, 500},

  {90, 163, 50, 120, 500},


  {90, 163, 90, 130, 500},

  {90, 90, 90, 130, 500},

  {90, 163, 90, 130, 500},


  {90, 163, 50, 120, 500},
  {90, 158, 60, 120, 500},
  
  {90, 135, 80, 105, 500},

  {90, 90, 82, 70, 500},

  // {90, 145, 80, 110, 500},

  // {120, 145, 60, 120, 500},

  // {90, 145, 60, 135, 500},

  // {90, 145, 90, 135, 500},

  // {90, 90, 90, 135, 500},

  // {90, 145, 90, 135, 500},

  // {90, 145, 75, 135, 500},

  // {120, 145, 50, 105, 500},

  // {90, 130, 80, 100, 500},

  // {90, 80, 90, 90, 500},

};

// right_tilt

const int Servo_Prg_14_Step = 10;
const int Servo_Prg_14 [][ALLMATRIX] PROGMEM = {

  

  {96, 110, 90, 90, 500},

  {100, 75, 90, 45, 500},

  {130, 60, 90, 20, 500},

  {90, 50, 90, 20, 500},

  {90, 50, 90, 90, 500},

  {90, 50, 90, 20, 500},

  {130, 60, 90, 20, 500},

  {120, 60, 90, 25, 500},

  {100, 75, 90, 45, 500},

  {96, 110, 90, 90, 500},

  // {100, 70, 90, 45, 500},

  // {130, 50, 60, 45, 500},

  // {130, 50, 60, 45, 500},

  // {90, 50, 60, 45, 500},       

  // {90, 50, 90, 45, 500},      //mid   

  // {90, 50, 60, 45, 500},  

  // {130, 50, 60, 45, 500},

  // {98, 60, 90, 45, 500},

  // {90, 90, 90, 80, 500},

};


// rise 左跺脚
const int Servo_Prg_15_Step = 8;
const int Servo_Prg_15 [][ALLMATRIX] PROGMEM = {
  {90, 100, 92, 95, 300},

  {90, 60, 92, 85, 300},
  {90, 120, 92, 95, 300},
  {90, 60, 92, 85, 300},
  {90, 120, 92, 95, 300},
  {90, 60, 92, 85, 300},
  {90, 120, 92, 95, 300},

  {90, 90, 90, 90, 300},

};

// dance
const int Servo_Prg_16_Step = 24;
const int Servo_Prg_16 [][ALLMATRIX] PROGMEM = {

  {75, 140, 90, 70, 500},
  {75, 55, 90, 55, 500},
  {75, 90, 90, 90, 500},
  {75, 140, 90, 70, 500},
  {75, 55, 90, 55, 500},
  {75, 90, 90, 90, 500}, 

  {75, 140, 90, 70, 500},
  {75, 55, 90, 55, 500},
  {75, 90, 90, 90, 500},
  {75, 140, 90, 70, 500},
  {75, 55, 90, 55, 500},
  {75, 90, 90, 90, 500}, 

  //
  {75, 110, 90, 45, 500},
  {75, 125, 90, 105, 500},
  {75, 90, 90, 90, 500},
  {75, 110, 90, 45, 500},
  {75, 125, 90, 105, 500},
  {75, 90, 90, 90, 500},

  {75, 110, 90, 45, 500},
  {75, 125, 90, 105, 500},
  {75, 90, 90, 90, 500},
  {75, 110, 90, 45, 500},
  {75, 125, 90, 105, 500},
  {75, 90, 90, 90, 500},

  // {75, 130, 90, 80, 300},
  // {75, 130, 90, 50, 300},
  // {75, 90, 90, 75, 300},
  // {75, 80, 90, 95, 300},
  // // {75, 130, 90, 120, 300},
  // {75, 90, 90, 90, 300},

  // {75, 130, 90, 80, 300},
  // {75, 130, 90, 50, 300},
  // {75, 90, 90, 75, 300},
  // {75, 80, 90, 95, 300},
  // // {75, 130, 90, 120, 300},
  // {75, 90, 90, 90, 300},

  // {75, 130, 90, 80, 300},
  // {75, 130, 90, 50, 300},
  // {75, 90, 90, 75, 300},
  // {75, 80, 90, 95, 300},
  // // {75, 130, 90, 120, 300},
  // {75, 90, 90, 90, 300},

  // // right
  // {75, 100, 105, 50, 300},
  // {75, 130, 105, 50, 300},
  // {75, 115, 105, 90, 300},
  // {75, 85, 105, 130, 300},
  // {75, 60, 105, 50, 300},
  // {75, 90, 105, 90, 300},

  // {75, 100, 105, 50, 300},
  // {75, 130, 105, 50, 300},
  // {75, 115, 105, 90, 300},
  // {75, 85, 105, 130, 300},
  // {75, 60, 105, 50, 300},
  // {75, 90, 105, 90, 300},

  // {75, 100, 105, 50, 300},
  // {75, 130, 105, 50, 300},
  // {75, 115, 105, 90, 300},
  // {75, 85, 105, 130, 300},
  // {75, 60, 105, 50, 300},
  // {75, 90, 105, 90, 300},

  // {90, 130, 90, 80, 300},
  // {90, 130, 90, 50, 300},
  // {90, 90, 90, 75, 300},
  // {90, 50, 90, 70, 300},
  // {90, 130, 90, 110, 300},
  // {90, 90, 90, 90, 300},

  // {90, 130, 90, 80, 300},
  // {90, 130, 90, 50, 300},
  // {90, 90, 90, 75, 300},
  // {90, 50, 90, 70, 300},
  // {90, 130, 90, 110, 300},
  // {90, 90, 90, 90, 300},

  // {90, 130, 90, 80, 300},
  // {90, 130, 90, 50, 300},
  // {90, 90, 90, 75, 300},
  // {90, 50, 90, 70, 300},
  // {90, 130, 90, 110, 300},
  // {90, 90, 90, 90, 300},

 
  // //left foot support 0-15
  // {110, 90, 120, 90, 300},
  // {110, 80, 120, 65, 300},
  // {110, 80, 100, 65, 300},
  // {110, 80, 120, 65, 300},
  // {110, 80, 100, 65, 300},
  // {110, 80, 120, 65, 300},
  // {110, 80, 100, 65, 300},

  // {85, 120, 90, 90, 200},
  // {85, 105, 90, 90, 200},
  // {85, 90, 90, 90, 200},
  // {85, 75, 90, 90, 200},
  // {85, 60, 90, 90, 200},
  // {85, 75, 90, 90, 200},
  // {85, 90, 90, 90, 200},
  // {85, 105, 90, 90, 200},

  // {90, 90, 90, 90, 200},

  // //right foot support 16-31
  // {60, 90, 70, 90, 300},
  // {60, 115, 70, 95, 300},
  // {80, 115, 70, 95, 300},
  // {60, 115, 70, 95, 300},
  // {80, 115, 70, 95, 300},
  // {60, 115, 70, 95, 300},
  // {80, 115, 70, 95, 300},

  // {90, 90, 95, 60, 200},
  // {90, 90, 95, 75, 200},
  // {90, 90, 95, 90, 200},
  // {90, 90, 95, 105, 200},
  // {90, 90, 95, 120, 200},
  // {90, 90, 95, 105, 200},
  // {90, 90, 95, 90, 200},
  // {90, 90, 95, 75, 200},

  // {90, 90, 90, 90, 200},
 


};

// test1 左摇腿
const int Servo_Prg_17_Step = 11;
const int Servo_Prg_17 [][ALLMATRIX] PROGMEM = {
    {50,90,80,90, 300},
    {50,130,80,100, 300}, 
    {80,130,80,100, 300},
    {60,130,80,100, 300}, 
    {80,130,80,100, 300},

    {60,130,80,100, 300}, 
    {80,130,80,100, 300},
    {60,130,80,100, 300}, 
    {80,130,80,100, 300},
    {60,110,80,105, 300},

    {90,90,90,90, 300}, 

};

// test2 右跺脚
const int Servo_Prg_18_Step = 8;
const int Servo_Prg_18 [][ALLMATRIX] PROGMEM = {
  {90, 80, 90, 100, 300},

  {90, 95, 90, 60, 300},
  {90, 75, 90, 120, 300},
  {90, 95, 90, 60, 300},
  {90, 75, 90, 120, 300},
  {90, 95, 90, 60, 300},
  {90, 75, 90, 120, 300},

  {90, 90, 90, 90, 300},
  

};

// test3 右摇腿
const int Servo_Prg_19_Step = 11 ;
const int Servo_Prg_19 [][ALLMATRIX] PROGMEM = {
    {100,90,125,90, 300},
    {100,75,125,50, 300}, 
    {100,75,100,50, 300},
    {100,75,120,50, 300}, 
    {100,75,100,50, 300},

    {100,75,120,50, 300}, 
    {100,75,100,50, 300},
    {100,75,120,50, 300}, 
    {100,75,100,50, 300},

    {100,90,120,70, 300},

    // {100,80,120,60, 300},
    // {100,90,120,60, 300},
    // {100,80,120,60, 300},  

    // {100,90,120,60, 300},
    // {100,80,120,60, 300},
    // {100,90,120,60, 300},
    // {100,60,120,60, 300},    
    // {100,90,120,90, 300},

    {90,90,90,90, 300}, 

};



void Servo_PROGRAM_Zero() {  //servo initialize
	for (int Index = 0; Index < ALLMATRIX; Index++)  {
		Running_Servo_POS[Index] = Servo_Act_0[Index];
	}
  for (int iServo = 0; iServo < ALLSERVOS; iServo++) {
		Set_PWM_to_Servo(iServo, Running_Servo_POS[iServo]);
		delay(10);
	}
}

void Set_PWM_to_Servo(int iServo, int iValue){  //servo move
  int NewPWM = map(iValue, PWMRES_Min, PWMRES_Max, SERVOMIN, SERVOMAX);
	if (iServo >= 3) {
		servo_17.write(NewPWM);
	} else if (iServo >= 2) {
		servo_18.write(NewPWM);
	} else if (iServo >= 1) {
		servo_16.write(NewPWM);
	} else if (iServo == 0) {
		servo_5.write(NewPWM);
	}
}

void Servo_PROGRAM_Run(const int iMatrix[][ALLMATRIX], int iSteps) {
    int INT_TEMP_A, INT_TEMP_B, INT_TEMP_C;

    for (int MainLoopIndex = 0; MainLoopIndex < iSteps; MainLoopIndex++) // iSteps 步骤主循环
    {
        int InterTotalTime = iMatrix[MainLoopIndex][ALLMATRIX - 1]; // InterTotalTime 此步骤总时间

        int InterDelayCounter = InterTotalTime / BASEDELAYTIME; // InterDelayCounter 此步骤基本延迟次数

        for (int InterStepLoop = 0; InterStepLoop < InterDelayCounter; InterStepLoop++) // 内插值循环
        {
            for (int ServoIndex = 0; ServoIndex < ALLSERVOS; ServoIndex++) // 舵机主循环
            {
                INT_TEMP_A = Running_Servo_POS[ServoIndex]; // 舵机现在的位置
                INT_TEMP_B = iMatrix[MainLoopIndex][ServoIndex]; // 舵机的目标位置
 
                if (INT_TEMP_A == INT_TEMP_B) // 舵机数值不变
                {
                    INT_TEMP_C = INT_TEMP_B;
                } 
                else if (INT_TEMP_A > INT_TEMP_B) // 舵机数值减少
                {
                    INT_TEMP_C =  map(BASEDELAYTIME * InterStepLoop, 0, InterTotalTime, 0, INT_TEMP_A - INT_TEMP_B); // PWM內差值 = map(执行次数时间累加, 开始时间, 結束时间, 內差起始值, 內差最大值)
                    if (INT_TEMP_A - INT_TEMP_C >= INT_TEMP_B) 
                    {
                        Set_PWM_to_Servo(ServoIndex, INT_TEMP_A - INT_TEMP_C);
                    }
                } 
                else if (INT_TEMP_A < INT_TEMP_B) // 舵机数值增加
                {
                  INT_TEMP_C =  map(BASEDELAYTIME * InterStepLoop, 0, InterTotalTime, 0, INT_TEMP_B - INT_TEMP_A); // PWM內差值 = map(执行次数时间累加, 开始时间, 結束时间, 內差起始值, 內差最大值)
                  if (INT_TEMP_A + INT_TEMP_C <= INT_TEMP_B) {
                    Set_PWM_to_Servo(ServoIndex, INT_TEMP_A + INT_TEMP_C);
                    }
                }
            }

            delay(BASEDELAYTIME);
        }

        // 备份当前舵机数值
        for (int Index = 0; Index < ALLMATRIX; Index++) 
        {
            Running_Servo_POS[Index] = iMatrix[MainLoopIndex][Index];
        }
    }
}

void stop() {
  Servo_PROGRAM_Run(Servo_Prg_0, Servo_Prg_0_Step);
}

void forward() {
  Servo_PROGRAM_Run(Servo_Prg_1, Servo_Prg_1_Step);
}

void backward() {
  Servo_PROGRAM_Run(Servo_Prg_2, Servo_Prg_2_Step);
}

void leftward() {
  Servo_PROGRAM_Run(Servo_Prg_3, Servo_Prg_3_Step);
}

void rightward() {
  Servo_PROGRAM_Run(Servo_Prg_4, Servo_Prg_4_Step);
}

void sprint() {
  Servo_PROGRAM_Run(Servo_Prg_10, Servo_Prg_10_Step);
}

void left_kick() {
  Servo_PROGRAM_Run(Servo_Prg_11, Servo_Prg_11_Step);
}

void right_kick() {
  Servo_PROGRAM_Run(Servo_Prg_12, Servo_Prg_12_Step);
}

void left_tilt() {
  Servo_PROGRAM_Run(Servo_Prg_13, Servo_Prg_13_Step);
}

void right_tilt() {
  Servo_PROGRAM_Run(Servo_Prg_14, Servo_Prg_14_Step);
}

void left_stamp() {
  Servo_PROGRAM_Run(Servo_Prg_15, Servo_Prg_15_Step);
}

void dance() {
  Servo_PROGRAM_Run(Servo_Prg_16, Servo_Prg_16_Step);
}