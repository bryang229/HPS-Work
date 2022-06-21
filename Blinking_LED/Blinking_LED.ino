#define LED 11
#define BTN 10
int interval = 1000;

void setup() {
  pinMode(LED,OUTPUT);
  pinMode(BTN,INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void blinkLight(){
  digitalWrite(LED,HIGH);
  delay(interval);
  digitalWrite(LED,LOW);
  delay(interval);
}

// the loop function runs over and over again forever
void loop() {
  if(digitalRead(BTN) == LOW){
    Serial.print("Pressed but possible bouncing");
    delay(3);
    if(digitalRead(BTN) == LOW){
      Serial.println("Confirmed button press");
      interval = interval == 1000 ? 500 : 1000;
    }
  }
  blinkLight();
}
