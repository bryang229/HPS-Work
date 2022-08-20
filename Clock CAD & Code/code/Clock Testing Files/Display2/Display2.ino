#include <LedControl.h>


#define CLK_PIN 13
#define DATA_PIN 11
#define MAX_DEVICES 2
#define CS_PIN 3
#define BTN 5

int on = 0xff;
int off = 0x00;
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
LedControl myDisplay(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

int binaryImage[] = {0,0,0,0,0,0};


void setup() {
  // Intialize the object
  pinMode(BTN, INPUT_PULLUP);
  Serial.begin(9600);
  myDisplay.shutdown(0, false);
  myDisplay.shutdown(1, false);
  myDisplay.setIntensity(0, 6);
  myDisplay.clearDisplay(0);
  myDisplay.clearDisplay(1);
}

int seconds = 0;
long currentMillis = millis();
long lastMillis = currentMillis;
bool isBinaryMode = true;

int setBinaryFmt(){
  for(int i = 0; i < 6; i++){
    binaryImage[i] = ((seconds >> i) & 1); 
  }
}

void binaryMode() {
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
//  Serial.println("TODO");
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

void decimalMode() {
  int right = seconds % 10;
  for(int j = 7; j > 0; j--)
    myDisplay.setRow(0,j,getNumber(right,j));
  delay(10);
  
  int left = floor(seconds/10);
  for(int i = 7; i > 0; i--)
    myDisplay.setRow(1,i,getNumber(left,i));
  delay(10);
  
}

void updateSeconds(){
  currentMillis = millis();
  if (currentMillis - lastMillis > 1000) {
    seconds += seconds < 59 ? 1 : -59;
    lastMillis = currentMillis;
  }  
}

void loop() {
  if (isBinaryMode)
    binaryMode();
  else
    decimalMode();
    
  if(digitalRead(BTN) == LOW){
    delay(1);
    if(digitalRead(BTN) == LOW){
      isBinaryMode = !isBinaryMode;
    }
  }
  updateSeconds();
  delay(100);
}
