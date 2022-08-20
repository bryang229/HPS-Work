const int A = 1;
const int B = 2;
const int C = 3;
const int D = 4;
const int E = 5;
const int F = 6;
const int G = 7;
const int DECIMAL = 8;


long interval = 3000;
unsigned long previousMillis = 0;

void clearLed(){
    Serial.println("clearing display");
    int pins[] = {A,B,C,D,E,F,G,DECIMAL};
    for(int i = 0; i < sizeof(pins); i++)
      digitalWrite(pins[i], LOW);  
}


void turnOnAllLED(){
    Serial.println("turning on all on Display");
    int pins[] = {A,B,C,D,E,F,G,DECIMAL};
    for(int i = 0; i < sizeof(pins); i++)
      digitalWrite(pins[i], HIGH);    
}


void showNine(){
   clearLed();
   Serial.println("cleared screen now showing 9");
   int on[] = {G,F,D,C,B,A};
   int off[] = {E, DECIMAL};

   for(int i = 0; i < sizeof(on); i++)
      digitalWrite(on[i], HIGH);
  
   for(int i = 0; i < sizeof(off); i++)
      digitalWrite(off[i], LOW);
      
   Serial.println("Showed nine now finishing method call");
}

void setup() {
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(DECIMAL, OUTPUT);
  
  Serial.begin(9600);  

  int pins[] = {A,B,C,D,E,F,G,DECIMAL};
    for(int i = 0; i < sizeof(pins); i++)
      digitalWrite(pins[i], LOW);  
}

void loop() {
  //This millis() can be used for the real life clock, just making sure no real time is lost is gonna be the hard part
  //  unsigned long currentMillis = millis();
//
//  if(currentMillis - previousMillis >= interval){
    Serial.println("showing 9");
    showNine();
    delay(1000);
//    previousMillis = currentMillis;
  //}
}
