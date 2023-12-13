#include <Arduino.h>
#include "LiquidCrystal_I2C.h" // LCD
#include "Wire.h"
#include "pitches.h"


#define BUZZER 33        // Buzzer

LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD display

int melody[] = {
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_D5, NOTE_G5
};

// note durations: 4 = quarter note, 8 = eighth note, etc, also called tempo:
int noteDurations[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
};

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
    // digitalWrite(BUZZER, HIGH);
    // delay(300);
    // digitalWrite(BUZZER, LOW);
    // iterate over the notes of the melody:
  int size = sizeof(noteDurations) / sizeof(int);

  for (int thisNote = 0; thisNote < size; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(BUZZER);
  }
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
