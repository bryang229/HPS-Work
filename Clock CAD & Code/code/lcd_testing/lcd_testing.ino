//LCD Library
#include <LiquidCrystal.h>

//                RS,E,D4-D7
LiquidCrystal lcd(10, 12, 4, 5, 6, 7);

void setup() {
 lcd.begin(16, 2);
  lcd.clear();
}

void testRow(int row, String val){
  if(row == 0)
    lcd.clear();
  for(int k = 0; k < 16; k++){
    lcd.setCursor(k,row);
    lcd.print(val);
    delay(10);
  }
}

void testLCD(){
  char *testChars[] = {"a", "b", "c", "d", "e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"};
  for(int i = 0; i < 26; i++){
    for(int j = 0; j < 2; j++){
      testRow(j,testChars[i]);
    }
    delay(100);
  }
  lcd.clear();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  testLCD();
}
