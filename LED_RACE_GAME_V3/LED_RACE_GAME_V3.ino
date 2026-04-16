#include <Adafruit_NeoPixel.h>

#define MAXLED 300

// PINS
#define PIN_LED   3
#define PIN_P1    5
#define PIN_P2    7
#define PIN_P3    10
#define PIN_AUDIO 12

int NPIXELS = MAXLED;

// COLORS
#define COLOR1 track.Color(255,0,0)
#define COLOR2 track.Color(0,255,0)
#define COLOR3 track.Color(0,0,255)

// SOUND
int win_music[] = {2637,2637,0,2637,0,2093,2637,0,3136};

// TRACK PHYSICS
byte gravity_map[MAXLED];

float speed1=0, speed2=0, speed3=0;
float dist1=0, dist2=0, dist3=0;

byte loop1=0, loop2=0, loop3=0;
byte loop_max=5;

float ACEL=0.2;
float kf=0.015;
float kg=0.003;

byte flag_sw1=0, flag_sw2=0, flag_sw3=0;
byte draworder=0;

unsigned long timestamp=0;

Adafruit_NeoPixel track = Adafruit_NeoPixel(MAXLED, PIN_LED, NEO_GRB + NEO_KHZ800);

int TBEEP=3;
int tdelay=5;

// ================= TRACK SETUP =================
void set_ramp(byte H,byte a,byte b,byte c){
  for(int i=0;i<(b-a);i++){gravity_map[a+i]=127-i*((float)H/(b-a));}
  gravity_map[b]=127;
  for(int i=0;i<(c-b);i++){gravity_map[b+i+1]=127+H-i*((float)H/(c-b));}
}

// ================= SETUP =================
void setup() {
  for(int i=0;i<NPIXELS;i++) gravity_map[i]=127;

  track.begin();

  pinMode(PIN_P1,INPUT_PULLUP);
  pinMode(PIN_P2,INPUT_PULLUP);
  pinMode(PIN_P3,INPUT_PULLUP);

  start_race();
}

// ================= START =================
void start_race(){
  for(int i=0;i<NPIXELS;i++) track.setPixelColor(i, track.Color(0,0,0));
  track.show();
  delay(2000);

  track.setPixelColor(12, track.Color(0,255,0));
  track.setPixelColor(11, track.Color(0,255,0));
  track.show();
  tone(PIN_AUDIO,400); delay(2000); noTone(PIN_AUDIO);

  track.setPixelColor(12, track.Color(0,0,0));
  track.setPixelColor(11, track.Color(0,0,0));
  track.setPixelColor(10, track.Color(255,255,0));
  track.setPixelColor(9, track.Color(255,255,0));
  track.show();
  tone(PIN_AUDIO,600); delay(2000); noTone(PIN_AUDIO);

  track.setPixelColor(10, track.Color(0,0,0));
  track.setPixelColor(9, track.Color(0,0,0));
  track.setPixelColor(8, track.Color(255,0,0));
  track.setPixelColor(7, track.Color(255,0,0));
  track.show();
  tone(PIN_AUDIO,1200); delay(2000); noTone(PIN_AUDIO);

  for(int i=0;i<NPIXELS;i++) track.setPixelColor(i, track.Color(0,0,0));
  track.show();

  timestamp=0;
}

// ================= WINNER FX =================
void winner_fx() {
  int msize = sizeof(win_music)/sizeof(int);
  for (int i=0;i<msize;i++){
    tone(PIN_AUDIO, win_music[i],200);
    delay(230);
    noTone(PIN_AUDIO);
  }
}

// ================= DRAW =================
void draw_car1(){
  for(int i=0;i<=loop1;i++)
    track.setPixelColor(((word)dist1 % NPIXELS)+i, track.Color(0,255-i*20,0));
}

void draw_car2(){
  for(int i=0;i<=loop2;i++)
    track.setPixelColor(((word)dist2 % NPIXELS)+i, track.Color(255-i*20,0,0));
}

void draw_car3(){
  for(int i=0;i<=loop3;i++)
    track.setPixelColor(((word)dist3 % NPIXELS)+i, track.Color(0,0,255-i*20));
}

// ================= LOOP =================
void loop() {

  // TRACK BACKGROUND
  for(int i=0;i<NPIXELS;i++)
    track.setPixelColor(i, track.Color(0,0,(127-gravity_map[i])/8));

  // -------- PLAYER 1 --------
  if(flag_sw1 && !digitalRead(PIN_P1)){flag_sw1=0; speed1+=ACEL;}
  if(!flag_sw1 && digitalRead(PIN_P1)) flag_sw1=1;

  if(gravity_map[(word)dist1 % NPIXELS]<127)
    speed1-=kg*(127-gravity_map[(word)dist1 % NPIXELS]);
  else
    speed1+=kg*(gravity_map[(word)dist1 % NPIXELS]-127);

  speed1-=speed1*kf;
  dist1+=speed1;

  // -------- PLAYER 2 --------
  if(flag_sw2 && !digitalRead(PIN_P2)){flag_sw2=0; speed2+=ACEL;}
  if(!flag_sw2 && digitalRead(PIN_P2)) flag_sw2=1;

  if(gravity_map[(word)dist2 % NPIXELS]<127)
    speed2-=kg*(127-gravity_map[(word)dist2 % NPIXELS]);
  else
    speed2+=kg*(gravity_map[(word)dist2 % NPIXELS]-127);

  speed2-=speed2*kf;
  dist2+=speed2;

  // -------- PLAYER 3 --------
  if(flag_sw3 && !digitalRead(PIN_P3)){flag_sw3=0; speed3+=ACEL;}
  if(!flag_sw3 && digitalRead(PIN_P3)) flag_sw3=1;

  if(gravity_map[(word)dist3 % NPIXELS]<127)
    speed3-=kg*(127-gravity_map[(word)dist3 % NPIXELS]);
  else
    speed3+=kg*(gravity_map[(word)dist3 % NPIXELS]-127);

  speed3-=speed3*kf;
  dist3+=speed3;

  // -------- LAPS --------
  if(dist1>NPIXELS*loop1){loop1++; tone(PIN_AUDIO,600); TBEEP=2;}
  if(dist2>NPIXELS*loop2){loop2++; tone(PIN_AUDIO,700); TBEEP=2;}
  if(dist3>NPIXELS*loop3){loop3++; tone(PIN_AUDIO,800); TBEEP=2;}

  // -------- WIN (FIXED ORIGINAL STYLE) --------
  if (loop1 > loop_max || loop2 > loop_max || loop3 > loop_max) {

    if(loop1 > loop_max){
      for(int i=0;i<NPIXELS;i++) track.setPixelColor(i, track.Color(0,255,0));
    }
    else if(loop2 > loop_max){
      for(int i=0;i<NPIXELS;i++) track.setPixelColor(i, track.Color(255,0,0));
    }
    else if(loop3 > loop_max){
      for(int i=0;i<NPIXELS;i++) track.setPixelColor(i, track.Color(0,0,255));
    }

    track.show();
    winner_fx();

    loop1=loop2=loop3=0;
    dist1=dist2=dist3=0;
    speed1=speed2=speed3=0;
    timestamp=0;

    start_race();
  }

  // -------- DRAW --------
  if(draworder==0){
    draw_car1(); draw_car2(); draw_car3();
  } else {
    draw_car3(); draw_car2(); draw_car1();
  }

  draworder=!draworder;

  track.show();
  delay(tdelay);

  // -------- SOUND FIX --------
  if (TBEEP>0) {
    TBEEP--;
    if (TBEEP==0) {
      noTone(PIN_AUDIO);
    }
  }
}