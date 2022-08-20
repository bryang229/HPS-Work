#include <RotaryEncoder.h>
#include <LiquidCrystal.h>
//                RS,E,D4,D5,D6,D7
LiquidCrystal lcd(10, 12, 4, 5, 6, 7);
// PinA, pinB, Latchmode
RotaryEncoder encoder(CLK,DT, RotaryEncoder::LatchMode::TW083);

//Setting up Interrupt
PCICR |= (1 << PCIE1);
PCMSK1 |= (1 <<PCINT10) | (1 << PCINT11);
ISR(PCINT1_vect){
  encoder.tick();
}


const char *daysOfTheWeek[] = {"N/A", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const char *months[] = {"N/A", "Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug",
                        "Sep", "Oct", "Nov", "Dec"};
int month = 1;
int day = 1;
int year = 2000;
int tempYear;


const char *dates[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"};
char *date[] = {months[month], dates[day], daysOfTheWeek[day]};

bool settingsMode = false;
volatile int counter = 0;
volatile int lastCounter = 0;


void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  Serial.begin(9600);
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



void setDate(){
  Serial.println("TODO");
}


void handleSettings() {
  char *settingsMenu[] = {"Set date", "Exit Menu"};
  lcd.clear();
  lcd.print(settingsMenu[0]);
  lastCounter = counter;
  int mode = 0;
  char prevState =  mode;
  while (settingsMode) {.
  
    if (prevState != mode) {
      lcd.clear();
      lcd.print(settingsMenu[mode]);
    }
    int btnState = digitalRead(switchPin);
    
    if (lastCounter != counter) {
      String currentDir = getDirection();
      if (currentDir == "CCW")
        mode = mode > 0 ? mode-- : sizeof(settingsMenu) - 1;
      else
        mode = mode < sizeof(settingsMenu) - 1  ? mode++ : 0;
      delay(100);
    }


    else if (btnState == LOW) {
      delay(10);
      Serial.println(counter);
      Serial.println(lastCounter);
      if(digitalRead(switchPin) == LOW){
        switch (mode) {
          case 0: setDate();
          case 1: settingsMode = false;
          default: Serial.println("Nah");
        }
      }
    }
    prevState = mode;
  }
}



//Show date onto the LCD
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
}


void loop() {
  showDate(false);
  
  switchState = digitalRead(switchPin);    // Read the digital value of the switch (LOW/HIGH)
  if (switchState == LOW) {
    delay(10);
    if(digitalRead(switchPin) == LOW){
      settingsMode = true;
      handleSettings();
      Serial.println("Button pressed!");
    }
  }
  
  delay(1);
  delay(200);
}
