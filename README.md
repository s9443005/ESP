# 物聯網專題 Instructor: Dr. JOSEPH CW SHIH
## ESP開發板WEB程式設計
* 本課程為整合性課程，重點在於WEB Server程式設計，有別像是電子系以電子電路及感測器為主的學習內容。
* 先備條件：具程式設計熱忱，具HTML、CSS、JavaScript最基本基礎、修過網路概論課程佳。
* 模組一：為基礎ARDUINO程式開發，以LED燈及蜂鳴器做為成果，學生必須繳交驗收作業。
* 模組二：以開發板啟動HTTP服務，在CLIENT端（Bowoser）控制LED燈，學生必須繳交驗收作業。
* 模組三：以開發啟啟動HTTP服務，連結MySQL資料庫，在CLIENT端（Bowoser）存取DB。
* 模組四：介紹重要的WEB新型進階的網路協定，延伸開發板的應用能力。
* 感測器模組：在課程間穿插幾個搭配課程的感測器。
## 模組一
課程時間：9小時
### LESSON_01 認識開發板
ESP8266 D1 mini 是一塊廠商已經整合好的ESP8266開發板。
### LESSON_02 內建LED BLINK
  > 範例影片
  > 範例說明，[按我](https://electrosome.com/esp8266-arduino-programming-led-blink/)
### LESSON_03 自建LED
### LESSON_04 課堂作業--LED燈光秀
### LESSON_05 蜂鳴器-有源及無源
### LESSON_06 驗收作業--聲光秀
## 模組二
課程時間：18小時
### LESSON_01 WEB Server控制2顆LED開關
* GPIO5(D1) GPIO4(D2)各有一個LED燈
* 加入 GPIO3(RX) 第3個LED燈
* 以下在Serial Monitor輸出TCP/IP環境
```
  Serial.println("TCP/IP Configuration: ");
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.subnetMask());
  Serial.println(WiFi.gatewayIP());
  Serial.println(WiFi.dnsIP());
```
* 解析 *http://192.168.50.115/5/on* 及相關
```
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("GPIO 5 on");
              output5State = "on";
              digitalWrite(output5, HIGH);
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO 5 off");
              output5State = "off";
              digitalWrite(output5, LOW);
            }
```
* 解析 HTML 及其相關
```
            client.println("<p>GPIO 5 - State " + output5State + "</p>");
            // If the output5State is off, it displays the ON button       
            if (output5State=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
```

### LESSON_02 課堂作業--WEB Server控制3顆LED開關
### LESSON_03 WEB Server控制1顆LED亮度-使用RANGE物件
### LESSON_04 課堂作業--WEB Server控制2顆LED亮度-使用RANGE物件
### LESSON_05 HTTP協定參數傳遞
* 直接在瀏覽器讀取 PWM WEB SLIDER 的燈泡亮度
* http://192.168.50.115/ask/
* 是 __request->send()__，不是 request->send_p()
```
  server.on("/ask", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", sliderValue);
  });
```

### LESSON_06 儀表板基礎
* ESP Integrated Dashboard Server
```
  /*********
    Adapted from Rui Santos, Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-web-server-slider-pwm/
    結合了2支範例程式
  *********/
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>

  const char* ssid = "F6314";
  const char* password = "0912540452";
  unsigned long LED1_lastTime = 0;
  unsigned long timerDelay = 1000;
  // PWM Slider Server增加一個route /ask
  String LED1_serverName = "http://192.168.50.115/ask";
  String _LED1;

  IPAddress local_IP(192, 168, 50, 250); 
  IPAddress gateway(192, 168, 50, 1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress primaryDNS(8, 8, 8, 8);   //optional
  IPAddress secondaryDNS(8, 8, 4, 4); //optional

  AsyncWebServer server(80);

  const char index_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML><html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta charset="utf-8">
    <meta http-equiv="refresh" content="1">
    <title>ESP Web Dashboard Server</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css" rel="stylesheet">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/js/bootstrap.bundle.min.js"></script>
  </head>
  <body>
    <h1 style='text-align: center'>ESP Web Server Dashboard</h1><hr>
    <div class="container">
      <div class="row">
        <div class="card shadow p-3 m-3" style='width: 18rem;'>
          <div class="card-body">
            <h5 class="card-title">192.168.50.115</h5>
            <p class="card-text">LED亮度%LED1%</p>
            <a href="#" class="btn btn-primary">立即更新</a>
          </div>
        </div>
      </div>
    </div>
  </body>
  </html>
  )rawliteral";

  String processor(const String& var){
    //Serial.println(var);
    if (var == "LED1"){return _LED1;}
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
          Serial.println("_LED1=" + _LED1);
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
  }
```
以上程式應學習的重點有：
1. 本機自訂IP環境
2. 使用bootstrap5- Card
3. %LED1% 為 placehold，由 processor() 填入亮度字串
4. 每1秒更新頁面1次
5. 每隔 timerDelay 秒(1秒) 去讀取 LED 亮度



### LESSON_07 驗收作業--(A)伺服馬達 OR (B)溫濕度感測器DHT11

## 模組三
課程時間：12小時
### LESSON_01 MySQL及PHP Script基礎
### LESSON_02 WEB Server讀取溫濕度寫入MySQL
### LESSON_03 驗收作業--讀取溫濕度及氣壓寫入MySQL
### LeSson_04 WEB Server讀取

## 模組四
課程時間：12小時
### LESSON_01 WEB SOCKET協定
### LESSON_02 MQTT協定
### LESSON_03 Ad-hoc網路以及 MESH 實作
## 感測器模組
課程時間：3小時
### LESSON_01 溫濕度感測器DHT11
### LESSON_02 火焰感測器
### LESSON_03 伺服馬達
## 儀表板模組
### LESSON1 HTML meter 標籤
  > [W3SCHOOL教學](https://www.w3schools.com/tags/tag_meter.asp)
### LESSON2 CSS測量儀Gauge  
* [YOUTUBE教學影片1](https://www.google.com/search?q=HTML+CSS+gauge+tutorial&sxsrf=AJOqlzWQg46vvdK_PiWPPrHPNM37Wj300g%3A1674545368260&ei=2IjPY5XDD-Dk2roPzI6pgA8&ved=0ahUKEwiVj9fO19_8AhVgslYBHUxHCvAQ4dUDCA8&uact=5&oq=HTML+CSS+gauge+tutorial&gs_lcp=Cgxnd3Mtd2l6LXNlcnAQAzIICCEQoAEQwwQyCAghEKABEMMEOgoIABBHENYEELADOgYIABAHEB46BwgAEA0QgAQ6CAgAEAgQBxAeOgoIABAIEAcQHhAKOgYIABAeEA06CgghEKABEMMEEApKBAhBGABKBAhGGABQmQxYvxZg1BhoAnABeACAAWeIAYADkgEDNS4xmAEAoAEByAEKwAEB&sclient=gws-wiz-serp#fpstate=ive&vld=cid:4e7be008,vid:FnUkVcQ_3CQ)
* [YOUTUBE教學影片2-推薦](https://www.google.com/search?q=CSS+gauge+tutorial&oq=CSS+gauge+tutorial&aqs=chrome..69i57j69i61j69i60l2.9074j0j15&sourceid=chrome&ie=UTF-8#fpstate=ive&vld=cid:d774bf36,vid:4P1fhmDNHbk)
## MySQL+PHP Author:Dr. JOSEPH CW SHIH
參考來源：W3SCHOOL TUTORIAL
### Connecting to Database -- ex01.php
程式說明：使用PHP連接MySQL伺服器
```
<?php
$servername = "localhost";
$username = "root";
$password = "";

//建立和MySQL DB連線，使用SQLi物件導向
  <?php
  $servername = "localhost";
  $username = "root";
  $password = "";

  //建立和MySQL DB連線，使用SQLi物件導向
  $conn = new mysqli($servername, $username, $password);
  // 檢查連線
  if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
  }
  echo "<p>Connected successfully資料庫連線成功</p>";
  $conn->close();
  echo "<p>Connected close資料庫斷線</p>";
  ?>
```
### Creating a Database -- ex02.php
程式說明：使用PHP連接在MySQL伺服器建立一個資料庫叫做mydb
```
  <?php
  $servername = "localhost";
  $username = "root";
  $password = "";


  $conn = new mysqli($servername, $username, $password);
  // Check connection
  if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
  }

  // 在MySQL裡新增一個資料庫
  $sql = "CREATE DATABASE myDB";
  if ($conn->query($sql) === TRUE) {
    echo "<p>Table Created successfully資料庫建立成功</p>";
  } else {
    echo "<p>Error creating database: " . $conn->error . "<p>";
  }
  $conn->close();
  echo "<p>Connected close資料庫斷線</p>";
  ?>
```
### Creating a Table -- ex03.php
程式說明：使用PHP在資料庫mydb建立一張表格叫做myguests
```
  <?php
  $servername = "localhost";
  $username = "root";
  $password = "";
  $dbname = "myDB";

  //這個連線不僅連伺服器，直接叫到資料庫，
  $conn = new mysqli($servername, $username, $password, $dbname);
  // Check connection
  if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
  }

  //使用DDL語言，在mydb資料庫裡建立一張表格叫myguests
  //DDL是SQL其中一種
  $sql = "CREATE TABLE MyGuests (
  id INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY,
  firstname VARCHAR(30) NOT NULL,
  lastname VARCHAR(30) NOT NULL,
  email VARCHAR(50),
  reg_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
  )";

  if ($conn->query($sql) === TRUE) {
    echo "<p>Table MyGuests created successfully表格建立成功<p>";
  } else {
    echo "<p>Error creating table: " . $conn->error . "</p>"; 
  }
  $conn->close();
  echo "<p>Connected close資料庫斷線</p>";
  ?>
```
### Insert a Record/Multiple Records -- ex04.php
程式說明：使用PHP在表格myguests裡新增1筆記錄
```
  <?php
  $servername = "localhost";
  $username = "root";
  $password = "";
  $dbname = "myDB";

  // Create connection
  $conn = new mysqli($servername, $username, $password, $dbname);
  // Check connection
  if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
  }
  //使用DML語言，在表格myguests裡新增新增1筆記錄
  //DML是SQL其中一種
  $sql = "INSERT INTO MyGuests (firstname, lastname, email)
  VALUES ('John', 'Doe', 'john@example.com')";

  if ($conn->query($sql) === TRUE) {
    echo "<p>New record created successfully新增一筆記錄成功</p>";
  } else {
    echo "Error: " . $sql . "<br>" . $conn->error;
  }
  $conn->close();
  echo "<p>Connected close資料庫斷線</p>";
  ?>
```
### Insert Multiple Records -- ex05.php


## 需要材料
### 每位學生
* ESP8266 X1 建議 D1 mini [參考圖片](https://shop.mirotek.com.tw/shop/200022b/) 或 ESP32 (建議 Dev Kit v1)
* USB連接線 X1 TypeA + mini USB，一般手機充電線即可
* 麵包板 X1 [參考圖片](https://shopee.tw/【666】A29-麵包板-免焊式測試電路板-實驗板配跳線-400孔-Arduino-i.10398240.69894313)
* 杜邦線 X若干 (公公 及 公母)
  >以上項目於第五週起由學生自備，以下項目由課程提供
* LED燈X8
* 有源蜂鳴器X1 及 無源蜂鳴器X1
* 溫濕度感測器DHT11X1 或 溫濕度壓力感測器BME280X1
### 教室
* MySQL伺服器X1 (XAMPP)
* IP分享器X1
