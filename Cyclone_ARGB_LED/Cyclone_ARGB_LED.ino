#include "FastLED.h"

// up to 50
#define NUM_LEDS 39
#define CENTER_LED 19
#define DATA_PIN 6

#define LED_TYPE WS2811
#define COLOR_ORDER RGB 

// range 0-64
#define BRIGHTNESS 60

// Difficulty levels
#define EASY 1
#define MEDIUM 2
#define HARD 3
#define ON_SPEED 4
#define SONIC_SPEED 5
#define ROCKET_SPEED 6
#define LIGHT_SPEED 7
#define MISSION_IMPOSSIBLE 8

int difficulty = EASY;

CRGB leds[NUM_LEDS];

bool wonThisRound = false;
int LEDaddress = 0;

bool Playing = true;
bool CycleEnded = true;

// Button
const int buttonPin = 4;
int buttonState = 0;

void setup() 
{
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  pinMode(buttonPin, INPUT); // ✅ PULL-DOWN (external resistor required)
  Serial.begin(9600);
}

void loop() 
{
  buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);

  // ================= END GAME =================
  if (buttonState == HIGH) // ✅ pressed
  {
    Playing = false;

    // Clear LEDs
    for (int i = 0; i < NUM_LEDS; i++)
      leds[i] = CRGB::Black;

    leds[CENTER_LED] = CRGB::Red;
    leds[LEDaddress] = CRGB::Green;
    FastLED.show();

    if (CycleEnded == true) // ✅ FIXED
    {
      int diff = abs(CENTER_LED - LEDaddress);

      if (diff == 0)
      {
        wonThisRound = true;

        if (difficulty != MISSION_IMPOSSIBLE)
        {
          for (int i = 0; i < 2; i++)
            cylon();
        }
        else
        {
          for (int i = 0; i < 8; i++)
            cylon();

          difficulty = EASY;
        }

        increaseDifficulty();
        wonThisRound = false;
      }
      else
      {
        delay(1000);
        for (int i = 0; i < 2; i++)
          flash();
      }

      CycleEnded = false;
    }

    LEDaddress = 0;
    delay(250);

    // wait for release
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) // ✅ released
    {
      Playing = true;
    }
  }

  // ================= PLAYING =================
  if (Playing)
  {
    for (int i = 0; i < NUM_LEDS; i++)
      leds[i] = CRGB::Black;

    leds[CENTER_LED] = CRGB::Red;
    leds[LEDaddress] = CRGB::Green;

    FastLED.show();

    // Move backward
    LEDaddress--;
    if (LEDaddress < 0)
      LEDaddress = NUM_LEDS - 1;

    delay(getTime(difficulty));

    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH) // ✅ pressed
    {
      Playing = false;
      CycleEnded = true;
    }
  }
}

// ================= SPEED CONTROL =================
int getTime(int diff)
{
  switch (diff)
  {
    case EASY: return 100;
    case MEDIUM: return 80;
    case HARD: return 60;
    case ON_SPEED: return 40;
    case SONIC_SPEED: return 30;
    case ROCKET_SPEED: return 20;
    case LIGHT_SPEED: return 13;
    case MISSION_IMPOSSIBLE: return 7;
  }
  return 100;
}

// ================= DIFFICULTY =================
void increaseDifficulty() 
{
  if (difficulty != MISSION_IMPOSSIBLE && wonThisRound)
    difficulty++;
}

// ================= LOSE EFFECT =================
void flash()
{
  fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.show();
  delay(500);

  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  delay(500);
}

// ================= WIN EFFECT =================
void fadeall() 
{
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i].nscale8(250);
}

void cylon() 
{
  static uint8_t hue = 0;

  for (int i = NUM_LEDS - 1; i >= 0; i--)
  {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
    fadeall();
    delay(10);
  }

  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show();
    fadeall();
    delay(10);
  }
}