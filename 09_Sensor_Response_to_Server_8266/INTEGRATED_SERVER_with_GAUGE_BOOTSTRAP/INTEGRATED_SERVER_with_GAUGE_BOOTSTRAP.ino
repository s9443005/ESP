/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-web-server-slider-pwm/
*********/
#include <ESP8266WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
const char* ssid = "JOSEPH";
const char* password = "0912540452";

IPAddress local_IP(192, 168, 1, 215);
IPAddress gateway(192, 168, 1, 1);      
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

String _LED1; //RangeValue from 192.168.1.111
String _LED2; //RangeValue from 192.168.1.112
String _LED1deg; //RangeValue from 192.168.1.111
String _LED2deg; //RangeValue from 192.168.1.112
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <! meta http-equiv="refresh" content="5">
  <title>ESP32 Integrated Server</title>
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 2.3rem;}
    p {font-size: 1.9rem;}
    body {max-width: 400px; margin:0px auto; padding-bottom: 25px;}
    .slider { -webkit-appearance: none; margin: 14px; width: 360px; height: 25px; background: #FFD65C;
      outline: none; -webkit-transition: .2s; transition: opacity .2s; font-size: 2.0rem;}
    .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background: #003249; cursor: pointer;}
    .slider::-moz-range-thumb { width: 35px; height: 35px; background: #003249; cursor: pointer; } 
    .box1 {display: block; font-size: 1.2rem; margin: 50px; border: 3px solid blue; padding: 25px;}
    .meter {display: block; font-size 1.2rem; margin: auto;}
    .gauge {position: relative; border-radius: 50%/100% 100% 0 0; background-color: var(--color, #a22); overflow: hidden;}
    .gauge:before{content: ""; display: block; padding-top: 50%;}
    .gauge .chart {overflow: hidden;}
    .gauge .mask {position: absolute; left: 20%; right: 20%; bottom: 0; top: 40%; background-color: #fff; border-radius: 50%/100% 100% 0 0;}
    .gauge .percentage {position:  absolute; top: -1px; left: -1px; bottom: 0; right: -1px; background-color: var(--background, #aaa);
                        transform:rotate(var(--rotation)); transform-origin: bottom center; transition-duration: 600;}
    .gauge:hover {--rotation: 100deg;}
    .gauge .value {position:absolute; bottom:0%; left:0; width:100%; text-align: center;}


  </style>
</head>
<body>
  <h2>ESP32 Integrated Server <span id="LocalIP">%LOCALIP%</span></h2>
  <div class="box1">Server 192.168.1.111 LED value:<span>%LED1%</span></div>
  <div class="box1">Server 192.168.1.112 LED value:<span>%LED2%</span></div>
  <div class="meter"><label for="meter">Server 192.168.1.111</label><meter value="%LED1%" max="255" low="63"></meter></div>
  <div class="meter"><label for="meter">Server 192.168.1.113</label><meter value="%LED2%" max="255" low="63"></meter></div>
  <div class="gauge" style="width: 200px; --rotation:135deg; --color:#5cb85c; --background:#e9ecef;">
    <div class="percentage"></div><div class="mask"></div><span class="value">201</span>
  </div>
  <div class="gauge" style="width: 200px; --rotation:135deg; --color:#5cb85c; --background:#e9ecef;">
    <div class="percentage"></div><div class="mask"></div><span class="value">109</span>
  </div>
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if (var == "LOCALIP"){return WiFi.localIP().toString();}
  if (var == "LED111"){return _LED111;}
  if (var == "LED113"){return _LED113;}
  return String();
}

void setup(){
  Serial.begin(115200);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure TCP/IP.");
  }
  //己設固定IP 192.168.1.200/24
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.subnetMask());
  Serial.println(WiFi.gatewayIP());
  Serial.println(WiFi.dnsIP());

  // Route for root / web page
  //server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  //  request->send_P(200, "text/html", index_html, processor);
  //});

  server.on("/", HTTP_GET, [] (AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/together", HTTP_GET, [] (AsyncWebServerRequest *request) {
    Serial.println("COMING IN");
    if (request->hasParam("sip") && request->hasParam("value")) {
      if (request->getParam("sip")->value() == "192.168.1.111") {
        _LED111 = request->getParam("value")->value();
        Serial.println("Receiving LED 111 RangeValue " + _LED111);
      } else if (request->getParam("sip")->value() == "192.168.1.113") {
        _LED113 = request->getParam("value")->value();
        Serial.println("Receiving LED 113 RangeValue " + _LED113);
      }
    }
    else {Serial.println("No message or incompleted message sent.");}
    request->send(200, "text/plain", "Transmission OK");

  });
  // Start server
  server.begin();
}
  
void loop() {}