# 物聯網專題
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
### LESSON_02 課堂作業--WEB Server控制3顆LED開關
### LESSON_03 WEB Server控制1顆LED亮度-使用RANGE物件
### LESSON_04 課堂作業--WEB Server控制2顆LED亮度-使用RANGE物件
### LESSON_05 HTTP協定參數傳遞
### LESSON_06 儀表板基礎
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
