boolean read_delay(boolean last, int button);
void light(int LED_PIN);
void beepBuzzer(boolean isCorrect);
void lcd_init();
void lcd_clear();
void lcd_set_cursor(int y, int x);
void lcd_print(const char text[]);
void lcd_print_at(int y, int x, char* text);
