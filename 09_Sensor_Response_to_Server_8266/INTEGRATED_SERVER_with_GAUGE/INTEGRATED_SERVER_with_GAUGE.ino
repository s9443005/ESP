/*********
  Adapted from Rui Santos, Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-web-server-slider-pwm/
  結合了2支範例程式
*********/
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "JOSEPH";
const char* password = "0912540452";
unsigned long LED1_lastTime = 0;
unsigned long LED2_lastTime = 0;
unsigned long timerDelay = 5000;
String LED1_serverName = "http://192.168.1.111/LED";
String LED2_serverName = "http://192.168.1.112/LED";
String _LED1; //RangeValue from 192.168.1.111
String _LED2; //RangeValue from 192.168.1.113
String _LED1deg;
String _LED2deg;

IPAddress local_IP(192, 168, 1, 115);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta charset="utf-8">
  <meta http-equiv="refresh" content="10">
  <title>ESP Web Server</title>
  <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css" rel="stylesheet">
  <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/js/bootstrap.bundle.min.js"></script>
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 1.6rem;}
    p {font-size: 1.2rem;}
    body {max-width: 800px; margin:0px auto; padding-bottom: 10px;}
    .cellA {background: #FFFFCC; padding: 25px;}
    .cellB {background: #E5FFCC; padding: 25px;}
    .slider { -webkit-appearance: none; margin: 14px; width: 360px; height: 25px; background: #FFD65C;
      outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background: #003249; cursor: pointer;}
    .slider::-moz-range-thumb { width: 35px; height: 35px; background: #003249; cursor: pointer; }
    .gauge {position: relative; border-radius: 50%%/100%% 100%% 0 0; background-color: var(--color, #a22); overflow: hidden;}
    .gauge:before{content: ""; display: block; padding-top: 50%%;}
    .gauge .chart {overflow: hidden;}
    .gauge .mask {position: absolute; left: 20%%; right: 20%%; bottom: 0; top: 40%%; background-color: #fff; border-radius: 50%%/100%% 100%% 0 0;}
    .gauge .percentage {position:  absolute; top: -1px; left: -1px; bottom: 0; right: -1px; background-color: var(--background, #aaa);
                        transform:rotate(var(--rotation)); transform-origin: bottom center; transition-duration: 600;}
    .gauge:hover {--rotation: 100deg;}
    .gauge .value {position:absolute; bottom:0%%; left:0; width:100%%; text-align: center;}
  </style>
</head>
<body>
  <div class="container-fluid p-5 bg-primary text-white text-center">
    <h2>ESP Web Server Dashboard</h2>
    <p>192.168.1.115</p>
  </div>
  <div class="container mt-5">
    <div class="row">
      <div class="col-sm-6">
        <div class=cellA><p>Server111 LED亮度:%LED1%</p></div>
      </div>
      <div class="col-sm-6">
        <div class=cellB><p>Server112 LED亮度:%LED2%</p></div>
      </div>
      <div class="col-sm-6">
        <div class=cellA><div class="meter"><label for="meter"><p>Server111亮度:%LED1%</label></p><p style="font-size: 2.0rem;"><meter value="%LED1%" max="255" low="63"></p></meter></div></div>
      </div>
      <div class="col-sm-6">
          <div class=cellB><div class="meter"><label for="meter"><p>Server112亮度:%LED2%</label></p><p style="font-size: 2.0rem;"><meter value="%LED2%" max="255" low="63"></p></meter></div></div>
      </div>
      <div class="col-sm-6">
        <div class=cellA style="display: flex; justify-content: center; padding: 50px;"><p>Server111</p><div class="gauge" style="width: 200px; --rotation:%LED1deg%deg; --color:#3399FF; --background:#e9ecef;">
          <div class="percentage"></div><div class="mask"></div><span class="value">亮度:%LED1%</span>
        </div></div>
      </div>
      <div class="col-sm-6">
        <div class=cellB style="display: flex; justify-content: center; padding: 50px;"><p>Server112</p><div class="gauge" style="width: 200px; --rotation:%LED2deg%deg; --color:#3399FF; --background:#e9ecef;">
          <div class="percentage"></div><div class="mask"></div><span class="value">亮度:%LED2%</span>
        </div></div>
      </div>
    </div>
  </div>
</body>
</html>
)rawliteral";

String processor(const String& var){
  //Serial.println(var);
  if (var == "LED1"){return _LED1;}
  if (var == "LED1deg"){return _LED1deg;}
  if (var == "LED2"){return _LED2;}
  if (var == "LED2deg"){return _LED2deg;}
  return String();
}

void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {Serial.println("STA Failed to configure TCP/IP.");}
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.subnetMask());
  Serial.println(WiFi.gatewayIP());
  Serial.println(WiFi.dnsIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/html", index_html, processor);});
  server.begin();
}
  
void loop() {
    if ((millis() - LED1_lastTime) > timerDelay) {
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      String serverPath = LED1_serverName;
      http.begin(client, serverPath.c_str());
      int httpResponseCode = http.GET();
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        _LED1 = http.getString();
        _LED1deg = 180*_LED1.toInt()/255;
        Serial.println("_LED1=" + _LED1);
        Serial.println("_LED1deg=" + _LED1deg);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    LED1_lastTime = millis();
  }
    if ((millis() - LED2_lastTime) > timerDelay) {
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      String serverPath = LED2_serverName;
      http.begin(client, serverPath.c_str());
      int httpResponseCode = http.GET();
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        _LED2 = http.getString();
        _LED2deg = 180*_LED2.toInt()/255;
        Serial.println("_LED2=" + _LED2);
        Serial.println("_LED2deg=" + _LED2deg);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    LED2_lastTime = millis();
  }
}