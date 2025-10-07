#include "ACB_Biped_Robot_WiFi.h"
#include "ACB_Biped_Robot_WiFi_Action.h"
#include "esp_http_server.h"
#include "Arduino.h"

int ACB_Biped_Robot_WiFi::val = 0;





typedef struct {
        httpd_req_t *req;
        size_t len;
} jpg_chunking_t;
httpd_handle_t camera_httpd = NULL;


ACB_Biped_Robot_WiFi::ACB_Biped_Robot_WiFi()
{

}

void ACB_Biped_Robot_WiFi::myservo_init(int port1, int port2, int port3, int port4) {       //servo initialize
  
  servo_16.attach(port2, SERVOMIN, SERVOMAX);
  servo_17.attach(port3, SERVOMIN, SERVOMAX);
  servo_18.attach(port4, SERVOMIN, SERVOMAX);
  servo_5.attach(port1, SERVOMIN, SERVOMAX);

  Servo_PROGRAM_Zero();

}

void ACB_Biped_Robot_WiFi::stop() {
  Servo_PROGRAM_Run(Servo_Prg_0, Servo_Prg_0_Step);
}

void ACB_Biped_Robot_WiFi::forward() {
  Servo_PROGRAM_Run(Servo_Prg_1, Servo_Prg_1_Step);
}

void ACB_Biped_Robot_WiFi::backward() {
  Servo_PROGRAM_Run(Servo_Prg_2, Servo_Prg_2_Step);
}

void ACB_Biped_Robot_WiFi::leftward() {
  Servo_PROGRAM_Run(Servo_Prg_3, Servo_Prg_3_Step);
}

void ACB_Biped_Robot_WiFi::rightward() {
  Servo_PROGRAM_Run(Servo_Prg_4, Servo_Prg_4_Step);
}

void ACB_Biped_Robot_WiFi::sprint() {
  Servo_PROGRAM_Run(Servo_Prg_10, Servo_Prg_10_Step);
}

void ACB_Biped_Robot_WiFi::left_kick() {
  Servo_PROGRAM_Run(Servo_Prg_11, Servo_Prg_11_Step);
}

void ACB_Biped_Robot_WiFi::right_kick() {
  Servo_PROGRAM_Run(Servo_Prg_12, Servo_Prg_12_Step);
}

void ACB_Biped_Robot_WiFi::left_tilt() {
  Servo_PROGRAM_Run(Servo_Prg_13, Servo_Prg_13_Step);
}

void ACB_Biped_Robot_WiFi::right_tilt() {
  Servo_PROGRAM_Run(Servo_Prg_14, Servo_Prg_14_Step);
}

void ACB_Biped_Robot_WiFi::left_stamp() {
  Servo_PROGRAM_Run(Servo_Prg_15, Servo_Prg_15_Step);
}

void ACB_Biped_Robot_WiFi::dance() {
  Servo_PROGRAM_Run(Servo_Prg_16, Servo_Prg_16_Step);
}

void ACB_Biped_Robot_WiFi::left_ankles() {
  Servo_PROGRAM_Run(Servo_Prg_17, Servo_Prg_17_Step);
}

void ACB_Biped_Robot_WiFi::right_stamp() {
  Servo_PROGRAM_Run(Servo_Prg_18, Servo_Prg_18_Step);
}

void ACB_Biped_Robot_WiFi::right_ankles() {
  Servo_PROGRAM_Run(Servo_Prg_19, Servo_Prg_19_Step);
}

void ACB_Biped_Robot_WiFi::Ultrasonic_Init()
{
    pinMode(Trig_PIN, OUTPUT);
    pinMode(Echo_PIN, INPUT);
}

float ACB_Biped_Robot_WiFi::Ranging(int Trig, int Echo) 
{
    digitalWrite(Trig, LOW);
    delayMicroseconds(2);
    digitalWrite(Trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(Trig, LOW);
    float distance = pulseIn(Echo, HIGH) / 58.00;
    delay(10); 
    return distance;
}

void ACB_Biped_Robot_WiFi::avoid() {
  UT_distance = Ranging(Trig_PIN, Echo_PIN);
  middleDistance = UT_distance;
//   Serial.println(middleDistance);
//   delay(100);
  if (middleDistance <= 15) {
    stop();

    backward();
    backward();
    backward();
    backward();
    backward();
    backward();
    

    // if (middleDistance > 15 && middleDistance <= 20){
    int randNumber = random(1, 3);
      switch (randNumber) {
        case 1:
          Servo_PROGRAM_Run(Servo_Prg_3, Servo_Prg_3_Step);
          delay(500);
          Servo_PROGRAM_Run(Servo_Prg_3, Servo_Prg_3_Step);
          delay(500);
          Servo_PROGRAM_Run(Servo_Prg_3, Servo_Prg_3_Step);
          delay(500);
          Servo_PROGRAM_Run(Servo_Prg_3, Servo_Prg_3_Step);
          delay(500);
          Servo_PROGRAM_Run(Servo_Prg_3, Servo_Prg_3_Step);
          delay(500);
          // Servo_PROGRAM_Run(Servo_Prg_3, Servo_Prg_3_Step);
          // delay(500);
          // Servo_PROGRAM_Run(Servo_Prg_3, Servo_Prg_3_Step);
          // delay(500);
          // Servo_PROGRAM_Run(Servo_Prg_3, Servo_Prg_3_Step);
          // delay(500);
          // Servo_PROGRAM_Run(Servo_Prg_3, Servo_Prg_3_Step);
          // delay(500);
          // Servo_PROGRAM_Run(Servo_Prg_3, Servo_Prg_3_Step);
          // delay(500);
          break;
        case 2:
          Servo_PROGRAM_Run(Servo_Prg_4, Servo_Prg_4_Step);
          delay(500);
          Servo_PROGRAM_Run(Servo_Prg_4, Servo_Prg_4_Step);
          delay(500);
          Servo_PROGRAM_Run(Servo_Prg_4, Servo_Prg_4_Step);
          delay(500);
          Servo_PROGRAM_Run(Servo_Prg_4, Servo_Prg_4_Step);
          delay(500);
          Servo_PROGRAM_Run(Servo_Prg_4, Servo_Prg_4_Step);
          delay(500);
          // Servo_PROGRAM_Run(Servo_Prg_4, Servo_Prg_4_Step);
          // delay(500);
          // Servo_PROGRAM_Run(Servo_Prg_4, Servo_Prg_4_Step);
          // delay(500);
          // Servo_PROGRAM_Run(Servo_Prg_4, Servo_Prg_4_Step);
          // delay(500);
          // Servo_PROGRAM_Run(Servo_Prg_4, Servo_Prg_4_Step);
          // delay(500);
          // Servo_PROGRAM_Run(Servo_Prg_4, Servo_Prg_4_Step);
          // delay(500);
          break;
      }
    // }
  } else {
    forward();
  }

}

void ACB_Biped_Robot_WiFi::follow() {
  UT_distance = Ranging(Trig_PIN, Echo_PIN);
  if (UT_distance < 15) {
    backward();
  } else if (15 <= UT_distance && UT_distance <= 20) {
    stop();
  } else if (20 < UT_distance && UT_distance <= 35) {
    forward();
  } else {
    stop();
  }
}




//*************************Web_Processing*************************
static esp_err_t cmd_handler(httpd_req_t *req) {
    char*  buf;
    size_t buf_len;
    char variable[32] = {0,};
    char value[32] = {0,};

    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = (char*)malloc(buf_len);
        if(!buf){
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            if ((httpd_query_key_value(buf, "var", variable, sizeof(variable)) == ESP_OK &&
                httpd_query_key_value(buf, "val", value, sizeof(value)) == ESP_OK)){
            } else {
                free(buf);
                httpd_resp_send_404(req);
                return ESP_FAIL;
            }
        } else {
            free(buf);
            httpd_resp_send_404(req);
            return ESP_FAIL;
        }
        free(buf);
    } else {
        httpd_resp_send_404(req);
        return ESP_FAIL;
    }

    ACB_Biped_Robot_WiFi::val = atoi(value);
    int res = 0;

    if(!strcmp(variable, "robot")) {  
    //   if (val==1) {
    //     forward();
    //   } else if (val==2) {   
    //     backward();
    //   } else if (val==3) {
    //     leftward();      
    //   } else if (val==4) {
    //     rightward();
    //   } else if (val==5) {
        
    //   } else if (val==6) {
                      
    //   } else if (val==7) {
    //     state = false;
    //     // stop();
    //   } else if (val==8) {
    //     stop();       
    //   } else if (val==9) {
                    
    //   } else if (val==10) {
    //     shadows_step();
    //   } else if (val==11) {
    //     rub();

    //   } else if (val==12) {
    //     swing();
    //   } else if (val==13) {
    //     left_tilt(); 
    //   } else if (val==14) {
    //     right_tilt();
    //   } else if (val==15) {
    //     rise(); 
    //   } else if (val==16) {
    //     dance();
    //   } else if (val==17) {
    //     state = true;
    //     xTaskCreate(motionTask_avoid, "Motion Task avoid", 4096, NULL, 5, NULL);
    //   } else if (val==18) {
    //     state = true;
    //     xTaskCreate(motionTask_follow, "Motion Task following", 4096, NULL, 5, NULL);
    //   }

    } else { 
      // Serial.println("variable");
      res = -1; 
    }

    if(res){ return httpd_resp_send_500(req); }

    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    return httpd_resp_send(req, NULL, 0);
}

//*************************Web_HTML*************************
static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!doctype html>
<html>
      <head>
          <meta charset="utf-8">
          <meta name="viewport" content="width=device-width,initial-scale=1">
          <title>Biped Robot</title>
          <style>

            input[type=range] {
                -webkit-appearance: none;
                width: 80%;
                height: 10px;
                background: #ccc;
                cursor: pointer;
                margin: 10px;
            }

            input[type=range]::-webkit-slider-thumb {
                -webkit-appearance: none;
                width: 20px;
                height: 20px;
                background: #ff3034;
                cursor: pointer;
                border-radius: 50%;
            }

              *{
                  padding: 0; margin: 0;
                  font-family:monospace;
              }

              *{  
                  -webkit-touch-callout:none;  
                  -webkit-user-select:none;  
                  -khtml-user-select:none;  
                  -moz-user-select:none;  
                  -ms-user-select:none;  
                  user-select:none;  
              }

          canvas {
          margin: auto;
          display: block;

          }
          .tITULO{
              text-align: center;
              color: rgb(97, 97, 97);
              
          }
          .LINK{
              color: red;
              width: 60px;
              margin: auto;
              display: block;
              font-size: 14px;
          }
          .cont_flex{
              margin: 20px auto 20px;
              width: 70%;
              max-width: 400px;
              display: flex;
              flex-wrap: wrap;
              justify-content: space-around;
          }
          .cont_flex button{
              width: 80px;
              height: 35px;
              border: none;
              background-color: #3D9EFF;
              border-radius: 10px;
              color: white;

          }
          .cont_flex button:active{
              background-color: #0080FF;
          }

          .cont_flex5{
              margin: 20px auto 20px;
              width: 100%;
              max-width: 400px;
              display: flex;
              flex-wrap: wrap;
              justify-content: space-around;
          }
          .cont_flex5 button{
              width: 280px;
              height: 35px;
              border: none;
              background-color: #3D9EFF;
              border-radius: 10px;
              color: white;
          }

          .cont_flex5 button:active{
              background-color: #0080FF;
          }

          .custom-alert {
            position: fixed;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            background-color: lightskyblue;
            padding: 20px;
            border: 1px solid gray;
            border-radius: 5px;
            animation: fadeInOut 2s ease-in-out forwards;
            opacity: 0; 
            visibility: hidden; 
            }

          input{-webkit-user-select:auto;} 
          input[type=range]{-webkit-appearance:none;width:300px;height:25px;background:#cecece;cursor:pointer;margin:0}
          input[type=range]:focus{outline:0}
          input[type=range]::-webkit-slider-runnable-track{width:100%;height:2px;cursor:pointer;background:#EFEFEF;border-radius:0;border:0 solid #EFEFEF}
          input[type=range]::-webkit-slider-thumb{border:1px solid rgba(0,0,30,0);height:22px;width:22px;border-radius:50px;background:#ff3034;cursor:pointer;-webkit-appearance:none;margin-top:-10px}

          </style>
      </head>

      <body>
          <div id="customAlert" class="custom-alert">
            <p id="alertText" style="color: white; font-size: 15px;"></p>
          </div>

          <p style="color: black; display: flex; justify-content: center; align-items: center; font-size: 25px;">Biped Robot</p>   

          <div class="cont_flex">     
              <button type="button" id="Forward" ontouchstart="ForwardSending('1')" onmousedown="ForwardSending('1')" ontouchend="stopSending()" onmouseup="stopSending()">Forward</button>
          </div>

          <div class="cont_flex">     
              <button type="button" id="turn_left" ontouchstart="turnleftSending('3')" onmousedown="turnleftSending('3')" ontouchend="stopSending()" onmouseup="stopSending()">Turn<br>Left</button>

              <button type="button" id="Backward" ontouchstart="BackwardSending('2')" onmousedown="BackwardSending('2')" ontouchend="stopSending()" onmouseup="stopSending()">Backward</button>

              <button type="button" id="turn_right" ontouchstart="turnrightSending('4')" onmousedown="turnrightSending('4')" ontouchend="stopSending()" onmouseup="stopSending()">Turn<br>Right</button>  
          </div>

          <div class="cont_flex">     
              
          </div>

          <p><br></p>

          <p style="color: black; display: flex; justify-content: center; align-items: center; font-size: 25px;">Sports Mode</p>   

          <div class="cont_flex">   
              <button type="button" id="Rub" ontouchstart="fetch(document.location.origin+'/control?var=robot&val=11');" onmousedown="fetch(document.location.origin+'/control?var=robot&val=11');" ontouchend="stopSending()" onmouseup="stopSending()">Left<br>Kick</button>

              <button type="button" id="Shadows" ontouchstart="fetch(document.location.origin+'/control?var=robot&val=10');" onmousedown="fetch(document.location.origin+'/control?var=robot&val=10');">Sprint</button>

              <button type="button" id="Swing" ontouchstart="fetch(document.location.origin+'/control?var=robot&val=12');" onmousedown="fetch(document.location.origin+'/control?var=robot&val=12');" ontouchend="stopSending()" onmouseup="stopSending()">Right<br>Kick</button>
          </div>

          <div class="cont_flex">   
              <button type="button" id="left_tilt" onmousedown="fetch(document.location.origin+'/control?var=robot&val=13');" ontouchstart="fetch(document.location.origin+'/control?var=robot&val=13');" >Left<br>Tilt</button>

              <button type="button" id="Dancing" onmousedown="fetch(document.location.origin+'/control?var=robot&val=16');" ontouchstart="fetch(document.location.origin+'/control?var=robot&val=16');">Dance</button>

              <button type="button" id="right_tilt" onmousedown="fetch(document.location.origin+'/control?var=robot&val=14');" ontouchstart="fetch(document.location.origin+'/control?var=robot&val=14');" >Right<br>Tilt</button>
          </div>

          <div class="cont_flex">   
              <button type="button" id="test1" onmousedown="fetch(document.location.origin+'/control?var=robot&val=19');" ontouchstart="fetch(document.location.origin+'/control?var=robot&val=19');" >Left<br>Ankles</button>

              <button type="button" id="Following" onmousedown="fetch(document.location.origin+'/control?var=robot&val=18');" ontouchend="fetch(document.location.origin+'/control?var=robot&val=18');">Follow</button>

              <button type="button" id="test3" onmousedown="fetch(document.location.origin+'/control?var=robot&val=21');" ontouchstart="fetch(document.location.origin+'/control?var=robot&val=21');" >Right<br>Ankles</button>
          </div>

          <div class="cont_flex">   
              <button type="button" id="Rise" onmousedown="fetch(document.location.origin+'/control?var=robot&val=15');" ontouchstart="fetch(document.location.origin+'/control?var=robot&val=15');" ontouchend="stopSending()">Left<br>Stamp</button>

              <button type="button" id="avoid" onmousedown="fetch(document.location.origin+'/control?var=robot&val=17');" ontouchend="fetch(document.location.origin+'/control?var=robot&val=17');">Avoid</button>

              <button type="button" id="test2" onmousedown="fetch(document.location.origin+'/control?var=robot&val=20');" ontouchstart="fetch(document.location.origin+'/control?var=robot&val=20');" >Right<br>Stamp</button>
          </div>

          <div class="cont_flex5">   
              <button type="button" id="Stop" onmouseup="fetch(document.location.origin+'/control?var=robot&val=8');" ontouchend="fetch(document.location.origin+'/control?var=robot&val=8');">Stop</button>
          </div>

  

          <p><br></p>

          

          <script>
              let intervalId;

              function ForwardSending(value) {
                fetch(document.location.origin+'/control?var=robot&val=' + 1);
                intervalId = setInterval(() => {
                  fetch(document.location.origin+'/control?var=robot&val=' + value);
                }, 1200);
              }

              function turnleftSending(value) {
                fetch(document.location.origin+'/control?var=robot&val=' + 3);
                intervalId = setInterval(() => {
                  fetch(document.location.origin+'/control?var=robot&val=' + value);
                }, 1200);
              }

              function BackwardSending(value) {
                fetch(document.location.origin+'/control?var=robot&val=' + 2);
                intervalId = setInterval(() => {
                  fetch(document.location.origin+'/control?var=robot&val=' + value);
                }, 1200);
              }

              function turnrightSending(value) {
                fetch(document.location.origin+'/control?var=robot&val=' + 4);
                intervalId = setInterval(() => {
                  fetch(document.location.origin+'/control?var=robot&val=' + value);
                }, 1200);
              }

              function RubSending(value) { //踢腿
                fetch(document.location.origin+'/control?var=robot&val=' + 11);
                intervalId = setInterval(() => {
                  fetch(document.location.origin+'/control?var=robot&val=' + value);
                }, 1400);
              }

              function SwingSending(value) {
                fetch(document.location.origin+'/control?var=robot&val=' + 12);
                intervalId = setInterval(() => {
                  fetch(document.location.origin+'/control?var=robot&val=' + value);
                }, 1400);
              }

              function left_tiltSending(value) {
                fetch(document.location.origin+'/control?var=robot&val=' + 13);
                intervalId = setInterval(() => {
                  fetch(document.location.origin+'/control?var=robot&val=' + value);
                }, 5000);
              }

              function right_tiltSending(value) {
                fetch(document.location.origin+'/control?var=robot&val=' + 14);
                intervalId = setInterval(() => {
                  fetch(document.location.origin+'/control?var=robot&val=' + value);
                }, 5000);
              }

              function RiseSending(value) {
                fetch(document.location.origin+'/control?var=robot&val=' + 15);
                intervalId = setInterval(() => {
                  fetch(document.location.origin+'/control?var=robot&val=' + value);
                }, 2400);
              }

              function stopSending() {
                clearInterval(intervalId);
                fetch(document.location.origin+'/control?var=robot&val=8');
              }

              window.onload = function(){
                  var canvas = document.getElementById("canvas");
                  var ctx = canvas.getContext("2d");

                  ctx.fillStyle = "rgb(255,0,0)";
                  ctx.fillRect(73,25,60,35);
                  ctx.clearRect(78,30,50,25);

                  ctx.fillRect(93,20,20,5);
                  ctx.fillRect(68,35,5,15);
                  ctx.fillRect(133,35,5,15);

                  ctx.beginPath();
                  ctx.arc(92,42,6,0,2*Math.PI,true);
                  ctx.fill();

                  ctx.beginPath();
                  ctx.arc(117,42,6,0,2*Math.PI,true);
                  ctx.fill();

                  ctx.beginPath();
                  ctx.arc(104,100,35,0,Math.PI,true);
                  ctx.fill();

                  ctx.clearRect(50,85,100,20);

              }
          
              document.addEventListener(
              'DOMContentLoaded',function(){
                  function b(B){let C;switch(B.type){case'checkbox':C=B.checked?1:0;break;case'range':case'select-one':C=B.value;break;case'button':case'submit':C='1';break;default:return;}const D=`${c}/control?var=${B.id}&val=${C}`;fetch(D).then(E=>{console.log(`request to ${D} finished, status: ${E.status}`)})}var c=document.location.origin;const e=B=>{B.classList.add('hidden')},f=B=>{B.classList.remove('hidden')},g=B=>{B.classList.add('disabled'),B.disabled=!0},h=B=>{B.classList.remove('disabled'),B.disabled=!1},i=(B,C,D)=>{D=!(null!=D)||D;let E;'checkbox'===B.type?(E=B.checked,C=!!C,B.checked=C):(E=B.value,B.value=C),D&&E!==C?b(B):!D&&('aec'===B.id?C?e(v):f(v):'agc'===B.id?C?(f(t),e(s)):(e(t),f(s)):'awb_gain'===B.id?C?f(x):e(x):'face_recognize'===B.id&&(C?h(n):g(n)))};document.querySelectorAll('.close').forEach(B=>{B.onclick=()=>{e(B.parentNode)}}),fetch(`${c}/status`).then(function(B){return B.json()}).then(function(B){document.querySelectorAll('.default-action').forEach(C=>{i(C,B[C.id],!1)})});const j=document.getElementById('stream'),k=document.getElementById('stream-container'),l=document.getElementById('get-still'),m=document.getElementById('toggle-stream'),n=document.getElementById('face_enroll'),o=document.getElementById('close-stream'),p=()=>{window.stop(),m.innerHTML='Start Stream'},q=()=>{j.src=`${c+':81'}/stream`,f(k),m.innerHTML='Stop Stream'};l.onclick=()=>{p(),j.src=`${c}/capture?_cb=${Date.now()}`,f(k)},o.onclick=()=>{p(),e(k)},m.onclick=()=>{const B='Stop Stream'===m.innerHTML;B?p():q()},n.onclick=()=>{b(n)},document.querySelectorAll('.default-action').forEach(B=>{B.onchange=()=>b(B)});const r=document.getElementById('agc'),s=document.getElementById('agc_gain-group'),t=document.getElementById('gainceiling-group');r.onchange=()=>{b(r),r.checked?(f(t),e(s)):(e(t),f(s))};const u=document.getElementById('aec'),v=document.getElementById('aec_value-group');u.onchange=()=>{b(u),u.checked?e(v):f(v)};const w=document.getElementById('awb_gain'),x=document.getElementById('wb_mode-group');w.onchange=()=>{b(w),w.checked?f(x):e(x)};const y=document.getElementById('face_detect'),z=document.getElementById('face_recognize'),A=document.getElementById('framesize');A.onchange=()=>{b(A),5<A.value&&(i(y,!1),i(z,!1))},y.onchange=()=>{return 5<A.value?(alert('Please select CIF or lower resolution before enabling this feature!'),void i(y,!1)):void(b(y),!y.checked&&(g(n),i(z,!1)))},z.onchange=()=>{return 5<A.value?(alert('Please select CIF or lower resolution before enabling this feature!'),void i(z,!1)):void(b(z),z.checked?(h(n),i(y,!0)):g(n))}});
          
          </script>
      </body>
</html>
)rawliteral";

static esp_err_t index_handler(httpd_req_t *req){
    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, (const char *)INDEX_HTML, strlen(INDEX_HTML));
}

void ACB_Biped_Robot_WiFi::startWebServer() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    
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
    
    Serial.printf("Starting web server on port: '%d'\n", config.server_port);
    if (httpd_start(&camera_httpd, &config) == ESP_OK) {
        httpd_register_uri_handler(camera_httpd, &index_uri);
        httpd_register_uri_handler(camera_httpd, &cmd_uri);
    }
}

void ACB_Biped_Robot_WiFi::startAppServer() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    
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
    
    Serial.printf("Starting web server on port: '%d'\n", config.server_port);
    if (httpd_start(&camera_httpd, &config) == ESP_OK) {
        // httpd_register_uri_handler(camera_httpd, &index_uri);
        httpd_register_uri_handler(camera_httpd, &cmd_uri);
    }
}
