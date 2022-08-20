#include <LiquidCrystal.h>
LiquidCrystal lcd(5,6,0,1,2,3);

const char *daysOfTheWeek[] = {"N/A","Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};

int getWeekIndex(int m, int d, int y) {
  int monthKeys[] = {0, 1, 4, 4, 0, 2, 5, 0, 3, 6, 1, 4, 6};
  int index = String(y).substring(2).toInt();
  index = floor(index / 4);
  index += d;
  index += monthKeys[m];
  if ( m == 0 ||( m == 1 && y % 4 == 0 && (y % 100 != 0 || y % 400 && y % 100 == 0)))
    index--;
  int topTwo = floor(y / 100);
  while (topTwo < 20)
    topTwo += 4;
  if ((topTwo - 17) % 4 == 0)
    index += 4;
  else if ((topTwo - 18) % 4 == 0)
    index += 2;
  else if ((topTwo - 19) % 4 == 0)
    index += 0;
  else if ((topTwo - 20) % 4 == 0)
    index += 6;
  index += String(y).substring(2).toInt();
  index %= 7;
  return index;
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.clear();
  Serial.println(daysOfTheWeek[getWeekIndex(1,1,2000)]);
}



void loop() {
 
}
