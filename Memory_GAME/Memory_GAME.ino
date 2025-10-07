// ------------------------------------------
// Tone frequency definitions
// ------------------------------------------
#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978

// ------------------------------------------
// Choices & LEDs
// ------------------------------------------
#define CHOICE_OFF      0
#define CHOICE_NONE     0
#define CHOICE_RED      (1 << 0)
#define CHOICE_GREEN    (1 << 1)
#define CHOICE_BLUE     (1 << 2)
#define CHOICE_YELLOW   (1 << 3)

#define LED_RED     28
#define LED_GREEN   16
#define LED_BLUE    17
#define LED_YELLOW  20

// Buttons
#define BUTTON_RED    26
#define BUTTON_GREEN  27
#define BUTTON_BLUE   18
#define BUTTON_YELLOW 19

// Buzzer (two pins for Pico)
#define BUZZER_PIN1 21
#define BUZZER_PIN2 22

// Game parameters
#define ROUNDS_TO_WIN      13
#define ENTRY_TIME_LIMIT   3000

#define MODE_MEMORY  0
#define MODE_BATTLE  1
#define MODE_BEEGEES 2

// Game state
byte gameMode = MODE_MEMORY;
byte gameBoard[32];
byte gameRound = 0;

// ------------------------------------------
// Custom tone() + noTone() for 2-pin buzzer
// ------------------------------------------
void tone(uint8_t pin, unsigned int frequency, unsigned long duration) {
  (void)pin; // unused, kept for compatibility
  if (frequency == 0) {
    delay(duration);
    return;
  }

  unsigned long period_us = 1000000L / frequency;
  unsigned long halfPeriod_us = period_us / 2;
  unsigned long cycles = (duration * 1000L) / period_us;

  for (unsigned long i = 0; i < cycles; i++) {
    digitalWrite(BUZZER_PIN1, HIGH);
    digitalWrite(BUZZER_PIN2, LOW);
    delayMicroseconds(halfPeriod_us);

    digitalWrite(BUZZER_PIN1, LOW);
    digitalWrite(BUZZER_PIN2, HIGH);
    delayMicroseconds(halfPeriod_us);
  }
}

void noTone(uint8_t pin) {
  (void)pin; // unused
  digitalWrite(BUZZER_PIN1, LOW);
  digitalWrite(BUZZER_PIN2, LOW);
}

// ------------------------------------------
// Setup
// ------------------------------------------
void setup() {
  pinMode(BUTTON_RED, INPUT_PULLUP);
  pinMode(BUTTON_GREEN, INPUT_PULLUP);
  pinMode(BUTTON_BLUE, INPUT_PULLUP);
  pinMode(BUTTON_YELLOW, INPUT_PULLUP);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);

  pinMode(BUZZER_PIN1, OUTPUT);
  pinMode(BUZZER_PIN2, OUTPUT);

  gameMode = MODE_MEMORY;

  // Easter egg
  if (checkButton() == CHOICE_YELLOW) play_beegees();

  if (checkButton() == CHOICE_GREEN) {
    gameMode = MODE_BATTLE;
    setLEDs(CHOICE_GREEN);
    toner(CHOICE_GREEN, 150);
    setLEDs(CHOICE_RED | CHOICE_BLUE | CHOICE_YELLOW);
    while (checkButton() != CHOICE_NONE);
  }

  play_winner();
}

// ------------------------------------------
// Main Loop
// ------------------------------------------
void loop() {
  attractMode();
  setLEDs(CHOICE_RED | CHOICE_GREEN | CHOICE_BLUE | CHOICE_YELLOW);
  delay(1000);
  setLEDs(CHOICE_OFF);
  delay(250);

  if (gameMode == MODE_MEMORY) {
    if (play_memory()) play_winner();
    else play_loser();
  }

  if (gameMode == MODE_BATTLE) {
    play_battle();
    play_loser();
  }
}

// ------------------------------------------
// Memory Mode
// ------------------------------------------
boolean play_memory(void) {
  randomSeed(millis());
  gameRound = 0;

  while (gameRound < ROUNDS_TO_WIN) {
    add_to_moves();
    playMoves();

    for (byte currentMove = 0; currentMove < gameRound; currentMove++) {
      byte choice = wait_for_button();
      if (choice == 0) return false;
      if (choice != gameBoard[currentMove]) return false;
    }

    delay(1000);
  }

  return true;
}

// ------------------------------------------
// Battle Mode
// ------------------------------------------
boolean play_battle(void) {
  gameRound = 0;
  while (1) {
    byte newButton = wait_for_button();
    gameBoard[gameRound++] = newButton;

    for (byte currentMove = 0; currentMove < gameRound; currentMove++) {
      byte choice = wait_for_button();
      if (choice == 0) return false;
      if (choice != gameBoard[currentMove]) return false;
    }
    delay(100);
  }
  return true;
}

// ------------------------------------------
// Helpers
// ------------------------------------------
void playMoves(void) {
  for (byte currentMove = 0; currentMove < gameRound; currentMove++) {
    toner(gameBoard[currentMove], 150);
    delay(150);
  }
}

void add_to_moves(void) {
  byte newButton = random(0, 4);
  if (newButton == 0) newButton = CHOICE_RED;
  else if (newButton == 1) newButton = CHOICE_GREEN;
  else if (newButton == 2) newButton = CHOICE_BLUE;
  else if (newButton == 3) newButton = CHOICE_YELLOW;
  gameBoard[gameRound++] = newButton;
}

void setLEDs(byte leds) {
  digitalWrite(LED_RED, (leds & CHOICE_RED) ? HIGH : LOW);
  digitalWrite(LED_GREEN, (leds & CHOICE_GREEN) ? HIGH : LOW);
  digitalWrite(LED_BLUE, (leds & CHOICE_BLUE) ? HIGH : LOW);
  digitalWrite(LED_YELLOW, (leds & CHOICE_YELLOW) ? HIGH : LOW);
}

byte wait_for_button(void) {
  long startTime = millis();
  while ((millis() - startTime) < ENTRY_TIME_LIMIT) {
    byte button = checkButton();
    if (button != CHOICE_NONE) {
      toner(button, 150);
      while (checkButton() != CHOICE_NONE);
      delay(10);
      return button;
    }
  }
  return CHOICE_NONE;
}

byte checkButton(void) {
  if (digitalRead(BUTTON_RED) == 0) return (CHOICE_RED);
  else if (digitalRead(BUTTON_GREEN) == 0) return (CHOICE_GREEN);
  else if (digitalRead(BUTTON_BLUE) == 0) return (CHOICE_BLUE);
  else if (digitalRead(BUTTON_YELLOW) == 0) return (CHOICE_YELLOW);
  return (CHOICE_NONE);
}

void toner(byte which, int buzz_length_ms) {
  setLEDs(which);
  int freq = 440;
  switch (which) {
    case CHOICE_RED:    freq = 440; break;
    case CHOICE_GREEN:  freq = 880; break;
    case CHOICE_BLUE:   freq = 587; break;
    case CHOICE_YELLOW: freq = 784; break;
  }
  tone(BUZZER_PIN1, freq, buzz_length_ms);
  delay(buzz_length_ms);
  noTone(BUZZER_PIN1);
  setLEDs(CHOICE_OFF);
}

void play_winner(void) {
  setLEDs(CHOICE_GREEN | CHOICE_BLUE);
  tone(BUZZER_PIN1, 1000, 200); delay(200);
  setLEDs(CHOICE_RED | CHOICE_YELLOW);
  tone(BUZZER_PIN1, 1200, 200); delay(200);
  setLEDs(CHOICE_GREEN | CHOICE_BLUE);
  tone(BUZZER_PIN1, 1500, 200); delay(200);
  setLEDs(CHOICE_RED | CHOICE_YELLOW);
  tone(BUZZER_PIN1, 1800, 200); delay(200);
  noTone(BUZZER_PIN1);
  setLEDs(CHOICE_OFF);
}

void play_loser(void) {
  setLEDs(CHOICE_RED | CHOICE_GREEN);
  tone(BUZZER_PIN1, 200, 500); delay(500);
  setLEDs(CHOICE_BLUE | CHOICE_YELLOW);
  tone(BUZZER_PIN1, 150, 500); delay(500);
  noTone(BUZZER_PIN1);
  setLEDs(CHOICE_OFF);
}

void attractMode(void) {
  while (1) {
    setLEDs(CHOICE_RED); delay(100); if (checkButton() != CHOICE_NONE) return;
    setLEDs(CHOICE_BLUE); delay(100); if (checkButton() != CHOICE_NONE) return;
    setLEDs(CHOICE_GREEN); delay(100); if (checkButton() != CHOICE_NONE) return;
    setLEDs(CHOICE_YELLOW); delay(100); if (checkButton() != CHOICE_NONE) return;
  }
}

// ------------------------------------------
// Bee Gees Easter Egg
// ------------------------------------------
int melody[] = {
  NOTE_G4, NOTE_A4, 0, NOTE_C5, 0, 0, NOTE_G4, 0, 0, 0,
  NOTE_E4, 0, NOTE_D4, NOTE_E4, NOTE_G4, 0,
  NOTE_D4, NOTE_E4, 0, NOTE_G4, 0, 0,
  NOTE_D4, 0, NOTE_E4, 0, NOTE_G4, 0, NOTE_A4, 0, NOTE_C5, 0
};

int noteDuration = 115;
int LEDnumber = 0;

void play_beegees() {
  setLEDs(CHOICE_YELLOW);
  toner(CHOICE_YELLOW, 150);
  setLEDs(CHOICE_RED | CHOICE_GREEN | CHOICE_BLUE);
  while (checkButton() != CHOICE_NONE);
  setLEDs(CHOICE_NONE);
  delay(1000);

  while (checkButton() == CHOICE_NONE) {
    for (int thisNote = 0; thisNote < 32; thisNote++) {
      changeLED();
      if (melody[thisNote] != 0) tone(BUZZER_PIN1, melody[thisNote], noteDuration);
      delay(noteDuration * 1.30);
      noTone(BUZZER_PIN1);
    }
  }
}

void changeLED(void) {
  setLEDs(1 << LEDnumber);
  LEDnumber++;
  if (LEDnumber > 3) LEDnumber = 0;
}
