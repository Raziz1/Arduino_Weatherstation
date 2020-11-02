# Arduino_WeatherStation ☁
Arduino UnoR3 + TFT Display + Esp8266 (Esp-01)

## Parts 🔧🔨
* [Arduino Elegoo Super Starter Kit](https://www.amazon.ca/Elegoo-Project-Starter-Tutorial-Arduino/dp/B01D8KOZF4/ref=sr_1_1_sspa?dchild=1&keywords=arduino&qid=1603991079&s=electronics&sr=1-1-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUEyNTlMVTJPSjZBUklUJmVuY3J5cHRlZElkPUEwNDYwNTY1MVhHMUM4TklJS1NIJmVuY3J5cHRlZEFkSWQ9QTA5MTU0MDIyTlVaRkYwNUZNRFY1JndpZGdldE5hbWU9c3BfYXRmJmFjdGlvbj1jbGlja1JlZGlyZWN0JmRvTm90TG9nQ2xpY2s9dHJ1ZQ==)
  - [Arduino Elegoo Tutorial Files](https://www.elegoo.com/pages/arduino-kits-support-files)
* [Diymall Esp8266 (Esp-01) WiFi Module](https://www.amazon.ca/Diymall%C2%AE-Esp8266-Wireless-Transceiver-Mega2560/dp/B00O34AGSU/ref=sr_1_1?dchild=1&keywords=diymall+esp8266&qid=1603991104&s=electronics&sr=1-1)
  - [Technical Overview](https://nurdspace.nl/ESP8266#Introduction)
  - [Youtube Tutorial](https://www.youtube.com/watch?v=qU76yWHeQuw)
* [Kuman TFT Display Shield](https://www.amazon.ca/Kuman-Arduino-Screen-Tutorials-Mega2560/dp/B075FP83V5/ref=sr_1_10?dchild=1&keywords=TFT+display&qid=1604343441&sr=8-10)
  - [Technical Overview](http://www.lcdwiki.com/3.5inch_Arduino_Display-UNO)

## Libraries 📚
* [Esp8266 Library](https://github.com/sleemanj/ESP8266_Simple)
* Kuman TFT Display Shield Libraries
  - [LCDWIKI_gui](https://github.com/lcdwiki/LCDWIKI_gui)
  - [LCDWIKI_kbv](https://github.com/lcdwiki/LCDWIKI_kbv)
  - [LCDWIKI_SPI](https://github.com/lcdwiki/LCDWIKI_SPI)
  - [LCDWIKI_touch](https://github.com/lcdwiki/LCDWIKI_touch)
  
## Overview
This project assumes you have some basic knowledge when it comes to using both the Esp8266 WiFi Module and the Kuman TFT Display Shield.  
* This project uses OpenWeathers API to access online data. Go to OpenWeather.org to setup a free account and get your API key.
* In the "Weather_Scrapper_ESP8266.io" file you will have to change a fiew things.
  - Input your Wifi Network credentials in ssid & password
  - Input your API link with your city ID (& other parameters) as well as the API key
  - You can use [ArduinoJson](https://arduinojson.org/v5/assistant/) which is a parsing assistant to break down the Json file
  
 ## Schematics ⚡
<p> 
  <img align='Left' src="https://github.com/Raziz1/Arduino_WeatherStation/blob/main/images/Schematics.png? raw=true">
</p>
