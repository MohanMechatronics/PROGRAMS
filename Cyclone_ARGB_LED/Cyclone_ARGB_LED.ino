#include "FastLED.h"

//up to 50
#define NUM_LEDS 60 

#define CENTER_LED 30

#define DATA_PIN 3 


#define LED_TYPE WS2811

#define COLOR_ORDER RGB 

//range 0-64
#define BRIGHTNESS 60

//Definition of difficulty levels
#define EASY 1
#define MEDIUM 2
#define HARD 3
#define ON_SPEED 4
#define SONIC_SPEED 5
#define ROCKET_SPEED 6
#define LIGHT_SPEED 7
#define MISSION_IMPOSSIBLE 8

//Starting difficulty
int difficulty = 1;

// Define the array of leds
CRGB leds[NUM_LEDS];

// Did player win this round? This tag is used for difficulty parameters.
bool wonThisRound = false;

// Starting location of the cycling light
int LEDaddress = 0;

// Is game running?
bool Playing = true;

// Is this the first win?
bool CycleEnded = true;

// Button details
const int buttonPin = 4;
int buttonState = 0;

// Initialize the led library and arduino functions

void setup() 
{
  FastLED.addLeds<WS2811,DATA_PIN,RGB>(leds,NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  pinMode(buttonPin,INPUT_PULLUP); // Pull-up switch
  Serial.begin(9600);
}

// The meat and potatoes
//Two Modes - Playing and End Game
void loop() 
{
  //END GAME
  buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);
  if (buttonState == HIGH)
  {
    Playing = false; //User has pressed the button, and the LED has stopped on the winning address.
     for (int i = 0; i < NUM_LEDS; i++)
     {
      leds [i] = CRGB::Black;
     }
    leds[CENTER_LED] = CRGB::Red;
    leds[LEDaddress] = CRGB::Green;  
    FastLED.show();
    if (CycleEnded = true)
    {
      int diff = abs(CENTER_LED - LEDaddress); //Finds distance between the lit led and the center led
      if (diff == 0)
      {
        wonThisRound = true; //Player sucessfully beat the level
        if (difficulty != MISSION_IMPOSSIBLE)
         {
          for (int i = 0; i < 2; i++)
           {
            cylon();
           }
         }
        if (difficulty == MISSION_IMPOSSIBLE)
         {
          for (int i = 0; i < 8; i++)
           {
            cylon();
           }
          difficulty = 0; 
         }
        increaseDifficulty();      
        wonThisRound = false;
      }
       else
      {
      delay(1000);  
      for (int i = 0; i < 2; i++)
       {
        flash();
       }
      }
        CycleEnded = false;
      }
    LEDaddress = 0; 
    delay(250);
    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW)
    {
     Playing = true;
    }
  }
  
  //PLAYING
  if(Playing)
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CRGB::Black; //Turns off all the leds
    }
    leds[CENTER_LED] = CRGB::Red; //Sets center led color to green
    leds[LEDaddress] = CRGB::Green; //Sets cyling led color to red
    FastLED.show(); //Initializes light cycle
    LEDaddress--; // Move backward instead of forward
    if (LEDaddress < 0)
    {
      LEDaddress = NUM_LEDS - 1;
    }
    delay(getTime(difficulty));
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH)
    {
     Playing = false;
     CycleEnded = true;
    }
 }
}

//Level Parameters
int getTime(int diff) // Returns time delay for led movement base on difficulty
{
  int timeValue = 0;
  switch (diff) 
  {
    case EASY:
      timeValue = 100;
      break;
    case MEDIUM:
      timeValue = 80;
      break;
    case HARD:
      timeValue = 60;
      break;
    case ON_SPEED:
      timeValue = 40;
      break;
    case SONIC_SPEED:
      timeValue = 30;
      break;
    case ROCKET_SPEED:
      timeValue = 20;
      break;
    case LIGHT_SPEED:
      timeValue = 13;
      break;
    case MISSION_IMPOSSIBLE:
      timeValue = 7;
  }
  return timeValue;// Return the delay amount
}

//Winning difficulty increase parameters
void increaseDifficulty() 
{
  if (difficulty != MISSION_IMPOSSIBLE && wonThisRound) 
  {
    difficulty++;
  }
}

//Lost LED Show
void flash()
{ 
  fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.show();
  delay(500);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  delay(500);
}

//Won LED Show
void fadeall() 
{ 
  for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } 
}

void cylon() 
{ 
  static uint8_t hue = 0;
  Serial.print("x");
  // First slide the led in one direction
  for(int i = (NUM_LEDS)-1; i >= 0; i--) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }
  Serial.print("x");

  // Now go in the other direction.  
  for(int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show(); 
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }
}