#include <WiFiClientSecure.h>  // Internet connection, HTTP requests
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>       // work with JSON    
#include <HTTPClient.h>
// #include <Arduino_JSON.h>

#include "Pins.h"
WiFiClientSecure client;                    // WIFI connection

const char*  serverName = "https://citacka.azurewebsites.net";  // server name
String add_log = "/add_log.php?log=";                               // add a log
String check_pin = "/pin_check.php?pin=";                           // check a pin code 
String check_nfc = "/chip_exists.php?chip=";                        // check an NFC RFID

// const char* ssid     = "Redmi Note 11 Pro";      // WiFi hotspot name
// const char* password = "11111111";  // password
const char* ssid     = "TP-Link_871F";      // WiFi hotspot name
const char* password = "37909741";  // password


void connect_to_WIFI()
{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
        
    lcd_clear();
    lcd_print_at(6, 0, "WiFi: Redmi Note 11 Pro");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

String getPath_log(String log_message){
  return serverName + add_log + log_message;
}

String getPath_pin(String pin_code){
  return serverName + check_pin + pin_code;
}

String getPath_nfc(String card_ID){
  return serverName + check_nfc + card_ID;
}

bool request(String type, String text)
{
    if(WiFi.status()== WL_CONNECTED)
    {
        HTTPClient http;
        String path;
        if (type == "log")                              // choose the type of request
        {
            path = getPath_log(text);
            Serial.print("LOG: ");
            Serial.println(text);
        }
        if (type == "pin")
        {
            path = getPath_pin(text);
            Serial.print("PIN: ");
            Serial.println(text);
        }
        if (type == "nfc")
        {
            path = getPath_nfc(text);
            Serial.print("NFC: ");
            Serial.println(text);
        }
        http.begin(path.c_str());
        int httpResponseCode = http.GET();

        if (httpResponseCode > 0) {
          String payload = http.getString();
          Serial.println(payload);
          
          StaticJsonDocument<200> doc;                // JSON document
          DeserializationError error = deserializeJson(doc, payload);
          if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return false;
          }
          //Serial.println(doc["success"]);
          const char* response = doc["success"];
          http.end();
          //return response == "true";
          return payload == "{\"success\":true}";
        }
        else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
          http.end();
          return false;
        }
    }
}
