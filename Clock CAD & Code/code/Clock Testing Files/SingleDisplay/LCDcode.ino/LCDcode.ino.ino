#include <LiquidCrystal.h>
LiquidCrystal lcd(5,6,0,1,2,3);


void setup() {
  lcd.begin(16,2);
  lcd.clear();
  lcd.print("Hello world");
}

void loop() {
//  lcd.print("Hello world");
//  delay(1000);
//  lcd.clear();
}
