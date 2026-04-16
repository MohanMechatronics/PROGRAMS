const int buttonPin = 4;   // connect button to pin 2
int buttonState = 0;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP); // enable internal pull-up
}

void loop() {
  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) {
    Serial.println("Button Pressed");
  } else {
    Serial.println("Button Not Pressed");
  }

  delay(200);
}