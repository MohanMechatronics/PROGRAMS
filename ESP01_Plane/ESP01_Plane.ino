#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

const char* ssid = "I dont have internet";
const char* password = "gunasree";

ESP8266WebServer server(80);

const int throttleLeftPin = 1;  // keep same
const int throttleRightPin = 2; // keep same

const int rudderPin = 0;   // Servo pin for left/right
const int elevatorPin = 3; // Servo pin for up/down

Servo rudderServo;
Servo elevatorServo;

int basePWM = 0;

static const char html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <title>ESP32 Controller</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      margin: 0;
      background-color: black;
      color: white;
      display: flex;
      justify-content: space-between;
      align-items: center;
      height: 100vh;
      padding: 20px;
      box-sizing: border-box;
      font-family: sans-serif;
    }

    .vslider-container, .joystick-container {
      display: flex;
      flex-direction: column;
      align-items: center;
    }

    .vslider {
      writing-mode: bt-lr;
      -webkit-appearance: slider-vertical;
      width: 30px;
      height: 300px;
      background: white;
      border-radius: 10px;
    }

    .joystick {
      width: 200px;
      height: 200px;
      background: #222;
      position: relative;
      border-radius: 50%;
      border: 2px solid #444;
      touch-action: none;
    }

    .knob {
      width: 50px;
      height: 50px;
      background: hsl(242, 100%, 54%);
      border-radius: 50%;
      position: absolute;
      top: 75px;
      left: 75px;
      touch-action: none;
    }
  </style>
</head>
<body>

  <div class="vslider-container">
    <label>Throttle</label>
    <input type="range" min="0" max="255" value="0" class="vslider" id="throttleSlider" oninput="sendThrottle(this.value)">
  </div>

  <div class="joystick-container">
    <label>Joystick</label>
    <div class="joystick" id="joystick">
      <div class="knob" id="knob"></div>
    </div>
  </div>

<script>
let knob = document.getElementById("knob");
let joystick = document.getElementById("joystick");

let dragging = false;

joystick.addEventListener("touchstart", startDrag);
joystick.addEventListener("touchmove", drag);
joystick.addEventListener("touchend", endDrag);

function startDrag(e) {
  dragging = true;
}

function drag(e) {
  if (!dragging) return;
  e.preventDefault();
  let touch = e.touches[0];
  let rect = joystick.getBoundingClientRect();
  let x = touch.clientX - rect.left - 100;
  let y = touch.clientY - rect.top - 100;

  let maxDist = 75;
  let dist = Math.min(Math.sqrt(x*x + y*y), maxDist);
  let angle = Math.atan2(y, x);
  let dx = dist * Math.cos(angle);
  let dy = dist * Math.sin(angle);

  knob.style.left = (100 + dx - 25) + "px";
  knob.style.top = (100 + dy - 25) + "px";

  let joyX = Math.round((dx / maxDist) * 127 + 127);
  let joyY = Math.round((-dy / maxDist) * 127 + 127);

  sendJoystick(joyX, joyY);
}

function endDrag() {
  dragging = false;
  knob.style.left = "75px";
  knob.style.top = "75px";
  sendJoystick(127, 127);
}

function sendThrottle(val) {
  fetch(`/pwm?type=throttle&val=${val}`);
}

function sendJoystick(x, y) {
  fetch(`/joystick?x=${x}&y=${y}`);
}
</script>

</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", html);
}

void handlePWM() {
  String type = server.arg("type");
  int val = server.arg("val").toInt();
  val = constrain(val, 0, 255);

  if (type == "throttle") {
    basePWM = val;
    analogWrite(throttleLeftPin, basePWM);
    analogWrite(throttleRightPin, basePWM);
  }

  server.send(200, "text/plain", "OK");
}

void handleJoystick() {
  int x = constrain(server.arg("x").toInt(), 0, 255);
  int y = constrain(server.arg("y").toInt(), 0, 255);

  int rudderAngle = map(x, 0, 255, 0, 180);
  int elevatorAngle = map(y, 0, 255, 0, 180);

  rudderServo.write(rudderAngle);
  elevatorServo.write(elevatorAngle);

  server.send(200, "text/plain", "Joystick OK");
}

void setup() {
  Serial.begin(115200);

  pinMode(throttleLeftPin, OUTPUT);
  pinMode(throttleRightPin, OUTPUT);
  analogWrite(throttleLeftPin, 0);
  analogWrite(throttleRightPin, 0);

  rudderServo.attach(rudderPin);
  elevatorServo.attach(elevatorPin);

  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/pwm", handlePWM);
  server.on("/joystick", handleJoystick);
  server.begin();
  Serial.println("Server started");
}

void loop() {
  server.handleClient();
}
