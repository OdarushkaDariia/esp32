#include <Arduino.h>
#include "LiquidCrystal_I2C.h" // LCD
#include "Wire.h"

#define BUZZER 33        // Buzzer

LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD display

void light(int LED_PIN)
{
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
}

void beepBuzzer(boolean isCorrect)
{
  if(isCorrect)
  {
    digitalWrite(BUZZER, HIGH);
    delay(300);
    digitalWrite(BUZZER, LOW);
  }
  else
  {
    digitalWrite(BUZZER, HIGH);
    delay(100);
    digitalWrite(BUZZER, LOW);
    delay(50);
    digitalWrite(BUZZER, HIGH);
    delay(100);
    digitalWrite(BUZZER, LOW);
    delay(50);
    digitalWrite(BUZZER, HIGH);
    delay(100);
    digitalWrite(BUZZER, LOW);  
  }
}

boolean read_delay(boolean last, int button)
{
    boolean current = digitalRead(button);
    if(last != current)
    {
        delay(50);
        current = digitalRead(button);
    }
    return current;
}

void lcd_init(){
    lcd.init();
    lcd.backlight();
}

void lcd_clear(){
    lcd.clear();
}

void lcd_set_cursor(int y, int x){
    lcd.setCursor(y, x);
}

void lcd_print(const char text[]){
    lcd.printstr(text);
}

void lcd_print_at(int y, int x, char* text){
    lcd.setCursor(y, x);
    lcd.print(text);
}
