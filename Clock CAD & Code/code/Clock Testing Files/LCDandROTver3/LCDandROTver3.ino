#include "RotaryEncoder.h"
#include <LiquidCrystal.h>

//                RS,E,D4-D7
LiquidCrystal lcd(10, 12, 4, 5, 6, 7);

#define pinA 2
#define pinB 3
#define SwitchPin 8

RotaryEncoder *encoder = nullptr;
volatile int dir;

//Setting up Interrupt

void checkPosition() {
  encoder->tick(); // just call tick() to check the state.
  dir = (int)(encoder->getDirection());
}


//Labels for the weeks
const char *daysOfTheWeek[] = {"Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char *months[] = {"N/A", "Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug",
                        "Sep", "Oct", "Nov", "Dec"
                       };
const char *dates[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"};

int month = 7;
int year = 2022;
int day = 4;

//This will be used to extract the formatted date
char *date[] = {months[month], dates[day]};


void setup() {
  encoder = new RotaryEncoder(pinA, pinB, RotaryEncoder::LatchMode::FOUR3);
  attachInterrupt(digitalPinToInterrupt(pinA), checkPosition, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinB), checkPosition, CHANGE);
  lcd.begin(16, 2);
  lcd.clear();
  pinMode(SwitchPin, INPUT_PULLUP);
  Serial.begin(9600);
  showDate();
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

void formatDate(){
  date[0] = months[month];
  date[1] = dates[day];
}

void showDate() {
  formatDate();
  lcd.clear();
  for (int i = 0; i < 2; i++) {
    lcd.print(date[i]);
    lcd.print(" ");
  }
  lcd.print(year);
  lcd.print(" ");
  lcd.setCursor(0, 1);
  lcd.print(daysOfTheWeek[getWeekIndex(month, day, year)]);
}

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


void setDate() {
  boolean setDateMode = true;
  char *datesMenu[] = {"Set Month", "Set Day", "Set Year", "Exit Menu"};
  int menuLen = 3;
  int menuPos = 0;

  lcd.clear();
  lcd.print(datesMenu[menuPos]);

  while (setDateMode) {
    int tempMenuPos = menuPos;
    dir = (int)(encoder->getDirection());
    if (dir != 0) {
      if (dir < 0) {
        menuPos += menuPos > 0 ? dir : menuLen;
      } else {
        menuPos += menuPos < menuLen ? dir : -1 * menuLen;
      }
      if (menuPos > menuLen || menuPos < 0)
        menuPos = 0;
      delay(25);
    }

    if (menuPos != tempMenuPos) {
      lcd.clear();
      lcd.print(datesMenu[menuPos]);
      tempMenuPos = menuPos;
      delay(50);
    }

    if (checkButton()) {
      switch (menuPos) {
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

void changeMonth() {
  boolean settingMonth = true;
  int monthPos = month;
  showDate(monthPos, day, year);
  int monthLen = 13;
  while (settingMonth) {
    dir = (int)(encoder->getDirection());
    int tempMonthPos = monthPos;
    if (dir != 0) {
      if (dir < 0) {
        monthPos += monthPos > 1 ? dir : monthLen;
      } else {
        monthPos += monthPos < monthLen ? dir : -1 * (monthLen - 1);
      }
      if (monthPos > monthLen || monthPos < 1)
        monthPos = 1;
      delay(25);
    }
    if (monthPos != tempMonthPos) {
      Serial.print("Month Pos: ");
      Serial.print(monthPos);
      Serial.print("\t Temp Month Pos: ");
      Serial.println(tempMonthPos);
      if (monthPos == 13) {
        lcd.clear();
        lcd.print("Exit Menu");
      } else if (monthPos < 13 && monthPos >= 0) {
        showDate(monthPos, day, year);
      }
      delay(10);
    }
    //TODO's, add a 13th option so that the user can exit the menu however you have
    //to check for that.

    if (checkButton()) {
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
        dayPos += dayPos > 1 ? dir : dayLen;
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
    //TODO's, add a 13th option so that the user can exit the menu however you have
    //to check for that.

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
void changeYear() {
  boolean settingYear = true;
  int yearPos = year;
  showDate(month, day, yearPos);
  while (settingYear) {
    dir = (int)(encoder->getDirection());
    int tempYearPos = yearPos;
    if (dir != 0) {
      yearPos+= dir;
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
      } else{
        showDate(month, day, yearPos);
      }
      delay(50);
    }
    //TODO's, add a 13th option so that the user can exit the menu however you have
    //to check for that.

    if (checkButton()) {
      if (yearPos == 2016) {
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


void displaySettings() {
  char *settings[] = {"Set Date", "Exit Menu"};
  int settingLen = 1;
  boolean settingsMode = true;
  //  int lastDir = dir;
  int settingsPos = 0;
  lcd.clear();
  lcd.print(settings[settingsPos]);
  delay(100);
  while (settingsMode) {
    dir = (int)(encoder->getDirection());
    int tempSettingsPos = settingsPos;
    if (dir != 0) {
      if (dir < 0) {
        settingsPos += settingsPos > 0 ? dir : settingLen;
      } else {
        settingsPos += settingsPos < settingLen ? dir : -1 * settingLen;
      }
      if (settingsPos > settingLen || settingsPos < 0)
        settingsPos = 0;
      delay(25);
    }
    if (settingsPos != tempSettingsPos) {
      lcd.clear();
      lcd.print(settings[settingsPos]);
      tempSettingsPos = settingsPos;
      delay(50);
    }

    if (checkButton()) {
      switch (settingsPos) {
        case 0:
          delay(150);
          setDate();
          settingsMode = false; break;
        case 1:
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

void loop() {
  if (checkButton()) {
    delay(150);
    displaySettings();
  }
}
