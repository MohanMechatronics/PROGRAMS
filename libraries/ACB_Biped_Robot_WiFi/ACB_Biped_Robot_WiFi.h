#ifndef ACB_Biped_Robot_WiFi_H
#define ACB_Biped_Robot_WiFi_H

#include <Arduino.h>

#define Trig_PIN            13
#define Echo_PIN            14       

class ACB_Biped_Robot_WiFi 
{

public:
  
    ACB_Biped_Robot_WiFi();

    static int val;

    void myservo_init(int port1, int port2, int port3, int port4);
    void startWebServer();
    void startAppServer();

    void stop();
    void forward();
    void backward();
    void leftward();
    void rightward();
    void sprint();
    void left_kick();
    void right_kick();
    void left_tilt();
    void right_tilt();
    void left_stamp();
    void dance();

    void left_ankles();
    void right_stamp();
    void right_ankles();

    void Ultrasonic_Init(); 
    float Ranging(int Trig, int Echo);
    void avoid();

    void follow();

    int UT_distance = 0;
    int middleDistance = 0;
  


private:
  
};

#endif // ACB_Biped_Robot_WiFi_H