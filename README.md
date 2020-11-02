# Arduino_WeatherStation ☁
Arduino Uno R3 + TFT Display + Esp8266 (Esp-01)

<p> 
  <img align='Right' src="https://github.com/Raziz1/Arduino_WeatherStation/blob/main/images/WeatherStation.png? raw=true">
</p>

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
  <img align='Center' src="https://github.com/Raziz1/Arduino_WeatherStation/blob/main/images/Schematic.png? raw=true">
</p>

### Wiring TFT Display 
The TFT display shield consumes all the pins when directly mounted on the Arduino Uno R3 so instead follow this wiring scheme:
| TFT Display   | Arduino      |
| ------------- |:-------------:| 
| 5V            | 5V            | 
| 3.3V          | 3.3V          |   
| Ground        | Ground        | 
| LCD_DO        | 8             | 
| LCD_D1        | 9             | 
| LCD_D2        | 2             | 
| LCD_D3        | 3             | 
| LCD_D4        | 4             |
| LCD_D5        | 5             | 
| LCD_D6        | 6             | 
| LCD_D7        | 7             | 
| LCD_RST       | A4            | 
| LCD_CS        | A3            | 
| LCD_RS        | A2            | 
| LCD_WR        | A1            | 
| LCD_RD        | A0            | 
| SD_SS         | 10            | 
| SD_DI         | 11            | 
| SD_DO         | 12            | 
| SD_SCK        | 13            | 
* View the documentation of both the board and the GUI library to familiarize yourself with it. Walk throught the examples in the GUI library specifically the graphics and SD card examples. You can also reference the GUI documentation for syntax.

___

### Flashing Arduino Code on to Esp8266 (Esp-01) 📶
To flash the WiFi Module ensure you have the following wirings connected:
| Esp8266        | Arduino      |
| ------------- |:-------------:| 
| VCC           | 3.3v          | 
| Ground        | Ground        |   
| Tx            |Tx             | 
| Rx            |Rx             | 
| CHPD(Enable)  |3.3v           | 
| GPIO          |Ground         | 
|               |Reset - Ground | 

**Arduino IDE setup:**
* In File/Preferences/Additional Boards Manager URLs:, add the following URL, http://arduino.esp8266.com/stable/package_esp8266com_index.json
* In Tools/Boards click on Board Manager and download the Esp8266 library. I used version 2.5 because it was the only library that worked with my module
* Change the Baudrate of the board in the serial monitor and Board Manager. Mine worked ad 115200
* Select generic Esp8266 module and upload the code.
* Once the code is uploaded open the Serial Monitor and check what the board is outputting
* *When flashing the WiFi Module with new code ensure you unplug the VCC power cable and replug it back in. This Should reboot the WiFi Module into Flash mode while clearing its memory*

### Uploading Code to Arduino for TFT Display🖥
To upload code to the Arduino board ensure you have the following wirings connected:
| Esp8266        | Arduino      |
| ------------- |:-------------:| 
| VCC           | 3.3v          | 
| Ground        | Ground        |   
| Tx            |Tx             | 
| Rx            |Rx             | 
| CHPD(Enable)  |3.3v           | 
| GPIO          |Ground         | 
|               |Reset - Nothing| 

**Arduino IDE setup:**
* Ensure the LCD library is imported
* In Tools/Boards change the board to Arduino Uno 
* Upload the code
* Once the code is uploaded **Switch the Tx and Rx cables.** This will allow the Arduino board to receive and transmit data to the TFT display
* Once you are finished all the above steps **Switch the GPIO pin on the Esp8266 to 3.3v.** This ensures that it boots up into a none flash mode and reads the code in its flash memory.

## Notes 📝
* This project is still being completed...
* This display does not support the degrees symbol
* If you decide to change the BMP images familiarize yourself with the libraries documentaiton on Bitmaps because it is a little complicated.
* The sketch became too large for an Arduino Uno R3 (32K bytes) so I have upgraded to an Arduino Mega.
