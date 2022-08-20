#include <LedControl.h>
#include "Wire.h"
#include "RTClib.h"

#define CLK_PIN 13
#define DATA_PIN 11
#define MAX_DEVICES 2
#define CS_PIN 3

RTC_DS1307 rtc;
LedControl myDisplay(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

//binToImg and binToImgH hold each possible state that a 


void setup() {
  // Intialize the object
  Serial.begin(9600);
  myDisplay.shutdown(0, false);
  myDisplay.shutdown(1, false);
  myDisplay.setIntensity(0, 6);
  myDisplay.setIntensity(1, 6);
  myDisplay.clearDisplay(0);
  myDisplay.clearDisplay(1);
  Serial.println("Start up");
  Serial.println( 4 >> 2 & 1);
  Serial.println( 4 >> 1 & 1);
  Serial.println( 4 >> 0 & 1);
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
    }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  
}

int seconds = 0;
int minute = 0;
int hour = 0;
long currentMillis = millis();
long lastMillis = currentMillis;
bool isBinaryMode = true;

void showHour(){ 
  int binToImgH[] = {0x00, 0x6, 0x60, 0x66};

  int index = 0;
  for(int i = 0; i < 2; i++)
    index += ((hour >> i) & 1) * pow(2,i);
  for(int i = 7; i > 4; i--)
    myDisplay.setColumn(0,i,binToImgH[index]);

  index = 0;
  for(int i = 0; i < 2; i++)
    index += ((hour >> i+2) & 1) * pow(2,i);
  for(int i = 7; i > 4; i-- )
    myDisplay.setColumn(1,i,binToImgH[index]);
}

void showMinute(){ 
 int binToImg[] = {0x00,0x2, 0x18, 0x1A, 0x40,0x42,0x58,0x5A};

 int index = 0;
  for(int i = 0; i < 3; i++)
    index += ((minute >> i) & 1) * pow(2,i);
  for(int i = 3; i > 1; i--)
    myDisplay.setColumn(0,i,binToImg[index]);
  
  index = 0;

  for(int i = 0; i < 3; i++)
      index += ((minute >> i+3) & 1) * pow(2,i);
 
  for(int i = 3; i > 1; i--)
    myDisplay.setColumn(1,i,binToImg[index]);
}

void showSecond(){
  int binToImg[] = {0x00,0x2, 0x18, 0x1A, 0x40,0x42,0x58,0x5A};

  int index = 0;
  for(int i = 0; i < 3; i++)
    index += ((seconds >> i) & 1) * pow(2,i);
    
  myDisplay.setColumn(0,0,binToImg[index]);

  index = 0;
  for(int i = 0; i < 3; i++)
    index += ((seconds >> i+3) & 1) *pow(2,i);
  
  myDisplay.setColumn(1,0,binToImg[index]);
}


void updateTime() {
  int tempS = seconds;
  int tempM = minute;
  int tempH = hour;
  DateTime now = rtc.now();
  minute = int(now.minute());
  seconds = int(now.second());
  hour = now.hour() < 12 ? int(now.hour()) : int(now.hour())-12;

  if(tempS != seconds)
    showSecond();
  if(tempM != minute)
    showMinute();
  if(tempH != hour)
    showHour();
}

void loop() {
  updateTime();
  delay(50);
}
