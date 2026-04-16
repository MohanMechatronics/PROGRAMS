/*
         ---------------
        |     O   O     |
        |---------------|
YR 12==> |             | <== YL 27   180-0   180-0
         ---------------
            ||     ||
            ||     ||
RR 25==> ------   ------ <== RL 26
  180-0  180-0
         |-----   -----|
*/

#include <ACB_Biped_Robot_WiFi.h>
#include <WiFi.h>

#define FIRMWARE_VERSION "20240814 V1.0"

ACB_Biped_Robot_WiFi Biped_Robot;

// WiFi user name and password 

const char* ssid = "Biped_Robot";
const char* password = "12345678";


void setup() {    // initialize
  Serial.begin(115200);  // set the baud rate to 115200
 
  Biped_Robot.myservo_init(4,39,5,38);
  // Biped_Robot.myservo_init(12,25,26,27);

  Biped_Robot.Ultrasonic_Init();

  WiFi.setTxPower(WIFI_POWER_19_5dBm);  // Set the transmit power of WiFi
  WiFi.mode(WIFI_AP); // Set WiFi mode to AP (Access Point) mode
  WiFi.softAP(ssid, password, 9); // Set the WiFi hotspot name and password, and enable AP mode
  IPAddress myIP = WiFi.softAPIP(); // Obtain the IP address of the AP mode
  Serial.print("AP IP address: "); 
  Serial.println(myIP); // address: 192.168.4.1
  Biped_Robot.stop();
  Biped_Robot.startWebServer();
  
}

void loop() {
  
  if (Biped_Robot.val==1) 
  {
    Biped_Robot.forward();
  } 
  
  else if (Biped_Robot.val==2) 
  {   
    Biped_Robot.backward();
  } 
  
  else if (Biped_Robot.val==3) 
  {
    Biped_Robot.leftward();      
  } 
  
  else if (Biped_Robot.val==4) 
  {
    Biped_Robot.rightward();
  }

  else if (Biped_Robot.val==4) 
  {
    Biped_Robot.rightward();
  }

  else if (Biped_Robot.val==8) 
  {
    Biped_Robot.stop();
  }

  // Sprint
  else if (Biped_Robot.val==10) 
  {
    Biped_Robot.sprint();
    Biped_Robot.val = 8;
  } 
  
  else if (Biped_Robot.val==11) 
  {
    Biped_Robot.left_kick();
  } 
  
  else if (Biped_Robot.val==12) 
  {
    Biped_Robot.right_kick();
  } 
      
  else if (Biped_Robot.val==13) 
  {
    Biped_Robot.left_tilt(); 
    Biped_Robot.val = 8;
  }

  else if (Biped_Robot.val==14) 
  {
    Biped_Robot.right_tilt();
    Biped_Robot.val = 8;
  }
      
  else if (Biped_Robot.val==15) 
  {
    Biped_Robot.left_stamp(); 
    Biped_Robot.val = 8;
  } 
  
  else if (Biped_Robot.val==16) 
  {
    Biped_Robot.dance();
    Biped_Robot.val = 8;
  }

  // avoid
  else if (Biped_Robot.val==17)     
  {
    Biped_Robot.avoid();
  }

  // follow
  else if (Biped_Robot.val==18) 
  {
    Biped_Robot.follow();
  }

  else if (Biped_Robot.val==19) 
  {
    Biped_Robot.left_ankles();
    Biped_Robot.val = 8;
  }

  else if (Biped_Robot.val==20) 
  {
    Biped_Robot.right_stamp();
    Biped_Robot.val = 8;
  } 

  else if (Biped_Robot.val==21) 
  {
    Biped_Robot.right_ankles();
    Biped_Robot.val = 8;
  }



}
