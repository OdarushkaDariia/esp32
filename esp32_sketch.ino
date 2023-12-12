#include "Pins.h"              // my lib for working with sensors and actuators of this project 
#include "HTTP.h"              // my lib for working with WiFi connection and HTTP requests
#include <SPI.h>               // NFC
#include <MFRC522.h>

#define SS_PIN 5               // NFC RFID
#define RST_PIN 0
#define BTN_1_PIN 27           // Buttons pins button2
#define BTN_2_PIN 26          //button1
#define BTN_3_PIN 25
#define BTN_4_PIN 12
#define BTN_ENTER_PIN 32
#define BUZZER 33              // Buzzer
#define LED_GREEN 14          // RGB LED
#define LED_RED 13

MFRC522 rfid(SS_PIN, RST_PIN); // NFC Reader 

boolean current_1 = LOW;
boolean last_1 = LOW;
boolean current_2 = LOW;
boolean last_2 = LOW;
boolean current_3 = LOW;
boolean last_3 = LOW;
boolean current_4 = LOW;
boolean last_4 = LOW;
boolean current_enter = LOW;
boolean last_enter = LOW;

void setup() {
    pinMode(BTN_1_PIN, INPUT);
    pinMode(BTN_2_PIN, INPUT);
    pinMode(BTN_3_PIN, INPUT);
    pinMode(BTN_4_PIN, INPUT);
    pinMode(BTN_ENTER_PIN, INPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    
    Serial.begin(115200);
    SPI.begin();
    rfid.PCD_Init();
    lcd_init();
    
    lcd_print("NFC tag reader");
    lcd_print_at(0, 1, "Connection...");
    light(LED_GREEN);
    delay(1000);
    
    //connect_to_WIFI();
}

void loop() {
    String code = enter_code();
    bool response = request("pin", code);
    
    char* message = "Login failed    ";
    if(response)
        message = "Login successful";
    lcd_print_at(0, 1, message);
    beepBuzzer(response);
    light(response ? LED_GREEN : LED_RED);
    request("log", "PIN%20" + code + (response ? "%20Login%20successful" : "%20Login%20failed"));
    if(digitalRead(BTN_ENTER_PIN) == HIGH) {
    lcd_clear();
    char* message = "Login failed    ";
        lcd_clear();
    free(message);
    delay(3000);
    lcd_clear();
 
  }
}

String enter_code()
{
  char guess[5];
  guess[0] = '0';
  guess[1] = '0';
  guess[2] = '0';
  guess[3] = '0';
  guess[4] = '\0';
  current_1 = LOW;
  last_1 = LOW;
  current_2 = LOW;
  last_2 = LOW;
  current_3 = LOW;
  last_3 = LOW;
  current_4 = LOW;
  last_4 = LOW;
  current_enter = LOW;
  last_enter = LOW;
  while(true)
  {
    String card_ID = readRFID();     // read NFC card
    if (card_ID != "")
    {
        bool response = request("nfc", card_ID); // make an NFC request
        
        char* message = "Login failed    ";
        if(response)
            message = "Login successful";
        lcd_print_at(0, 1, message);
        beepBuzzer(response);
        light(response ? LED_GREEN : LED_RED);
        request("log", "NFC%20" + card_ID + (response ? "%20Login%20successful" : "Login%20failed")); // make a log
    }
    current_1 = read_delay(last_1, BTN_1_PIN);              // read buttons
    current_2 = read_delay(last_2, BTN_2_PIN);
    current_3 = read_delay(last_3, BTN_3_PIN);
    current_4 = read_delay(last_4, BTN_4_PIN);
    current_enter = read_delay(last_enter, BTN_ENTER_PIN);
    if(current_1 && !last_1)                                // change pincode digits
        guess[0] = guess[0] == '9' ? '0' : guess[0] + 1;
    if(current_2 && !last_2)
        guess[1] = guess[1] == '9' ? '0' : guess[1] + 1;
    if(current_3 && !last_3)
        guess[2] = guess[2] == '9' ? '0' : guess[2] + 1;
    if(current_4 && !last_4)
        guess[3] = guess[3] == '9' ? '0' : guess[3] + 1;
    if(current_enter && !last_enter)                        // enter the pin code
        return guess;
    last_1 = current_1;
    last_2 = current_2;
    last_3 = current_3;
    last_4 = current_4;
    last_enter = current_enter;
    
    lcd_print_at(0, 0, guess);                              // display the code
  }
}

String readRFID() {                    //Read RFID card
    
    if (!rfid.PICC_IsNewCardPresent()) // Look for new card
        return "";
    if (!rfid.PICC_ReadCardSerial())   // Verify if the NUID has been readed
        return "";
    String card_ID = "";
    for (byte i = 0; i < rfid.uid.size; i++)
        card_ID += rfid.uid.uidByte[i];
    return card_ID;
    
}
