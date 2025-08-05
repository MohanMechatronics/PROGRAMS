const int buttonPin = 14;   // Button input
const int ledPin = 15;      // LED output
const int buzzerPin = 12;   // Buzzer output

int pressCount = 0;
bool waitingForRelease = false;
unsigned long firstPressTime = 0;
const unsigned long pressTimeout = 1000; // 1 second window

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);

  Serial.begin(115200);
  Serial.println("ESP32 System Ready (Heart rate code removed)");
}

void loop() {
  // ------------------ Button Logic ------------------
  int buttonState = digitalRead(buttonPin);

  if (buttonState == LOW && !waitingForRelease) {
    pressCount++;
    waitingForRelease = true;

    Serial.print("Button Pressed - Count: ");
    Serial.println(pressCount);

    if (pressCount == 1) {
      firstPressTime = millis(); // Start timing
    }
  }

  if (buttonState == HIGH && waitingForRelease) {
    waitingForRelease = false;
  }

  if (pressCount > 0 && (millis() - firstPressTime > pressTimeout)) {
    if (pressCount == 1) {
      digitalWrite(ledPin, LOW);
      digitalWrite(buzzerPin, LOW);
      Serial.println("Action: ALL OFF");
    } else if (pressCount == 2) {
      digitalWrite(ledPin, HIGH);
      digitalWrite(buzzerPin, LOW);
      Serial.println("Action: LED ON");
    } else if (pressCount == 3) {
      digitalWrite(ledPin, LOW);
      digitalWrite(buzzerPin, HIGH);
      Serial.println("Action: BUZZER ON");
    } else {
      Serial.print("Action: Invalid count = ");
      Serial.println(pressCount);
    }

    pressCount = 0;
  }

  delay(10); // Avoid flooding serial monitor
}
