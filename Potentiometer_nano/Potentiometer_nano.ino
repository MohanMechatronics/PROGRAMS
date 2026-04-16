int potPin = A0;
int potValue = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  potValue = analogRead(potPin); // read value (0–1023)
  Serial.println(potValue);      // print to Serial Monitor
  delay(200);
}