#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "I dont have internet";
const char* password = "gunasree";

ESP8266WebServer server(80);

const int leftPin = 0;  // GPIO1 (TX)
const int rightPin = 1; // GPIO0

int basePWM = 0;  // Throttle value (from vertical slider)

static const char PROGMEM html[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP8266 Transmitter</title>
  <style>
    :root {
      --clr-neon: hsl(242, 100%, 54%);
    }

    body {
      margin: 0;
      background-color: black;
      height: 100vh;
      display: flex;
      justify-content: space-between;
      align-items: center;
      padding: 20px;
      box-sizing: border-box;
    }

    .vslider-container, .hslider-container {
      display: flex;
      justify-content: center;
      align-items: center;
    }

    .vslider {
      writing-mode: bt-lr;
      -webkit-appearance: slider-vertical;
      width: 30px;
      height: 300px;
      background: white;
      border-radius: 10px;
      outline: none;
      opacity: 0.7;
    }

    .vslider:hover, .hslider:hover {
      opacity: 1;
    }

    .vslider::-webkit-slider-thumb,
    .hslider::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 30px;
      height: 30px;
      border-radius: 50%;
      background: var(--clr-neon);
      cursor: pointer;
    }

    .hslider {
      width: 300px;
      height: 30px;
      background: white;
      border-radius: 10px;
      outline: none;
      opacity: 0.7;
    }
  </style>
</head>
<body>

  <!-- Left Vertical Throttle Slider -->
  <div class="vslider-container">
    <input type="range" min="0" max="255" value="0" class="vslider" id="throttleSlider" 
           oninput="sendThrottle(this.value)">
  </div>

  <!-- Right Horizontal Steering Slider -->
  <div class="hslider-container">
    <input type="range" min="0" max="255" value="127" class="hslider" id="steerSlider"
           oninput="sendSteer(this.value)" onmouseup="resetSteer()" ontouchend="resetSteer()">
  </div>

  <script>
    function sendThrottle(val) {
      fetch(`/pwm?type=throttle&val=${val}`);
    }

    function sendSteer(val) {
      fetch(`/pwm?type=steer&val=${val}`);
    }

    function resetSteer() {
      document.getElementById("steerSlider").value = 127;
      sendSteer(127);
    }
  </script>

</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", (const char*) html);
}

void handlePWM() {
  String type = server.arg("type");
  int val = server.arg("val").toInt();

  if (val < 0) val = 0;
  if (val > 255) val = 255;

  if (type == "throttle") {
    basePWM = val;
    analogWrite(leftPin, basePWM);
    analogWrite(rightPin, basePWM);
  } else if (type == "steer") {
    // Calculate new PWM values based on steer offset
    int offset = map(val, 0, 255, -basePWM, basePWM);
    int leftVal = constrain(basePWM - offset, 0, 255);
    int rightVal = constrain(basePWM + offset, 0, 255);

    analogWrite(leftPin, leftVal);
    analogWrite(rightPin, rightVal);
  }

  server.send(200, "text/plain", "PWM Updated");
}

void setup() {
  Serial.begin(115200);
  pinMode(leftPin, OUTPUT);
  pinMode(rightPin, OUTPUT);
  analogWriteRange(255);  // Set range

  WiFi.softAP(ssid, password);
  Serial.print("Access Point IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/pwm", handlePWM);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
