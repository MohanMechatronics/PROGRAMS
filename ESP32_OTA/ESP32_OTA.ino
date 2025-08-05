#include <WiFi.h>        // For connecting ESP32 to WiFi
#include <ArduinoOTA.h>  // For enabling over-the-air updates

#define LED_Blue 2 //2
#define LED_red 13

const char* ssid = "Airtel_Disguise";  // Change to your WiFi Network name
const char* password = "gunasree";  // Change to your password

void setup() {
  Serial.begin(115200);

  // WiFi.begin(ssid, password);  // Connect to WiFi - defaults to WiFi Station mode

  // // Ensure WiFi is connected
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  // }
  
  WiFi.softAP(ssid,password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  pinMode(LED_Blue, OUTPUT);
  pinMode(LED_red, OUTPUT);
  ArduinoOTA.begin();  // Starts OTA
}

void loop() {

  ArduinoOTA.handle();  // Handles a code update request
  digitalWrite(LED_Blue, HIGH);
  // digitalWrite(LED_red, HIGH);
  Serial.println("turn on");
  delay(100);
  // digitalWrite(LED_red, LOW);
  digitalWrite(LED_Blue, LOW);
  Serial.println("turn off");
  delay(100);
  // All loop you're code goes here.
}