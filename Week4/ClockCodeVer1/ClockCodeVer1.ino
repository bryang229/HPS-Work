#include "RotaryEncoder.h"
//LCD Library
#include <LiquidCrystal.h>

//                RS,E,D4-D7
LiquidCrystal lcd(10, 12, 4, 5, 6, 7);

//Libraries for the 8x8 Displays
#include <LedControl.h>
#include "Wire.h"
#include "RTClib.h"

//Pins for 8x8 Based off pin mapping
#define CLK_PIN 13
#define DATA_PIN 11
#define MAX_DEVICES 2
#define CS_PIN 9


RTC_DS1307 rtc;
LedControl myDisplay(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

//Rotary Enc Pins based off pin mapping
#define pinA 2
#define pinB 3
#define SwitchPin 8

RotaryEncoder *encoder = nullptr; //Creating a pointer for my encoder instance
int dir;                 //Creating my direction variable which will change with interrupts
//-1 -> Counter Clock Wise || 0 -> Not moving || 1 -> Clock Wise ^^^^^^^^^^^^^^^^^^^^^^^^^^

//Setting up Interrupt
void checkPosition() {
  encoder->tick(); // just call tick() to check the state.
  //  dir = (int)(encoder->getDirection()); //Updating the direction variable
}

//Display variables (8x8)
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


//Labels for the weeks (daysOfTheWeek has Saturday twice due to an error when generating the day of the week)
const char *daysOfTheWeek[] = {"Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char *months[] = {"N/A", "Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug",
                        "Sep", "Oct", "Nov", "Dec"
                       };
const char *dates[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"};

//Global D-M-Y Variables for the date
int month;
int year;
int day;
//Global Time variables
int seconds = 0;
int minute = 0;
int hour = 0;
// bool isBinaryMode = true; //Is used for changing modes (This feature has not Been added yet)

//Used for HMS formatting
boolean isFirstTime = false;

//This will be used to extract the formatted date
char *date[2];

int displayMode = 0;
//-1: Binary Mode ||0: Hours Minutes Seconds || 1: Decimal Mode
//Used for binary mode
int binaryImage[] = {0, 0, 0, 0, 0, 0};


void setup() {
  //Starting up 8x8
  myDisplay.shutdown(0, false);
  myDisplay.shutdown(1, false);
  myDisplay.setIntensity(0, 6);
  myDisplay.setIntensity(1, 6);
  myDisplay.clearDisplay(0);
  myDisplay.clearDisplay(1);
  //Setting up Rot Enc (LatchMode::FOUR3 is for when pinA and pinB are normally high)
  encoder = new RotaryEncoder(pinA, pinB, RotaryEncoder::LatchMode::FOUR3);
  //Attaching interrupts to pinA and pinB on falling edge
  attachInterrupt(digitalPinToInterrupt(pinA), checkPosition, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinB), checkPosition, FALLING);
  //begining lcd
  lcd.begin(16, 2);
  lcd.clear();
  //Setting up button on the Rot Enc
  pinMode(SwitchPin, INPUT_PULLUP);
  Serial.begin(9600);

  //Setting up RTC
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  //Setting up date for the first time and giving Date variables values
  DateTime now = rtc.now();
  year = (int)(now.year());
  day = (int)(now.day());
  month = (int)(now.month());

  //Showing date on LCD
  showDate();
}

int setBinaryFmt() {
  for (int i = 0; i < 6; i++) {
    binaryImage[i] = ((seconds >> i) & 1);
  }
}

void binaryMode() {
  setBinaryFmt();
  //right side first
  myDisplay.setRow(0, 0, off);
  myDisplay.setRow(0, 1, binaryImage[2] > 0 ? on : off);
  myDisplay.setRow(0, 2, off);
  myDisplay.setRow(0, 3, binaryImage[1] > 0 ? on : off);
  myDisplay.setRow(0, 4, binaryImage[1] > 0 ? on : off);
  myDisplay.setRow(0, 5, off);
  myDisplay.setRow(0, 6, binaryImage[0] > 0 ? on : off);
  myDisplay.setRow(0, 7, off);
  delay(10);

  //left side now
  myDisplay.setRow(1, 0, off);
  myDisplay.setRow(1, 1, binaryImage[5] > 0 ? on : off);
  myDisplay.setRow(1, 2, off);
  myDisplay.setRow(1, 3, binaryImage[4] > 0 ? on : off);
  myDisplay.setRow(1, 4, binaryImage[4] > 0 ? on : off);
  myDisplay.setRow(1, 5, off);
  myDisplay.setRow(1, 6, binaryImage[3] > 0 ? on : off);
  myDisplay.setRow(1, 7, off);

  delay(10);
}

int getNumber(int num, int row) {
  //  int tempNum = 0;
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

void decimalMode() {
  int right = seconds % 10;
  for (int j = 7; j > 0; j--)
    myDisplay.setRow(0, j, getNumber(right, j));
  delay(10);

  int left = floor(seconds / 10);
  for (int i = 7; i > 0; i--)
    myDisplay.setRow(1, i, getNumber(left, i));
  delay(10);

}

//Used for giving the M-S weekday
int getWeekIndex(int m, int d, int y) {
  Serial.print("Dates given: ");
  Serial.print(m);
  Serial.print("-");
  Serial.print(d);
  Serial.print("-");
  Serial.println(y);

  int monthKeys[] = {0, 1, 4, 4, 0, 2, 5, 0, 3, 6, 1, 4, 6};
  int index = String(y).substring(2).toInt();
  index = floor(index / 4);
  index += d;
  index += monthKeys[m];
  if ( m == 0 || ( m == 1 && y % 4 == 0 && (y % 100 != 0 || y % 400 == 0 && y % 100 == 0)))
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

//This gets the int values of M and D and gets the String version out their respective Arr
void formatDate() {
  date[0] = months[month];
  date[1] = dates[day];
}

//Shows current set date onto the LCD
void showDate() {
  formatDate(); //Formats date
  lcd.clear();
  for (int i = 0; i < 2; i++) {
    lcd.print(date[i]); //Prints all things from the date arr
    lcd.print(" ");
  }
  lcd.print(year); //prints year
  lcd.print(" ");
  lcd.setCursor(0, 1); //next line
  lcd.print(daysOfTheWeek[getWeekIndex(month, day, year)]); //prints the M-S date based of the current M-D-Y
}

//Shows date onto the LCD (Based off input)
void showDate(int m, int d, int y) {
  char *tempDates[] = {months[m], dates[d]};
  lcd.clear();
  for (int i = 0; i < 2; i++) {
    lcd.print(tempDates[i]);
    lcd.print(" ");
  }
  lcd.print(y);
  lcd.print(" ");
  lcd.setCursor(0, 1);
  lcd.print(daysOfTheWeek[getWeekIndex(m, d, y)]);

}

//Sets global date (Month, Day or Year)
//The following menus will have the same skeleton for Menu Scrolling and selection
void setDate() {
  boolean setDateMode = true; //For staying in this Menu
  char *datesMenu[] = {"Set Month", "Set Day", "Set Year", "Exit Menu"};
  int menuLen = 3; //For knowing the end of the list
  int menuPos = 0; //Users Position in the list

  lcd.clear();
  lcd.print(datesMenu[menuPos]); //Showing the users position in the menu on the LCD

  while (setDateMode) {
    int tempMenuPos = menuPos; //Knowing when the user moved
    dir = (int)(encoder->getDirection()); //Getting the direction
    if (dir != 0) { //Checking for movement
      if (dir < 0) {
        menuPos += menuPos > 0 ? dir : menuLen; //making sure it doesn't go below 0
      } else {
        menuPos += menuPos < menuLen ? dir : -1 * menuLen; //making sure it is at max the menuLen or back to 0
      }
      if (menuPos > menuLen || menuPos < 0)
        menuPos = 0; //Checking for any crazy errors and forcing it back to 0
      delay(25);
    }

    if (menuPos != tempMenuPos) { //only updating when user scrolls
      lcd.clear();
      lcd.print(datesMenu[menuPos]);
      tempMenuPos = menuPos;
      delay(50);
    }

    DateTime now = rtc.now();   //Checking for a change of time to display time on the 8x8
    int tempS = seconds;
    seconds = int(now.second());
    if (tempS != seconds)
      updateTime();

    //Checking for a button press on Rot Enc
    if (checkButton()) {
      switch (menuPos) { //Running different menus based off the user's position and deboucing
        //0 -> Month || 1 -> Day || 2 -> Year || 3 -> Exit (Settings Menu)
        case 0:
          setDateMode = false;
          delay(150);
          changeMonth(); break;
        case 1:
          setDateMode = false;
          delay(150);
          changeDate(); break;
        case 2:
          setDateMode = false;
          delay(150);
          changeYear(); break;
        case 3:
          setDateMode = false; displaySettings(); break;
        default:
          Serial.print("Error: ");
          Serial.println(menuPos); break;
      }
    }
  }
}

//Changing Global month menu
//Following change methods are very similar to this one
void changeMonth() {
  boolean settingMonth = true;
  int monthPos = month;           //user position
  showDate(monthPos, day, year);  //showing the date the user has selected on LCD
  int monthLen = 13;              //Max value Important since months[13] does not exist it is for handling an exit
  while (settingMonth) {
    dir = (int)(encoder->getDirection());  //Getting direction
    int tempMonthPos = monthPos;           //Used to make sure the user moved
    if (dir != 0) {                        //handling a Non-Zero direction
      if (dir < 0) {
        monthPos += monthPos > 1 ? dir : monthLen - 1;    //Making sure it's above 1
      } else {
        monthPos += monthPos < monthLen ? dir : -1 * (monthLen - 1); //making sure it's at max menuLen or back to 0
      }
      if (monthPos > monthLen || monthPos < 1) //Checking for any crazy errors and bringing it back to 1 if so
        monthPos = 1;
      delay(25);
    }
    if (monthPos != tempMonthPos) { //Checking for movement and displaying said movement
      Serial.print("Month Pos: ");
      Serial.print(monthPos);
      Serial.print("\t Temp Month Pos: ");
      Serial.println(tempMonthPos);
      if (monthPos == 13) { //Checking for 13 and showing the exit menu since we cannot call showDate with 13 as the month value
        lcd.clear();
        lcd.print("Exit Menu");
      } else if (monthPos < 13 && monthPos >= 0) {
        showDate(monthPos, day, year);
      }
      delay(10);
    }
    DateTime now = rtc.now();   //Updating 8x8 only when time changes
    int tempS = seconds;
    seconds = int(now.second());
    if (tempS != seconds)
      updateTime();

    if (checkButton()) { //Checking for button input
      if (monthPos == 13) {
        settingMonth = false;
        delay(150);
        setDate();
      }
      else {
        month = monthPos;
        delay(150);
        showDate();
      }
    }
  }
}

//Similar as the change month function
//Pretty much the above with different values of max position
void changeDate() {
  boolean settingDay = true;
  int dayPos = day;
  showDate(month, dayPos, year);
  int dayLen = 32;
  while (settingDay) {
    dir = (int)(encoder->getDirection());
    int tempDayPos = dayPos;
    if (dir != 0) {
      if (dir < 0) {
        dayPos += dayPos > 1 ? dir : dayLen - 1;
      } else {
        dayPos += dayPos < dayLen ? dir : -1 * (dayLen - 1);
      }
      if (dayPos > dayLen || dayPos < 1)
        dayPos = 1;
      delay(10);
    }
    if (dayPos != tempDayPos) {
      Serial.print("Day Pos: ");
      Serial.print(dayPos);
      Serial.print("\t Temp Day Pos: ");
      Serial.println(tempDayPos);
      if (dayPos == 32) {
        lcd.clear();
        lcd.print("Exit Menu");
      } else if (dayPos < 32 && dayPos >= 0) {
        showDate(month, dayPos, year);
      }
      delay(25);
    }
    DateTime now = rtc.now();

    int tempS = seconds;
    seconds = int(now.second());
    if (tempS != seconds)
      updateTime();

    if (checkButton()) {
      if (dayPos == 32) {
        settingDay = false;
        delay(150);
        setDate();
      }
      else {
        day = dayPos;
        delay(150);
        showDate();
      }
    }
  }
}

//Similar to the above change functions with a small change on the exit menu
void changeYear() {
  boolean settingYear = true;
  int yearPos = year;
  showDate(month, day, yearPos);
  while (settingYear) {
    dir = (int)(encoder->getDirection());
    int tempYearPos = yearPos;
    if (dir != 0) {
      yearPos += dir;
      delay(10);
    }
    if (yearPos != tempYearPos) {
      Serial.print("Year Pos: ");
      Serial.print(yearPos);
      Serial.print("\t Temp Year Pos: ");
      Serial.println(tempYearPos);
      //Mid year so it is the exit menu
      if (yearPos == 2016) {
        lcd.clear();
        lcd.print("Exit Menu");
      } else {
        showDate(month, day, yearPos);
      }
      delay(50);
    }
    DateTime now = rtc.now();

    int tempS = seconds;
    seconds = int(now.second());
    if (tempS != seconds)
      updateTime();

    if (checkButton()) {
      if (yearPos == 2016) { //Had to pick a year to be the exit screen and 2016 was trash so it got it
        settingYear = false;
        delay(150);
        setDate();
      }
      else {
        year = yearPos;
        delay(150);
        showDate();
      }
    }
  }
}

void disSettings() {
  char *menu[] = {"Hour Minutes Seconds Mode", "Decimal Mode", "Binary Mode", "Exit Menu"};
  int menuLen = 4;
  boolean disMode = true;
  int disPos = 0;
  //Lazy fix
  lcd.clear();
  lcd.print("Hour Minutes");
  lcd.setCursor(0, 1);
  lcd.print("Seconds Mode");
  while (disMode) {
    dir = (int)(encoder->getDirection());       //updating direction to see if Rot Enc has been turned
    int tempDisPos = disPos;          //This is for making sure the user moved in the menu
    if (dir != 0) {
      if (dir < 0) {
        disPos += disPos > 0 ? dir : menuLen;  //making sure it doesn't go below 0 and wrap around
      } else {
        disPos += disPos < menuLen ? dir : -1 * menuLen;  //making it wrap around if it goes over the menu len
      }
      if (disPos > menuLen || disPos < 0) //fixing any crazy errors
        disPos = 0;
      delay(25);
    }

    //Only updating LCD when movement is there
    if (disPos != tempDisPos) {
      if (disPos == 0) {
        //Lazy fix
        lcd.clear();
        lcd.print("Hour Minutes");
        lcd.setCursor(0, 1);
        lcd.print("Seconds Mode");
      }
      else {
        lcd.clear();
        lcd.print(menu[disPos]);
        tempDisPos = disPos;
      }
      delay(50);
    }

    //Updating 8x8 when time changes
    DateTime now = rtc.now();
    int tempS = seconds;
    seconds = int(now.second());
    if (tempS != seconds)
      updateTime();

    if (checkButton()) {
      //Handling button press using the user position to pick a function to be ran
      switch (disPos) {
        case 0:
          delay(150);
          isFirstTime = true;
          displayMode = 0; break;
        case 1:
          delay(150);
          displayMode = 1; break;
        case 2:
          delay(150);
          displayMode = -1; break;
        case 3:
          delay(150);
          disMode = false; break;
        default:
          Serial.print("Error: ");
          Serial.println(disPos); break;
      }
    }
  }
}



//Settings Menu will show on LCD and be controlled by the Rot Enc
void displaySettings() {
  char *settings[] = {"Set Date", "Change Display Mode", "Exit Menu"};
  int settingLen = 2;                             //Prone to change with more features coming
  boolean settingsMode = true;
  int settingsPos = 0;                            //User position
  lcd.clear();
  lcd.print(settings[settingsPos]);               //Displaying user's postion in the menu
  delay(100);
  while (settingsMode) {
    dir = (int)(encoder->getDirection());       //updating direction to see if Rot Enc has been turned
    int tempSettingsPos = settingsPos;          //This is for making sure the user moved in the menu
    if (dir != 0) {                             //checking for movement
      if (dir < 0) {
        settingsPos += settingsPos > 0 ? dir : settingLen;  //making sure it doesn't go below 0 and wrap around
      } else {
        settingsPos += settingsPos < settingLen ? dir : -1 * settingLen;  //making it wrap around if it goes over the menu len
      }
      if (settingsPos > settingLen || settingsPos < 0) //fixing any crazy errors
        settingsPos = 0;
      delay(25);
    }

    //Only updating LCD when movement is there
    if (settingsPos != tempSettingsPos) {
      if (settingsPos == 1) {
        //another lazy fix
        lcd.clear();
        lcd.print("Change Display");
        lcd.setCursor(0, 1);
        lcd.print("Mode");
      }
      else {
        lcd.clear();
        lcd.print(settings[settingsPos]);
        tempSettingsPos = settingsPos;
      }
      delay(50);
    }

    //Updating 8x8 when time changes
    DateTime now = rtc.now();
    int tempS = seconds;
    seconds = int(now.second());
    if (tempS != seconds)
      updateTime();

    if (checkButton()) {
      //Handling button press using the user position to pick a function to be ran
      switch (settingsPos) {
        case 0:
          delay(150);
          setDate();
          settingsMode = false; break;
        case 1:
          delay(150);
          disSettings();
          settingsMode = false; break;
        case 2:
          delay(150);
          showDate();
          settingsMode = false; break;
        default:
          Serial.print("Error: ");
          Serial.println(settingsPos); break;
      }
    }
  }
}

//Simple function that checks for a button press and returns a bool
//has a bit of deboucing built in but may need more when implemented
boolean checkButton() {
  int switchState = digitalRead(SwitchPin);
  if (switchState == LOW) {
    delay(30);
    if (switchState == digitalRead(SwitchPin)) {
      return true;
    }
  }
  return false;
}

//Function to show hour on the 8x8 Display
void showHour() {
  int binToImgH[] = {0x00, 0x6, 0x60, 0x66};
  //The different states of the hour portion of the clock 00 01 10 11 in Hex (keep in mind this is for a column of a 8x8 diplsay
  //Formatted 0XX00XX0 on the 8x8
  int index = 0;
  for (int i = 0; i < 2; i++) //This for loop gets the lower half of a four bit number (Hour)
    index += ((hour >> i) & 1) * pow(2, i);
  for (int i = 7; i > 4; i--) //This for loop shows it on the right 8x8
    myDisplay.setColumn(0, i, binToImgH[index]);

  index = 0;
  for (int i = 0; i < 2; i++)//This for loop gets the upper half of a four bit number (Hour) and shows it on the left side of the screen
    index += ((hour >> i + 2) & 1) * pow(2, i);
  for (int i = 7; i > 4; i-- )//This for loop shows it on the left 8x8
    myDisplay.setColumn(1, i, binToImgH[index]);
}

//Function to show minutes on the 8x8 Display
void showMinute() {
  int binToImg[] = {0x00, 0x2, 0x18, 0x1A, 0x40, 0x42, 0x58, 0x5A};
  //Different states 000  001  010  011    100   101   110   111
  //Formatted 0X0XX0X0 on 8x8
  int index = 0;
  for (int i = 0; i < 3; i++) //This for loop gets the lower half of a six bit number (Minute)
    index += ((minute >> i) & 1) * pow(2, i);
  for (int i = 3; i > 1; i--) //This for loop shows the number on the right 8x8
    myDisplay.setColumn(0, i, binToImg[index]);

  index = 0;

  for (int i = 0; i < 3; i++)//This for loop gets the upper half of a six bit number (Minute)
    index += ((minute >> i + 3) & 1) * pow(2, i);

  for (int i = 3; i > 1; i--)//This for loop shows the number on the left 8x8
    myDisplay.setColumn(1, i, binToImg[index]);
}

//Function to show seconds on 8x8 Display
void showSecond() {
  int binToImg[] = {0x00, 0x2, 0x18, 0x1A, 0x40, 0x42, 0x58, 0x5A};
  //Different states 000  001  010  011    100   101   110   111
  //Formatted 0X0XX0X0 on 8x8
  int index = 0;
  for (int i = 0; i < 3; i++) //This for loop gets the lower half of a six bit number (Seconds)
    index += ((seconds >> i) & 1) * pow(2, i);
  //Displaying the seconds (Only gets one Col/Row depending on how you loop at it)
  myDisplay.setColumn(0, 0, binToImg[index]);

  index = 0;
  for (int i = 0; i < 3; i++) //This for loop gets the upper half of the six bit number (Seconds)
    index += ((seconds >> i + 3) & 1) * pow(2, i);

  //Displaying it
  myDisplay.setColumn(1, 0, binToImg[index]);
}

//This function is used for updating time but only when the time changes
void updateTime() {
  if (displayMode < 0)
    binaryMode();
  else if (displayMode == 0) {
    int tempM = minute;   //Used for checking if minute changes
    int tempH = hour;     //Used for checking if hour changes
    DateTime now = rtc.now();     //Getting current time
    minute = int(now.minute());    //Getting current minute
    hour = now.hour() < 12 ? int(now.hour()) : int(now.hour()) - 12; //Getting current time in 12 hour format

    showSecond(); //Showing second since this is only called when seconds change
    if (tempM != minute || isFirstTime) //Only calling showMinute when the minutes change
      showMinute();
    if (tempH != hour || isFirstTime){  //Only calling showHour when the hour changes
      showHour();
      isFirstTime = false;
    }
  }
  else
    decimalMode();
}

void loop() {
  //Checking to see if the time changes to update seconds and other time
  DateTime now = rtc.now();
  int tempS = seconds;
  seconds = int(now.second());
  if (tempS != seconds)
    updateTime();

  //Checking for a button press to start menu with deboucing
  if (checkButton()) {
    delay(150);
    displaySettings();
  }
  else
    delay(50);
}
