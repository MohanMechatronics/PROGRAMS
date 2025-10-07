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


#define Trig_PIN            13
#define Echo_PIN            14       

int UT_distance = 0;
int middleDistance = 0;

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


void Ultrasonic_Init()
{
    pinMode(Trig_PIN, OUTPUT);
    pinMode(Echo_PIN, INPUT);
}

float Ranging(int Trig, int Echo) 
{
    digitalWrite(Trig, LOW);
    delayMicroseconds(2);
    digitalWrite(Trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(Trig, LOW);
    float distance = pulseIn(Echo, HIGH) / 58.00;
    delay(10); 
    return distance;
}

const int Servo_Act_0 [ ] PROGMEM = {   90,  90,  90,  90,  500  };   // YL RL YR RR

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
    // delay(2000);
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

