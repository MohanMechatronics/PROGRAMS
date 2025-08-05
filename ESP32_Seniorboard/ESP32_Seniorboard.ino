// Define motor control pins
const int enableA = 21;
const int enableB = 25;

const int leftF = 18;
const int leftB = 19;
const int rightF = 23;
const int rightB = 22;

// Alert output pin (LED or buzzer)
const int alertPin = 5;

String inputString = ""; // To hold incoming serial data
bool stringComplete = false;

void setup() {
  // Start Serial2 on GPIO13 (RX2), GPIO14 (TX2)
  Serial2.begin(115200, SERIAL_8N1, 13, 14);

  // Optional debug over USB
  Serial.begin(115200);  
  Serial.println("ESP32 Bot Ready...");

  // Motor pin setup
  pinMode(enableA, OUTPUT);
  pinMode(enableB, OUTPUT);
  pinMode(leftF, OUTPUT);
  pinMode(leftB, OUTPUT);
  pinMode(rightF, OUTPUT);
  pinMode(rightB, OUTPUT);

  // Alert pin setup
  pinMode(alertPin, OUTPUT);

  stopCar(); // Start in stopped state
}

void loop() {
  while (Serial2.available()) {
    char inChar = (char)Serial2.read();
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }

  if (stringComplete) {
    inputString.trim(); // Remove whitespace/newline
    handleCommand(inputString);
    inputString = "";
    stringComplete = false;
  }
}

// Handle commands received via UART
void handleCommand(String command) {
  if (command == "car:1") {
    forward();
  } else if (command == "car:2") {
    driftLeft();
  } else if (command == "car:3") {
    stopCar();
  } else if (command == "car:4") {
    driftRight();
  } else if (command == "car:5") {
    backward();
  } else if (command == "car:6") {
    digitalWrite(alertPin, HIGH);
    Serial.println("Alert ON");
  } else if (command == "car:7") {
    digitalWrite(alertPin, LOW);
    Serial.println("Alert OFF");
  } else {
    Serial.print("Unknown Command: ");
    Serial.println(command);
  }
}

// Motor control functions
void forward() {
  digitalWrite(enableA, HIGH);
  digitalWrite(enableB, HIGH);
  digitalWrite(leftB, LOW);
  digitalWrite(rightB, LOW);
  digitalWrite(leftF, HIGH);
  digitalWrite(rightF, HIGH);
  Serial.println("Forward");
}

void backward() {
  digitalWrite(enableA, HIGH);
  digitalWrite(enableB, HIGH);
  digitalWrite(leftF, LOW);
  digitalWrite(rightF, LOW);
  digitalWrite(leftB, HIGH);
  digitalWrite(rightB, HIGH);
  Serial.println("Backward");
}

void stopCar() {
  digitalWrite(enableA, LOW);
  digitalWrite(enableB, LOW);
  digitalWrite(leftF, LOW);
  digitalWrite(rightF, LOW);
  digitalWrite(leftB, LOW);
  digitalWrite(rightB, LOW);
  Serial.println("Stopped");
}

void driftLeft() {
  digitalWrite(enableA, HIGH);
  digitalWrite(enableB, HIGH);
  digitalWrite(leftF, LOW);
  digitalWrite(rightB, LOW);
  digitalWrite(leftB, HIGH);
  digitalWrite(rightF, HIGH);
  Serial.println("Drift Left");
}

void driftRight() {
  digitalWrite(enableA, HIGH);
  digitalWrite(enableB, HIGH);
  digitalWrite(leftB, LOW);
  digitalWrite(rightF, LOW);
  digitalWrite(leftF, HIGH);
  digitalWrite(rightB, HIGH);
  Serial.println("Drift Right");
}
