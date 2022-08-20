#include <max7219.h>
#define LEFT 0
#define RIGHT 1

MAX7219 max7219;
int intToChar = 48;
int hour = 1;
int minutes = 0;

void setup() {
//  Serial.begin(9600);
  max7219.Begin();
}

void incTime(){
  minutes += minutes < 59 ? 1 : -59;
  if(minutes == 0)
    hour += hour < 12 ? 1 : -11;
}

void showTime(){
//  Serial.println("Showing time");
  max7219.Clear();
  int a = floor(hour / 10);
  a+= intToChar;
  max7219.DisplayChar(0,a,false);
  int b = hour % 10;
  b+= intToChar;
  max7219.DisplayChar(1,b,false);
  int c = floor(minutes / 10);
  c+= intToChar;  
  max7219.DisplayChar(2,c,false);  
  int d = minutes % 10;
  d+= intToChar;
  max7219.DisplayChar(3 ,d,false);
//  Serial.println("Time shown");
}

void loop() {
//  String temp;
//  char temp2[4];
//  temp = String(hour);
//  temp += String(minutes);
//  temp.toCharArray(temp2,temp.length());
//  max7219.DisplayText(temp2,RIGHT);
//  Serial.println(temp2);
//  incTime();
//  delay(100);
//  max7219.Clear();
  // put your main code here, to run repeatedly:
  showTime();
  incTime();
  delay(500);
}
