#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#define CLK_PIN 13
#define DATA_PIN 11
#define MAX_DEVICES 2
#define CS_PIN 3


#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

const char* hours[] = {"        ","      ||","    ||  ","    ||||","  ||    ","  ||  ||","  ||||  ","  ||||||","||      ","||    ||","||  ||  ","||  ||||","||||    "};
const char* minutes[] = {"......",".....|","....|.","....||","...|..","...|.|","...||.","...|||","..|...","..|..|","..|.|.","..|.||","..||..","..||.|","..|||.","..||||",".|....",".|...|",".|..|.",".|..||",".|.|..",".|.|.|",".|.||.",".|.|||",".||...",".||..|",".||.|.",".||.||",".|||..",".|||.|",".||||.",".|||||","|.....","|....|","|...|.","|...||","|..|..","|..|.|","|..||.","|..|||","|.|...","|.|..|","|.|.|.","|.|.||","|.||..","|.||.|","|.|||.","|.||||","||....","||...|","||..|.","||..||","||.|..","||.|.|","||.||.","||.|||","|||...","|||..|","|||.|.","|||.||"};

void setup() {
  // Intialize the object
  Serial.begin(9600);
  myDisplay.begin();
  myDisplay.displayScroll("b", PA_CENTER, PA_NO_EFFECT, 100);
  // Set the intensity (brightness) of the display (0-15)
  myDisplay.setIntensity(6);

  // Clear the display
  myDisplay.displayClear();
}

int hour = 1;
int minute = 0;
long currentMillis = millis();
long lastMillis = currentMillis;
void loop() {
  currentMillis = millis();
  myDisplay.setTextAlignment(PA_LEFT);
  myDisplay.print(minutes[minute]);
//  delay(10);
//  myDisplay.print((hour*100)+minute);
//  Serial.println(minutes[minute]);
  myDisplay.setTextAlignment(PA_RIGHT);
//  myDisplay.print(hours[hour]);
//  myDisplay.displayText("Hey",PA_LEFT,1,1,PA_NO_EFFECT);
  delay(1000);
  myDisplay.displayClear();
//  myDisplay.setTextAlignment(PA_LEFT);
//  myDisplay.print("Left");
//  delay(2000);
  if(currentMillis - lastMillis > 1000){
    minute += minute < 59 ? 1 : -59;
    lastMillis = currentMillis;
    if(minute == 0){
      hour += hour < 12 ? 1 : -12;
    }
    
  }
  
}
