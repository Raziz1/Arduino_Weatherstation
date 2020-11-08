
//https://techtutorialsx.com/2018/03/17/esp32-arduino-getting-weather-data-from-api/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

//Network information__________________
const char* ssid = "pagaladhmi";
const char* password = "pagalinsaan";

//API information__________________________
const String endpoint = "http://api.openweathermap.org/data/2.5/weather?q=Ottawa,CA&units=metric&appid=";
const String key = "0498228c4cc70f8c9f9051d17dee5679";
const String endpoint_clock = "http://worldtimeapi.org/api/timezone/America/Toronto";
//const String endpoint_snow = "https://www.arduino.cc/reference/en/language/variables/data-types/string/functions/substring/";
String date_time;
int string_beginning;
int string_end;
String percentage;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); //Baud Rate of ESP8266
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to the WiFi network");


}

void loop() {

  // put your main code here, to run repeatedly:
  if ((WiFi.status() == WL_CONNECTED)) {//Check the current connection status

    HTTPClient http;


    //Clock API_____________________________________________________________________________________________________
    http.begin(endpoint_clock);
    int httpCode_clock = http.GET();
    if (httpCode_clock > 0) {
      String payload_clock = http.getString();
      const size_t capacity = JSON_OBJECT_SIZE(15) + 340;
      DynamicJsonBuffer jsonBuffer(capacity);

      //const char* json = "{\"abbreviation\":\"EST\",\"client_ip\":\"174.112.80.9\",\"datetime\":\"2020-11-06T14:29:04.314759-05:00\",\"day_of_week\":5,\"day_of_year\":311,\"dst\":false,\"dst_from\":null,\"dst_offset\":0,\"dst_until\":null,\"raw_offset\":-18000,\"timezone\":\"America/Toronto\",\"unixtime\":1604690944,\"utc_datetime\":\"2020-11-06T19:29:04.314759+00:00\",\"utc_offset\":\"-05:00\",\"week_number\":45}";

      JsonObject& root = jsonBuffer.parseObject(payload_clock);

      const char* abbreviation = root["abbreviation"]; // "EST"
      const char* client_ip = root["client_ip"]; // "174.112.80.9"
      const char* datetime = root["datetime"]; // "2020-11-06T14:29:04.314759-05:00"
      int day_of_week = root["day_of_week"]; // 5
      int day_of_year = root["day_of_year"]; // 311
      bool dst = root["dst"]; // false
      int dst_offset = root["dst_offset"]; // 0
      int raw_offset = root["raw_offset"]; // -18000
      const char* timezone = root["timezone"]; // "America/Toronto"
      long unixtime = root["unixtime"]; // 1604690944
      const char* utc_datetime = root["utc_datetime"]; // "2020-11-06T19:29:04.314759+00:00"
      const char* utc_offset = root["utc_offset"]; // "-05:00"
      int week_number = root["week_number"]; // 45
      date_time = datetime;
    } else {
      Serial.println("Error on HTTP request");
      http.end();
    }

    //Weather____________________________________________________________________________________________________________________________________________
    http.begin(endpoint + key); //Send request to url with API key
    int httpCode = http.GET(); //Make actual request
    if (httpCode > 0) { //Check for the returning code
      String payload = http.getString();
      //Serial.println(httpCode);
      //Serial.println(payload);


      //Parsing JSON____________________________________________________________________________________
      //Visit https://arduinojson.org/v5/assistant/ for a parsing assistant that breaks down the code_____________
      const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(13) + 420;
      DynamicJsonBuffer jsonBuffer(capacity);

      //const char* json = "{\"coord\":{\"lon\":-75.7,\"lat\":45.41},\"weather\":[{\"id\":804,\"main\":\"Clouds\",\"description\":\"overcast clouds\",\"icon\":\"04n\"}],\"base\":\"stations\",\"main\":{\"temp\":11.61,\"feels_like\":9.34,\"temp_min\":10.56,\"temp_max\":12.78,\"pressure\":1020,\"humidity\":71},\"visibility\":10000,\"wind\":{\"speed\":2.1,\"deg\":200},\"clouds\":{\"all\":90},\"dt\":1604637213,\"sys\":{\"type\":1,\"id\":872,\"country\":\"CA\",\"sunrise\":1604576902,\"sunset\":1604612667},\"timezone\":-18000,\"id\":6094817,\"name\":\"Ottawa\",\"cod\":200}";

      JsonObject& root = jsonBuffer.parseObject(payload);

      float coord_lon = root["coord"]["lon"]; // -75.7
      float coord_lat = root["coord"]["lat"]; // 45.41

      JsonObject& weather_0 = root["weather"][0];
      int weather_0_id = weather_0["id"]; // 804
      const char* weather_0_main = weather_0["main"]; // "Clouds"
      const char* weather_0_description = weather_0["description"]; // "overcast clouds"
      const char* weather_0_icon = weather_0["icon"]; // "04n"

      const char* base = root["base"]; // "stations"

      JsonObject& main = root["main"];
      float main_temp = main["temp"]; // 11.61
      float main_feels_like = main["feels_like"]; // 9.34
      float main_temp_min = main["temp_min"]; // 10.56
      float main_temp_max = main["temp_max"]; // 12.78
      int main_pressure = main["pressure"]; // 1020
      int main_humidity = main["humidity"]; // 71

      int visibility = root["visibility"]; // 10000

      float wind_speed = root["wind"]["speed"]; // 2.1
      int wind_deg = root["wind"]["deg"]; // 200

      int clouds_all = root["clouds"]["all"]; // 90

      long dt = root["dt"]; // 1604637213

      JsonObject& sys = root["sys"];
      int sys_type = sys["type"]; // 1
      int sys_id = sys["id"]; // 872
      const char* sys_country = sys["country"]; // "CA"
      long sys_sunrise = sys["sunrise"]; // 1604576902
      long sys_sunset = sys["sunset"]; // 1604612667

      int timezone = root["timezone"]; // -18000
      long id = root["id"]; // 6094817
      const char* name = root["name"]; // "Ottawa"
      int cod = root["cod"]; // 200

      /*
        //Snow Day Predictor_________________________________________________________________________________________________________________
        http.setTimeout(15000);
        http.begin(endpoint_snow); //Send request to url with API key
        int httpCode_snow = http.GET(); //Make actual request
        Serial.print(httpCode_snow);
        if (httpCode_snow > 0) { //Check for the returning code
        String payload_snow = http.getString();
        //Serial.println(payload_snow);
        //string_beginning = payload_snow.indexOf("theChance[");
        //percentage = payload_snow.substring(string_beginning, 16);
        //Serial.println(string_beginning);
        //Serial.println(percentage);
        Serial.print("GG");
      */



      Serial.print(String(date_time));
      Serial.print(String(","));
      Serial.print(String(weather_0_description));
      Serial.print(String(","));
      Serial.print(String(weather_0_icon));
      Serial.print(String(","));
      Serial.print(String((int)main_temp));
      Serial.print(String(","));
      Serial.print(String((int)main_temp_max));
      Serial.print(String(","));
      Serial.print(String((int)main_temp_min));
      Serial.print(String(","));
      Serial.print(String(wind_speed));

      digitalWrite(LED_BUILTIN, LOW); //Turn on LED on ESP8266 when Updating
      delay(2000);
      digitalWrite(LED_BUILTIN, HIGH); //Turn off LED on ESP8266
    }
    else {
      Serial.println("Error on HTTP request");
    }
    http.end(); //Free the resources
  }
  delay(15000);
}
