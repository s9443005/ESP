/*********
  AUTHOR: Dr.CHIAO-WANG SHIH施教旺, modified from below:
  Rui Santos Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-web-server-slider-pwm/
  3顆LED燈接在GPIO5、GPIO4、GIOP3(RX)，使用WEBPAGE來控制明亮程度
*********/

// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "F6314";
const char* password = "0912540452";

const int output5 = 5;
const int output4 = 4;
const int output3 = 3;

String sliderValue5 = "0";
String sliderValue4 = "0";
String sliderValue3 = "0";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP Web Server</title>
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 2.3rem;}
    p {font-size: 1.9rem;}
    body {max-width: 400px; margin:0px auto; padding-bottom: 25px;}
    .slider { -webkit-appearance: none; margin: 14px; width: 360px; height: 25px; background: #FFD65C;
      outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background: #003249; cursor: pointer;}
    .slider::-moz-range-thumb { width: 35px; height: 35px; background: #003249; cursor: pointer; } 
  </style>
</head>
<body>
  <h2>ESP Web Server</h2>
  <p>LED5:<span id="textSliderValue5">%SLIDERVALUE5%</span></p>
  <p><input type="range" onchange="updateSliderPWM5(this)" id="pwmSlider5" min="0" max="255" value="%SLIDERVALUE5%" step="1" class="slider"></p>
  <p>LED4:<span id="textSliderValue4">%SLIDERVALUE4%</span></p>
  <p><input type="range" onchange="updateSliderPWM4(this)" id="pwmSlider4" min="0" max="255" value="%SLIDERVALUE4%" step="1" class="slider"></p>
  <p>LED3:<span id="textSliderValue3">%SLIDERVALUE3%</span></p>
  <p><input type="range" onchange="updateSliderPWM3(this)" id="pwmSlider3" min="0" max="255" value="%SLIDERVALUE3%" step="1" class="slider"></p>
<script>
function updateSliderPWM5(element) {
  var sliderValue5 = document.getElementById("pwmSlider5").value;
  document.getElementById("textSliderValue5").innerHTML = sliderValue5;
  console.log(sliderValue5);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value=" + sliderValue5 + "&LED=5", true);
  xhr.send();
}
function updateSliderPWM4(element) {
  var sliderValue4 = document.getElementById("pwmSlider4").value;
  document.getElementById("textSliderValue4").innerHTML = sliderValue4;
  console.log(sliderValue4);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value="+sliderValue4 + "&LED=4", true);
  xhr.send();
}
function updateSliderPWM3(element) {
  var sliderValue3 = document.getElementById("pwmSlider3").value;
  document.getElementById("textSliderValue3").innerHTML = sliderValue3;
  console.log(sliderValue3);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value="+sliderValue3 + "&LED=3", true);
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if (var == "SLIDERVALUE5"){return sliderValue5;}
  if (var == "SLIDERVALUE4"){return sliderValue4;}
  if (var == "SLIDERVALUE3"){return sliderValue3;}
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  analogWrite(output5, sliderValue5.toInt());
  analogWrite(output4, sliderValue4.toInt());
  analogWrite(output3, sliderValue4.toInt());

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP Local TCP/IP Configuration
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.subnetMask());
  Serial.println(WiFi.gatewayIP());
  Serial.println(WiFi.dnsIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if (request->hasParam("value") && request->hasParam("LED")) {
      if (request->getParam("LED")->value()=="5"){
        sliderValue5 = request->getParam("value")->value();
        analogWrite(output5, sliderValue5.toInt());
        Serial.println("LED5_" + sliderValue5);
      }
      else if (request->getParam("LED")->value()=="4"){
        sliderValue4 = request->getParam("value")->value();
        analogWrite(output4, sliderValue4.toInt());
        Serial.println("LED4_" + sliderValue4);
      }
      else if (request->getParam("LED")->value()=="3"){
        sliderValue3 = request->getParam("value")->value();
        analogWrite(output3, sliderValue3.toInt());
        Serial.println("LED3_" + sliderValue4);
      }      
    }
    else {
      Serial.println("Input incompleted");
    }

    request->send(200, "text/plain", "OK");
  });
  
  // Start server
  server.begin();
}
  
void loop() {
  
}