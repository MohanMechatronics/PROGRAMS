// #define LED_Blue 0  // Use a PWM-capable pin

// int brightness = 0;     // LED brightness level (0 to 255)
// int fadeAmount = 5;     // How much to change the brightness

// void setup() {
//   Serial.begin(115200);
//   pinMode(LED_Blue, OUTPUT);
// }

// void loop() {
//   analogWrite(LED_Blue, brightness);
//   Serial.print("Brightness: ");
//   Serial.println(brightness);

//   brightness += fadeAmount;

//   // Reverse direction at bounds
//   if (brightness <= 0 || brightness >= 255) {
//     fadeAmount = -fadeAmount;
//   }

//   delay(30); // Adjust for smoothness/speed
// }


#define LED_Blue 1 //2
#define LED_red 0
void setup() {
 // initialize digital pin LED_BUILTSIN as an output.
  Serial.begin(115200);
  pinMode(LED_Blue, OUTPUT);
  pinMode(LED_red, OUTPUT);
  digitalWrite(LED_Blue, HIGH);
  digitalWrite(LED_red, HIGH);
  Serial.println("turn on");
}

// the loop function runs over and over again forever
void loop() {
  // digitalWrite(LED_Blue, HIGH);
  // digitalWrite(LED_red, HIGH);
  // Serial.println("turn on");
  // delay(5000);
  // digitalWrite(LED_red, LOW);
  // digitalWrite(LED_Blue, LOW);
  // Serial.println("turn off");
  // delay(5000);
}