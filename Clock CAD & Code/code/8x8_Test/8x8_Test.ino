//8x8 Testing Code

//Libraries for the 8x8 Displays
#include <LedControl.h>

//Pins for 8x8 Based off pin mapping
#define CLK_PIN 13
#define DATA_PIN 11
#define MAX_DEVICES 2
#define CS_PIN 9

LedControl myDisplay(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

int on = 0xff;
int off = 0x00;
int zero[] = {0x7c, 0xfe, 0xe3, 0xb3, 0x9a, 0xfe, 0x7c, 0x0};
int one[] = {0x80, 0x84, 0xff, 0xff, 0x80, 0x80, 0x0, 0x1};
int two[] = {0xc4, 0xe6, 0xb3, 0x93, 0xde, 0xcc, 0x1, 0x0};
int three[] = {0x44, 0xc6, 0x93, 0x93, 0xfe, 0x6c, 0x1, 0x1};
int four[] = {0x30, 0x38, 0x2d, 0xa7, 0xfe, 0xff, 0xa0, 0x0};
int five[] = {0x4e, 0xce, 0x8b, 0x8b, 0xfa, 0x73, 0x0, 0x1};
int six[] = {0x78, 0xfc, 0x97, 0x93, 0xf2, 0x61, 0x1, 0x0};
int seven[] = {0x6, 0x6, 0xe3, 0xf3, 0x1e, 0xf, 0x1, 0x1};
int eight[] = {0x6c, 0xfe, 0x93, 0x93, 0xff, 0x6c, 0x0, 0x0};
int nine[] = {0xc, 0x9e, 0x93, 0xd3, 0x7f, 0x3c, 0x0, 0x1};

int binaryImage[] = {0, 0, 0, 0, 0, 0};


void setup() {
  // put your setup code here, to run once:
  //Starting up 8x8
  myDisplay.shutdown(0, false);
  myDisplay.shutdown(1, false);
  myDisplay.setIntensity(0, 6);
  myDisplay.setIntensity(1, 6);
  myDisplay.clearDisplay(0);
  myDisplay.clearDisplay(1);
}

int getNumber(int num, int row) {
  switch (num) {
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

void checkScreen(int indexMax){
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8;j++){
      for(int k = 0; k <= indexMax; k++)
        myDisplay.setLed(k,i,j,true);
      delay(40);
      for(int k = 0; k <= indexMax; k++)
        myDisplay.clearDisplay(k);
      
    }
  }
}

void testDecimal(int indexMax) {
  for(int k = 0; k <= indexMax; k++)
    for(int i = 0; i < 10; i++){
      myDisplay.clearDisplay(k);
      int right = i % 10;
      for (int j = 7; j > 0; j--)
        myDisplay.setRow(k, j, getNumber(right, j));
      delay(50);
    }
}

int setBinaryFmt(int number) {
  for (int i = 0; i < 6; i++) {
    binaryImage[i] = ((number >> i) & 1);
  }
}

void testBinaryMode() {
  for(int i = 0; i < 64; i++){
    setBinaryFmt(i);
    //right side first
    myDisplay.setRow(0, 0, off);
    myDisplay.setRow(0, 1, binaryImage[2] > 0 ? on : off);
    myDisplay.setRow(0, 2, off);
    myDisplay.setRow(0, 3, binaryImage[1] > 0 ? on : off);
    myDisplay.setRow(0, 4, binaryImage[1] > 0 ? on : off);
    myDisplay.setRow(0, 5, off);
    myDisplay.setRow(0, 6, binaryImage[0] > 0 ? on : off);
    myDisplay.setRow(0, 7, off);
    delay(75);
  
    //left side now
    myDisplay.setRow(1, 0, off);
    myDisplay.setRow(1, 1, binaryImage[5] > 0 ? on : off);
    myDisplay.setRow(1, 2, off);
    myDisplay.setRow(1, 3, binaryImage[4] > 0 ? on : off);
    myDisplay.setRow(1, 4, binaryImage[4] > 0 ? on : off);
    myDisplay.setRow(1, 5, off);
    myDisplay.setRow(1, 6, binaryImage[3] > 0 ? on : off);
    myDisplay.setRow(1, 7, off);
  
    delay(75);
  }
}

//Function to show hour on the 8x8 Display
void showHour(int number) {
  int binToImgH[] = {0x00, 0x6, 0x60, 0x66};
  //The different states of the hour portion of the clock 00 01 10 11 in Hex (keep in mind this is for a column of a 8x8 diplsay
  //Formatted 0XX00XX0 on the 8x8
  float index = 0;
  for (int i = 0; i < 2; i++) //This for loop gets the lower half of a four bit number (Hour)
    index += ((number >> i) & 1) * pow(2, i);
  for (int i = 7; i > 4; i--) //This for loop shows it on the right 8x8
    myDisplay.setColumn(0, i, binToImgH[index <= 3.0 ? (int)index : (int)index + 1]);

  index = 0;
  for (int i = 0; i < 2; i++)//This for loop gets the upper half of a four bit number (Hour) and shows it on the left side of the screen
    index += ((number >> i + 2) & 1) * pow(2, i);
  for (int i = 7; i > 4; i-- )//This for loop shows it on the left 8x8
    myDisplay.setColumn(1, i, binToImgH[index <= 3.0 ? (int)index : (int)index + 1]);
}

//Function to show minutes on the 8x8 Display
void showMinute(int number) {
  int binToImg[] = {0x00, 0x2, 0x18, 0x1A, 0x40, 0x42, 0x58, 0x5A};
  //Different states 000  001  010  011    100   101   110   111
  //Formatted 0X0XX0X0 on 8x8
  float index = 0;
  for (int i = 0; i < 3; i++) //This for loop gets the lower half of a six bit number (Minute)
    index += ((number >> i) & 1) * pow(2, i);
  for (int i = 3; i > 1; i--) //This for loop shows the number on the right 8x8
    myDisplay.setColumn(0, i, binToImg[index <= 3.0 ? (int)index: (int)index + 1]);

  index = 0;

  for (int i = 0; i < 3; i++)//This for loop gets the upper half of a six bit number (Minute)
    index += ((number >> i + 3) & 1) * pow(2, i);

  for (int i = 3; i > 1; i--)//This for loop shows the number on the left 8x8
    myDisplay.setColumn(1, i, binToImg[index <= 3.0 ? (int)index: (int)index + 1]);
}

//Function to show seconds on 8x8 Display
void showSecond(int number) {
  int binToImg[] = {0x00, 0x2, 0x18, 0x1A, 0x40, 0x42, 0x58, 0x5A};
  char *testArr[] = {"000", "001", "010", "011", "100", "101", "110", "111"};
  //Different states 000  001  010  011    100   101   110   111
  //Formatted 0X0XX0X0 on 8x8
  float index = 0;
  for (int i = 0; i < 3; i++) //This for loop gets the lower half of a six bit number (Seconds)
      index += (number >> i & 1) * pow(2, i);
  //Displaying the seconds (Only gets one Col/Row depending on how you loop at it)
  myDisplay.setColumn(0, 0, binToImg[index <= 3.0 ? (int)index: (int)index + 1]);
 
  float index2 = 0;
  for (int i = 0; i < 3; i++) //This for loop gets the upper half of the six bit number (Seconds)
    index2 += (number >> i + 3 & 1) * pow(2, i);
  myDisplay.setColumn(1, 0, binToImg[index2 <= 3.0 ? (int)index2 : (int)index2 + 1]);
}

void testHMS(){
  myDisplay.clearDisplay(0);
  myDisplay.clearDisplay(1);
  for(int loopVar = 0; loopVar < 2; loopVar++){
    for(int h = 1; h < 13; h++){
      showHour(h);
      for(int m = 0; m < 60; m++){
        showMinute(m);
        for(int s = 0; s < 60; s++){
          showSecond(s);
          delay(5);
        }
        
        delay(10);
      }
      delay(15); 
    }
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  checkScreen(1);
  testDecimal(1);
  testBinaryMode();
  testHMS();
}
