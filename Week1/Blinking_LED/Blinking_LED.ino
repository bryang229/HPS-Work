#define LED 11
#define BTN 10
int interval = 1000;

void setup() {
  pinMode(LED,OUTPUT);
  pinMode(BTN,INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
}

void blinkLight(){
  digitalWrite(LED,HIGH);
  delay(interval);
  digitalWrite(LED,LOW);
  delay(interval);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  if(digitalRead(BTN) == LOW){
    Serial.print("Pressed but possible bouncing");
    delay(3);
    if(digitalRead(BTN) == LOW){
      Serial.println("Confirmed button press");
      interval = interval == 1000 ? 2000 : 1000;
    }
  }
  blinkLight();
}
