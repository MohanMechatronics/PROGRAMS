#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "I dont have internet";
const char* password = "gunasree";

ESP8266WebServer server(80);

// GPIO pins for PWM
const int leftPin = 1;  // GPIO1 (TX)
const int rightPin = 0; // GPIO2 (D4 / Built-in LED on many boards)

// HTML must be declared before it's used in handleRoot()
static const char PROGMEM html[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Vertical Sliders</title>
    <style>
        :root {
            --clr-neon: hsl(242 100% 54%);
        }

        body {
            margin: 0;
            background-color: black;
            display: flex;
            justify-content: space-between;
            align-items: center;
            height: 100vh;
            padding: 0 20px;
        }

        .vslider-container {
            height: 300px;
            display: flex;
            justify-content: center;
            align-items: center;
        }

        .vslider {
            writing-mode: bt-lr; /* bottom to top */
            -webkit-appearance: slider-vertical;
            width: 30px;
            height: 300px;
            background: #ffffff;
            border-radius: 10px;
            outline: none;
            opacity: 0.7;
            transition: opacity 0.2s;
        }

        .vslider:hover {
            opacity: 1;
        }

        .vslider::-webkit-slider-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 30px;
            height: 30px;
            border-radius: 50%;
            background: var(--clr-neon);
            cursor: pointer;
        }

        .vslider::-moz-range-thumb {
            width: 30px;
            height: 30px;
            border-radius: 50%;
            background: var(--clr-neon);
            cursor: pointer;
        }
    </style>
</head>
<body>

    <!-- Left Slider -->
    <div class="vslider-container">
        <input type="range" min="0" max="255" value="0" class="vslider" 
               oninput="updatePWM('left', this.value)">
    </div>

    <!-- Right Slider -->
    <div class="vslider-container">
        <input type="range" min="0" max="255" value="0" class="vslider" 
               oninput="updatePWM('right', this.value)">
    </div>

<script>
function updatePWM(side, val) {
    fetch(`/pwm?side=${side}&val=${val}`);
}
</script>
</body>
</html>
)rawliteral";

// Handle root page
void handleRoot() {
  server.send(200, "text/html", (const char*) html);
}

// Handle slider PWM input
void handlePWM() {
  String side = server.arg("side");
  int val = server.arg("val").toInt();

  if (val < 0) val = 0;
  if (val > 255) val = 255;

  if (side == "left") {
    analogWrite(leftPin, val);
  } else if (side == "right") {
    analogWrite(rightPin, val);
  }

  server.send(200, "text/plain", "PWM set");
}

void setup() {
  Serial.begin(115200);
  pinMode(leftPin, OUTPUT);
  pinMode(rightPin, OUTPUT);

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
