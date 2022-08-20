// TODO: Make this all controlled by SPI

#define MOSI 11
#define CLK_PIN 13
#define CS_MAX 4

#include <LedControl.h>
#include <max7219.h>

#include "Wire.h"
#include "RTClib.h"
#define LEFT 0
#define RIGHT 1
//#define DATA_PIN 11
#define MAX_DEVICES 2
#define CS_DOT 3
#define BTN 5
RTC_DS1307 rtc;
MAX7219 max7219;
LedControl myDisplay(MOSI, CLK_PIN, CS_DOT, MAX_DEVICES);

int intToChar = 48;
int hour = 0;
int minutes = 0;
int seconds = 0;
long currentMillis = millis();
long lastMillis = currentMillis;
bool isBinaryMode = false;

int on = 0xff;
int off = 0x00;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int zero[] = {0x7c,0xfe,0xe3,0xb3,0x9a,0xfe,0x7c,0x0};
int one[] = {0x80,0x84,0xff,0xff,0x80,0x80,0x0,0x1};
int two[] = {0xc4,0xe6,0xb3,0x93,0xde,0xcc,0x1,0x0};
int three[] = {0x44,0xc6,0x93,0x93,0xfe,0x6c,0x1,0x1};
int four[] = {0x30,0x38,0x2d,0xa7,0xfe,0xff,0xa0,0x0};
int five[] = {0x4e,0xce,0x8b,0x8b,0xfa,0x73,0x0,0x1};
int six[] = {0x78,0xfc,0x97,0x93,0xf2,0x61,0x1,0x0};
int seven[] = {0x6,0x6,0xe3,0xf3,0x1e,0xf,0x1,0x1};
int eight[] = {0x6c,0xfe,0x93,0x93,0xff,0x6c,0x0,0x0};
int nine[] = {0xc,0x9e,0x93,0xd3,0x7f,0x3c,0x0,0x1};


int binaryImage[] = {0,0,0,0,0,0};


void setup() {
  // Intialize the object
  pinMode(BTN, INPUT_PULLUP);
  myDisplay.shutdown(0, false);
  myDisplay.shutdown(1, false);
  myDisplay.setIntensity(0, 6);
  myDisplay.setIntensity(1, 6);
  myDisplay.clearDisplay(0);
  myDisplay.clearDisplay(1);
  max7219.Begin();

  if (! rtc.begin()) {
    pinMode(8,OUTPUT);
    digitalWrite(8,HIGH);
    while(1) delay(10);    
  }

  if(!rtc.isrunning())
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  updateTime();
  
}

int setBinaryFmt(){
  for(int i = 0; i < 6; i++){
    binaryImage[i] = ((seconds >> i) & 1); 
  }
}

//void incTime(){
//  minutes += minutes < 59 ? 1 : -59;
//  if(minutes == 0)
//    hour += hour < 12 ? 1 : -11;
//}

void updateTime() {
  DateTime now = rtc.now();
  minutes = int(now.minute());
  seconds = int(now.second());
  hour = now.hour() < 12 ? int(now.hour()) : int(now.hour())-12;
}


void showTime(){
  unsetCS(CS_DOT);
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
  delay(30);
}

void binaryMode() {
  unsetCS(CS_MAX);
  setBinaryFmt();
  //right side first
  myDisplay.setRow(0,0,off);
  myDisplay.setRow(0,1,binaryImage[2] > 0 ? on : off);
  myDisplay.setRow(0,2,off);
  myDisplay.setRow(0,3,binaryImage[1] > 0 ? on : off);
  myDisplay.setRow(0,4,binaryImage[1] > 0 ? on : off);
  myDisplay.setRow(0,5,off);
  myDisplay.setRow(0,6,binaryImage[0] > 0 ? on : off);
  myDisplay.setRow(0,7,off);
  delay(10);

  //left side now
  myDisplay.setRow(1,0,off);
  myDisplay.setRow(1,1,binaryImage[5] > 0 ? on : off);
  myDisplay.setRow(1,2,off);
  myDisplay.setRow(1,3,binaryImage[4] > 0 ? on : off);
  myDisplay.setRow(1,4,binaryImage[4] > 0 ? on : off);
  myDisplay.setRow(1,5,off);
  myDisplay.setRow(1,6,binaryImage[3] > 0 ? on : off);
  myDisplay.setRow(1,7,off);

  delay(10);
}

int getNumber(int num, int row){
//  int tempNum = 0;
  switch(num){
    case 0: return zero[row];
    case 1: return one[row];
    case 2: return two[row];
    case 3: return three[row];
    case 4: return four[row];
    case 5: return five[row];
    case 6: return six[row];
    case 7: return seven[row];
    case 8: return eight[row];
    case 9: return nine[row];
    default: return zero[0];
    }
}

void unsetCS(int cs){
  digitalWrite(cs,LOW);
}

void decimalMode() {
  unsetCS(CS_MAX);
  int right = seconds % 10;
  for(int j = 7; j > 0; j--)
    myDisplay.setRow(0,j,getNumber(right,j));
  delay(20);
  
  int left = floor(seconds/10);
  for(int i = 7; i > 0; i--)
    myDisplay.setRow(1,i,getNumber(left,i));
  delay(20);
  
}

void loop() {  
  showTime();
  
  if (isBinaryMode)
    binaryMode();
  else
    decimalMode();
  if(digitalRead(BTN) == LOW){
    delay(5);
    if(digitalRead(BTN) == LOW){
      isBinaryMode = !isBinaryMode;
    }
  }
  updateTime();
  delay(1);
}
