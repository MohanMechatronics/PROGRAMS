#include "esp_http_server.h"
#include "esp_camera.h"
#include "esp_wifi.h"
#include <WiFi.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "driver/ledc.h"

const char* ssid = "Surveillance_BOT";
const char* password = "12345678";

// Camera model config
#define CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define FlashPin 4
#define PART_BOUNDARY "123456789000000000000987654321"
static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

httpd_handle_t stream_httpd = NULL;
httpd_handle_t camera_httpd = NULL;

static esp_err_t stream_handler(httpd_req_t *req) {
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  char * part_buf[64];
  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if (res != ESP_OK) return res;

  while (true) {
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      res = ESP_FAIL;
    } else {
      if (fb->format != PIXFORMAT_JPEG) {
        bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
        esp_camera_fb_return(fb);
        fb = NULL;
        if (!jpeg_converted) {
          Serial.println("JPEG compression failed");
          res = ESP_FAIL;
        }
      } else {
        _jpg_buf_len = fb->len;
        _jpg_buf = fb->buf;
      }
    }

    if (res == ESP_OK) {
      size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if (res == ESP_OK) {
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    if (res == ESP_OK) {
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }
    if (fb) {
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    } else if (_jpg_buf) {
      free(_jpg_buf);
      _jpg_buf = NULL;
    }
    if (res != ESP_OK) break;
  }
  return res;
}

static esp_err_t cmd_handler(httpd_req_t *req) {
  char*  buf;
  size_t buf_len;
  char variable[32] = {0,};
  char value[32] = {0,};

  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) {
    buf = (char*)malloc(buf_len);
    if (!buf) return httpd_resp_send_500(req);
    if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
      if (httpd_query_key_value(buf, "var", variable, sizeof(variable)) != ESP_OK ||
          httpd_query_key_value(buf, "val", value, sizeof(value)) != ESP_OK) {
        free(buf);
        return httpd_resp_send_404(req);
      }
    } else {
      free(buf);
      return httpd_resp_send_404(req);
    }
    free(buf);
  } else {
    return httpd_resp_send_404(req);
  }

  int val = atoi(value);
  sensor_t * s = esp_camera_sensor_get();
  int res = 0;

  if (!strcmp(variable, "framesize")) {
    res = s->set_framesize(s, (framesize_t)val);
  }
  else if (!strcmp(variable, "quality")) {
    res = s->set_quality(s, val);
  }
  else if (!strcmp(variable, "flash")) {
    ledcWrite(4, val);
  }
  else if (!strcmp(variable, "car")) {
    Serial.print("car:");
    Serial.println(val);  // Send command to other ESP32
  }
  else {
    res = -1;
  }

  if (res) return httpd_resp_send_500(req);
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, NULL, 0);
}

static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!doctype html>
<html>
    <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width,initial-scale=1">
        <title>HAVE FUN WITH YOUR CONTROLS </title>
        <style>
:root {
--clr-neon: hsl(242 100% 54%);
--clr-bg: hsl(17 100% 54%);
}
            .btn{
    width: 100px;
    height: 65px;
    width:65px;
    position: relative;
    overflow: hidden;
    cursor: pointer;
    border: var(--clr-neon) 3px solid;
   background-color:transparent;
   border-radius: 0.50em;
   box-shadow: inset 0 0 10px var(--clr-neon), 0 0 30px var(--clr-neon);
   transition:all 0.5s;
}
.btn1:before {
    content: "";
    position: absolute;
    top: 0;
    left: 0;
    width: 0%;
    height: 100%;
    transition: all 0.3s;
    z-index: -1;
}
.btn2:before {
    content: "";
    position: absolute;
    top: 0;
    right: 0;
    width: 0%;
    height: 100%;
    transition: all 0.3s;
    z-index: -1;
}
.btn3:before {
    content: "";
    position: absolute;
    top: 0;
    right: 0;
    width: 100%;
    height: 0%;
    transition: all 0.3s;
    z-index: -1;
}
.btn4:before {
    content: "";
    position: absolute;
    bottom: 0;
    right: 0;
    width: 100%;
    height: 0%;
    transition: all 0.3s;
    z-index: -1;
}
@media (hover: hover){
 .btn1:hover:before{
    background-color:var(--clr-bg);
    color:#fff; 
    width: 100%;
 }
 .btn2:hover:before{
    background-color:var(--clr-bg);
    color:#fff; 
    width: 100%;
 }
 .btn3:hover:before{
    background-color:var(--clr-bg);
    color:#fff; 
    height: 100%;
 }
 .btn4:hover:before{
    background-color:var(--clr-bg);
    color:#fff; 
    height: 100%;
 }
}
@media (hover: none){
 .btn1:active:before{
    background-color:var(--clr-bg);
    color:#fff; 
    width: 100%;
 }
 .btn2:active:before{
    background-color:var(--clr-bg);
    color:#fff; 
    width: 100%;
 }
 .btn3:active:before{
    background-color:var(--clr-bg);
    color:#fff; 
    height: 100%;
 }
 .btn4:active:before{
    background-color:var(--clr-bg);
    color:#fff; 
    height: 100%;
 }
}

.btn5:before {
    content: "";
    bottom: 0;
    right: 0;
    width: 100%;
    height: 0%;
    position: absolute;
   z-index: -1;
}

.btn5:hover:before{
    background-color:var(--clr-bg);
    color:#fff; 
    height: 100%;
}
            .slider {appearance: none;width: 70%;height: 15px;border-radius: 10px;background: #ffffff;outline: none;opacity: 0.7;-webkit-transition: .2s;transition: opacity .2s;}
            .slider::-webkit-slider-thumb {appearance: none;width: 30px;height: 30px;border-radius: 50%;background: var(--clr-neon);cursor: pointer;}
            .slider:hover {opacity: 1;}
            .label {font-family: "Balsamiq Sans", cursive;color: var(--clr-neon);text-shadow: 0 0 0.25em hsl(0 0% 100% / 0.3), 0 0 0.95em currentColor;font-size: 20px;}
            /* ----------------------------------- Stream Viewer */
            img {
                width: 250;
                max-width: 100% ;
                height: 250 ;
            }
            /* ----------------------------------- */
.triangle-up::after {
            content:"";
  border-left: 25px solid transparent;
  border-right: 25px solid transparent;
  border-bottom: 50px solid #ffffff;
           display: inline-block;
          
}
.triangle-left::after {
            content:"";
  border-top: 25px solid transparent;
  border-right: 50px solid #ffffff;
  border-bottom: 25px solid transparent;
           display: inline-block;
}
.triangle-right::after{
            content:"";
  border-top: 25px solid transparent;
  border-left: 50px solid #ffffff;
  border-bottom: 25px solid transparent;
  display: inline-block;
}
.triangle-down::after {
             content:"";
  border-left: 25px solid transparent;
  border-right: 25px solid transparent;
  border-top: 50px solid #ffffff;
           display: inline-block;
}
.stop::after {
  content:"";
  border-radius: 25px;
  border: 4px solid #73AD21;
 background: #ff1900;
  padding: 18px; 
  width: 6px;
  height: 7px;
 display: inline-block;
font-size:25px; 
color: var(--clr-neon);
position: relative;
}
.switch {
  position: relative;
  display: flex;
  border: var(--clr-neon) 3px solid;
   background-color:transparent;
   border-radius: 30px;
   box-shadow: inset 0 0 50px var(--clr-neon), 0 0 30px var(--clr-neon);
   transition:all 0.5s;
  width: 60px;
  height: 34px;
}
.switch input { 
  outline: 0;
  opacity: 0;
  width: 0;
  height: 0;
}

.slider2 {
  position: absolute;
  cursor: pointer;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: #ccc;
  -webkit-transition: .4s;
  transition: .4s;
}

.slider2:before {
  position: absolute;
  content: "";
  height: 26px;
  width: 26px;
  left: 4px;
  bottom: 4px;
  background-color: white;
  -webkit-transition: .4s;
  transition: .4s;
}

input:checked + .slider2 {
  background-color: var(--clr-neon);
}

input:focus + .slider2 {
  box-shadow: 0 0 5px #2196F3;
}

input:checked + .slider2:before {
  -webkit-transform: translateX(26px);
  -ms-transform: translateX(26px);
  transform: translateX(26px);
}

/* Rounded sliders */
.slider2.round {
  border-radius: 34px;
}

.slider2.round:before {
  border-radius: 50%;
}
        /* ----------------------------------- */
        </style>
    </head>
    <body
 style="background-color:#000000;">
<div align=center> <img src="" id="vdstream"></div>
    <br/>
    <div align=center> 
    <button class=" btn btn4 triangle-up" id="forward" ontouchstart="fetch(document.location.origin+'/control?var=car&val=1');" ontouchend="fetch(document.location.origin+'/control?var=car&val=3');"></button>
    </div>
    <br/>
    <div align=center> 
    <button class="btn btn2 triangle-left" id="turnleft" ontouchstart="fetch(document.location.origin+'/control?var=car&val=4');" ontouchend="fetch(document.location.origin+'/control?var=car&val=3');"></button>
    <button class="btn btn5 stop" id="stop" onclick="fetch(document.location.origin+'/control?var=car&val=3'  stop);"></button>
    <button class="btn btn1 triangle-right" id="turnright" ontouchstart="fetch(document.location.origin+'/control?var=car&val=2');" ontouchend="fetch(document.location.origin+'/control?var=car&val=3');" ></button>
    </div>
    <br/>
    <div align=center> 
    <button class="btn btn3 triangle-down" id="backward" ontouchstart="fetch(document.location.origin+'/control?var=car&val=5');" ontouchend="fetch(document.location.origin+'/control?var=car&val=3');"></button>
    </div>
    <br/>
    <div align=center> 
    <label class="label">Flash</label>
    <input type="range" class="slider" id="flash" min="0" max="255" value="0" onchange="try{fetch(document.location.origin+'/control?var=flash&val='+this.value);}catch(e){}">
    </div>
    <br/>
    <div align=center>
    <label for="controlstate" class="switch">
      <input type="checkbox" id="controlstate" onclick="myFunction()">
      <span class="slider2 round" ></span>
    </label>
    </div>
</br>

    <script>
    /* ----------------------------------- Calls the video stream link and displays it */ 
    window.onload = document.getElementById("vdstream").src = window.location.href.slice(0, -1) + ":81/stream";
    /* ----------------------------------- */
    function myFunction() {
     var checkBox = document.getElementById("controlstate");
     if (checkBox.checked == true){
      fetch(document.location.origin+'/control?var=car&val=6');
     } else {
      fetch(document.location.origin+'/control?var=car&val=7');
     }
   }
  /* ----------------------------------- */
    </script>   
    </body>
</html>
)rawliteral";

static esp_err_t index_handler(httpd_req_t *req) {
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, (const char *)INDEX_HTML, strlen(INDEX_HTML));
}

void startCameraServer(){
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;

  httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_handler,
    .user_ctx  = NULL
  };

  httpd_uri_t cmd_uri = {
    .uri       = "/control",
    .method    = HTTP_GET,
    .handler   = cmd_handler,
    .user_ctx  = NULL
  };

  httpd_uri_t stream_uri = {
    .uri       = "/stream",
    .method    = HTTP_GET,
    .handler   = stream_handler,
    .user_ctx  = NULL
  };

  if (httpd_start(&camera_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(camera_httpd, &index_uri);
    httpd_register_uri_handler(camera_httpd, &cmd_uri);
  }

  config.server_port += 1;
  config.ctrl_port += 1;

  if (httpd_start(&stream_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(stream_httpd, &stream_uri);
  }

  Serial.println("Camera stream ready at:");
  Serial.println(WiFi.softAPIP());
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Disable brownout
  Serial.begin(115200);  // TX to motor ESP32

  // Camera config
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 10;
  config.fb_count = 1;

  // Init camera
  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera init failed");
    return;
  }


  // Start WiFi AP
  WiFi.softAP(ssid, password);
  Serial.println("AP Started");
  Serial.println(WiFi.softAPIP());

  startCameraServer();
}

void loop() {
  // Nothing here
}
