void setup() {
  Serial.begin(9600);  // Start serial communication
}

void loop() {
  int val0 = analogRead(A0);
  int val1 = analogRead(A1);
  int val2 = analogRead(A2);
  int val3 = analogRead(A3);

  Serial.print("A0: ");
  Serial.print(val0);
  Serial.print("  |  A1: ");
  Serial.print(val1);
  Serial.print("  |  A2: ");
  Serial.print(val2);
  Serial.print("  |  A3: ");
  Serial.println(val3);

  delay(500); // Small delay for readability
}