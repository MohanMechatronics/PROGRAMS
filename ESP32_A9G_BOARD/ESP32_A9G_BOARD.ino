#include "WiFi.h"
#define DEBUG true

//=================== ULTRASONIC SENSOR (ADDED) ===================
#define TRIG_PIN 25
#define ECHO_PIN 26

long duration;
int distance;
unsigned long ultrasonicMillis = 0;
const unsigned long ultrasonicDelay = 500;
//===============================================================


//******************* Pin Configurations *******************//
#define A9G_PON     13
#define A9G_LOWP    12
#define SOS_Button  14


//******************* Necessary Variables *******************//
boolean stringComplete = false;
String inputString = "";
String fromGSM = "";
bool CALL_END = 1;
char* response = " ";
String res = "";
int c = 0;
String msg;
String custom_message;


//******************* SIM Parameters *******************//
String SOS_NUM = "+919487385165";


//******************* SOS Button Press *******************//
int SOS_Time = 5;

void A9G_Ready_msg();


//=========================== SETUP =============================//
void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 16, 17);

  pinMode(A9G_PON, OUTPUT);
  pinMode(A9G_LOWP, OUTPUT);

  digitalWrite(A9G_LOWP, HIGH);
  digitalWrite(A9G_PON, HIGH);
  delay(1000);
  digitalWrite(A9G_PON, LOW);
  delay(10000);

  WiFi.mode(WIFI_OFF);
  btStop();

  pinMode(SOS_Button, INPUT_PULLUP);

  delay(20000);

  digitalWrite(A9G_LOWP, LOW);

  msg = "";
  msg = sendData("AT", 1000, DEBUG);
  while (msg.indexOf("OK") == -1) {
    msg = sendData("AT", 1000, DEBUG);
  }

  msg = "";
  msg = sendData("AT+GPS=1", 2000, DEBUG);
  while (msg.indexOf("OK") == -1) {
    msg = sendData("AT+GPS=1", 1000, DEBUG);
  }

  msg = "";
  msg = sendData("AT+GPSLP = 2", 2000, DEBUG);
  while (msg.indexOf("OK") == -1) {
    msg = sendData("AT+GPSLP = 2", 1000, DEBUG);
  }

  msg = "";
  msg = sendData("AT+SLEEP = 1", 2000, DEBUG);
  while (msg.indexOf("OK") == -1) {
    msg = sendData("AT+SLEEP = 1", 1000, DEBUG);
  }

  msg = "";
  msg = sendData("AT+CMGF = 1", 2000, DEBUG);
  while (msg.indexOf("OK") == -1) {
    msg = sendData("AT+CMGF = 1", 1000, DEBUG);
  }

  msg = "";
  msg = sendData("AT+CSMP  = 17,167,0,0 ", 2000, DEBUG);
  while (msg.indexOf("OK") == -1) {
    msg = sendData("AT+CSMP  = 17,167,0,0 ", 1000, DEBUG);
  }

  msg = "";
  msg = sendData("AT+CPMS = \"SM\",\"ME\",\"SM\" ", 2000, DEBUG);
  while (msg.indexOf("OK") == -1) {
    msg = sendData("AT+CPMS = \"SM\",\"ME\",\"SM\" ", 1000, DEBUG);
  }

  msg = "";
  msg = sendData("AT+SNFS=2", 2000, DEBUG);
  while (msg.indexOf("OK") == -1) {
    msg = sendData("AT+SNFS=2", 1000, DEBUG);
  }

  msg = "";
  msg = sendData("AT+CLVL=8", 2000, DEBUG);
  while (msg.indexOf("OK") == -1) {
    msg = sendData("AT+CLVL=8", 1000, DEBUG);
  }

  A9G_Ready_msg();
  digitalWrite(A9G_LOWP, HIGH);


  //================ ULTRASONIC SETUP (ADDED) =================
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.println("Ultrasonic Sensor Ready");
  //==========================================================
}


//============================ LOOP ============================//
void loop()
{
  if (Serial1.available())
  {
    char inChar = Serial1.read();

    if (inChar == '\n')
    {
      if (fromGSM == "SEND LOCATION\r" || fromGSM == "send location\r" || fromGSM == "Send Location\r")
      {
        Get_gmap_link(0);
        digitalWrite(A9G_LOWP, HIGH);
      }

      else if (fromGSM == "BATTERY?\r" || fromGSM == "battery?\r" || fromGSM == "Battery?\r")
      {
        digitalWrite(A9G_LOWP, LOW);
        msg = sendData("AT+CBC?", 2000, DEBUG);
        msg = msg.substring(19, 24);
        response = &msg[0];
        custom_message = response;
        Send_SMS(custom_message);
      }

      else if (fromGSM == "RING\r")
      {
        digitalWrite(A9G_LOWP, LOW);
        Serial1.println("ATA");
      }

      else if (fromGSM == "NO CARRIER\r")
      {
        CALL_END = 1;
        digitalWrite(A9G_LOWP, HIGH);
      }

      Serial.println(fromGSM);
      fromGSM = "";
    }
    else
    {
      fromGSM += inChar;
    }
  }

  if (Serial.available()) {
    Serial1.write(Serial.read());
  }

  if (digitalRead(SOS_Button) == LOW && CALL_END == 1)
  {
    for (c = 0; c < SOS_Time; c++)
    {
      delay(1000);
      if (digitalRead(SOS_Button) == HIGH)
        break;
    }
    if (c == 5)
    {
      Get_gmap_link(1);
    }
  }


  //================ ULTRASONIC TASK (ADDED) =================
  if (millis() - ultrasonicMillis >= ultrasonicDelay)
  {
    ultrasonicMillis = millis();

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    duration = pulseIn(ECHO_PIN, HIGH, 25000);

    if (duration > 0)
    {
      distance = duration * 0.034 / 2;
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" cm");
    }
    else
    {
      Serial.println("Ultrasonic: Out of range");
    }
  }
  //==========================================================
}


//========================= FUNCTIONS =========================//
void Get_gmap_link(bool makeCall)
{
  digitalWrite(A9G_LOWP, LOW);
  delay(1000);
  Serial1.println("AT+LOCATION = 2");

  while (!Serial1.available());
  while (Serial1.available())
  {
    char add = Serial1.read();
    res += add;
  }

  res = res.substring(17, 38);
  response = &res[0];

  if (!strstr(response, "GPS NOT"))
  {
    int i = 0;
    while (response[i] != ',') i++;

    String lat = res.substring(2, i);
    String lon = res.substring(i + 1);

    custom_message = "I'm Here http://maps.google.com/maps?q=" + lat + "+" + lon;
    Send_SMS(custom_message);
  }

  if (makeCall)
  {
    Serial1.println("ATD" + SOS_NUM);
    CALL_END = 0;
  }

  res = "";
}

void A9G_Ready_msg()
{
  custom_message = "A9G Ready!!";
  Send_SMS(custom_message);
}

String sendData(String command, const int timeout, boolean debug)
{
  String temp = "";
  Serial1.println(command);
  long int time = millis();
  while ((time + timeout) > millis())
  {
    while (Serial1.available())
    {
      char c = Serial1.read();
      temp += c;
    }
  }
  if (debug) Serial.print(temp);
  return temp;
}

void Send_SMS(String message)
{
  Serial1.println("AT+CMGF=1");
  delay(1000);
  Serial1.println("AT+CMGS=\"" + SOS_NUM + "\"\r");
  delay(1000);
  Serial1.println(message);
  delay(1000);
  Serial1.println((char)26);
  delay(3000);
  Serial1.println("AT+CMGD=1,4");
}
