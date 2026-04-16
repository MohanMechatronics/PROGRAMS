void setup() {
  pinMode(2, INPUT);   // Set pin 2 as input
  pinMode(6, INPUT);   // Set pin 6 as input
  
  Serial.begin(9600);  // Start serial monitor
}

void loop() {
  int pin2State = digitalRead(2);
  int pin6State = digitalRead(6);

  Serial.print("Pin 2: ");
  Serial.print(pin2State);
  Serial.print("  |  Pin 6: ");
  Serial.println(pin6State);

  delay(300);
}