// Rotary Encoder Inputs
#define CLK 10
#define DT 9
#define SW 8
#include <LiquidCrystal.h>
LiquidCrystal lcd(5, 6, 7, 12, 2, 4);
const char *daysOfTheWeek[] = {"N/A", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char *months[] = {"N/A", "Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug",
                        "Sep", "Oct", "Nov", "Dec"
                       };
int month = 1;
int day = 1;
int year = 2000;
int tempYear;


const char *dates[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"};
char *date[] = {months[month], dates[day], daysOfTheWeek[day]};


bool settingsMode = false;
int lastCounter;
int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir = "";
unsigned long lastButtonPress = 0;


void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  // Set encoder pins as inputs
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);

  // Setup Serial Monitor
  Serial.begin(9600);

  // Read the initial state of CLK
  lastStateCLK = digitalRead(CLK);
}

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
  Serial.print("Index: ");
  Serial.println(index);
  return index;
}

void showDate(bool useTempYear) {
  lcd.clear();
  String topLine = String(date[0]) + " " + String(useTempYear ? tempYear : year) + " " + String(date[1]);
  String bottomLine = String(date[2]);
  for (int i = 0; i < 2; i++) {
    lcd.print(date[i]);
    lcd.print(" ");
  }
  lcd.print(useTempYear ? tempYear : year);
  lcd.print(" ");
  lcd.setCursor(0, 1);
  lcd.print(date[2]);
  Serial.println(topLine);
  Serial.println(bottomLine);
}



void configureTime() {
  Serial.print("TODO");
}

void handleRotary() {
  lastCounter = counter;
  // Read the current state of CLK
  currentStateCLK = digitalRead(CLK);

  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1) {

    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(DT) != currentStateCLK) {
      counter --;
      currentDir = "CCW";
    } else {
      // Encoder is rotating CW so increment
      counter ++;
      currentDir = "CW";
    }


    Serial.print("Direction: ");
    Serial.print(currentDir);
    Serial.print(" | Counter: ");
    Serial.println(counter);
  }

  // Remember last CLK state
  lastStateCLK = currentStateCLK;
}

void formatDate(int m, int d, int y, bool updateYear) {
  date[0] = months[m];
  date[1] = dates[d];
  tempYear = y;
  year = updateYear ? y : year;
  int dayotw = getWeekIndex(m, d, y);
  date[2] = daysOfTheWeek[dayotw];
}

void formatDateM(int d, int y) {
  int dayotw = getWeekIndex(month, d, y);
  date[0] = char("<--");
  date[1] = dates[d];
  tempYear = y;
  date[2] = daysOfTheWeek[d];


}

void formateDateD(int m, int y) {
  int dayotw = getWeekIndex(m, day, y);
  date[0] = months[m];
  date[1] = char("<--");
  date[2] = daysOfTheWeek[dayotw];
  tempYear = year;
}

void formateDateY(int m, int d) {
  int dayotw = getWeekIndex(m, d, year);
  date[0] = months[m];
  date[1] = char("<--");
  date[1] = dates[d];
  date[2] = daysOfTheWeek[dayotw];
}



void setDate() {
  lcd.clear();
  int setYear = year;
  int setMonth = month;
  int setDay = day;
  int mode = 0;
  Serial.println("Setting date");
  while (true) {
    lcd.clear();
    int btnState = digitalRead(SW);
    handleRotary();
    showDate(false);
    if (lastCounter != counter) {
      if (currentDir == "CCW")
        mode = mode > 0 ? mode-- : 3;
      else
        mode = mode < 3  ? mode++ : 0;
    }

    else if (btnState == LOW) {
      if (millis() - lastButtonPress > 50) {
        switch (mode) {
          case 0:
            Serial.println("Month Mode");
            while (true) {
              lcd.clear();
              handleRotary();
              if (lastCounter != counter) {
                if (currentDir == "CCW")
                  setMonth = setMonth > 1 ? setMonth-- : 13;
                else
                  setMonth = setMonth < 13  ? setMonth++ : 1;
              }

              else if (btnState == LOW) {
                if (millis() - lastButtonPress > 50) {
                  if (setMonth == 13) {
                    break;
                  }
                  else
                    month = setMonth;
                }
                lastButtonPress = millis();
              }
              if (setMonth == 13)
                formatDateM(setDay, setYear);
              else
                formatDate(setMonth, setDay, setYear, false);
              showDate(true);
            }
              
        }
      }
      lastButtonPress = millis();
    }


  }
}

void handleSettings() {
  char *settingsMenu[] = {"Set date", "Exit Menu"};
  lcd.clear();
  lcd.print(settingsMenu[0]);
  int mode = 0;
  char prevState =  mode;
  while (settingsMode) {
    if (prevState != mode) {
      lcd.clear();
      lcd.print(settingsMenu[mode]);
    }
    int btnState = digitalRead(SW);
    handleRotary();
    if (lastCounter != counter) {
      if (currentDir == "CCW")
        mode = mode > 0 ? mode-- : sizeof(settingsMenu) - 1;
      else
        mode = mode < sizeof(settingsMenu) - 1  ? mode++ : 0;
      delay(100);
    }


    else if (btnState == LOW) {
      if (millis() - lastButtonPress > 50) {
        Serial.println(mode);
        Serial.println(currentDir);
        switch (mode) {
          case 0: setDate();
          default: Serial.println("Nah");
        }
      }
      lastButtonPress = millis();
    }
    prevState = mode;
  }
}


void loop() {

  showDate(false);
  lastStateCLK = currentStateCLK;

  // Read the button state
  int btnState = digitalRead(SW);
  //If we detect LOW signal, button is pressed
  if (btnState == LOW) {
    //if 50ms have passed since last LOW pulse, it means that the
    //button has been pressed, released and pressed again
    if (millis() - lastButtonPress > 50) {
      settingsMode = true;
      handleSettings();
      Serial.println("Button pressed!");
    }

    // Remember last button press event
    lastButtonPress = millis();
  }

  // Put in a slight delay to help debounce the reading
  delay(1);
  delay(200);
}
