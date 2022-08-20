# include <IRremote.h>
const byte RECV_PIN = 11; // define input pin on Arduino
const char OUT[] = {39, 41, 43, 45, 47, 49, 51, 53};



void setup (){
  for(int i = 0; i < sizeof(OUT); i++)
    pinMode(OUT[i], OUTPUT);
  
  
  Serial.begin (9600);
  IrReceiver.begin(RECV_PIN, false); // Start the receiver
}
void displaySegment(int index, int val){
  if(index != 0){ 
    bool shouldTurnOn = val >> index & 00000001;
    digitalWrite(OUT[index], shouldTurnOn ? HIGH : LOW);
  }
  else{
    bool shouldTurnOn = val & 00000001;
    digitalWrite(OUT[index], shouldTurnOn ? HIGH : LOW);
  }
}

void responseToRemote(int command){
  int response = 0;
  switch(command){
    case 0x16: response = 0x3F; break;
    case 0x0C: response = 0x06; break;
    case 0x18: response = 0x5B; break;
    case 0x5E: response = 0x4F; break;
    case 0x08: response = 0x66; break;
    case 0x1C: response = 0x6D; break;
    case 0x5A: response = 0x7D; break;
    case 0x42: response = 0x07; break;
    case 0x52: response = 0x7F; break;
    case 0x4A: response = 0x67; break;
    default: response = 0x79; break;
  }
  Serial.println(response, HEX);
  for(int i = 0; i < 8; i++){
        displaySegment(i, response);
      }
}



void loop () {
  if (IrReceiver.decode()){ 
    Serial.println(IrReceiver.decodedIRData.command, HEX);
    responseToRemote(IrReceiver.decodedIRData.command);
    IrReceiver.resume (); // Receive the next value
  }
}
/*
#include <IRremote.h>

const byte IR_RECEIVE_PIN = 11;

void setup()
{
   Serial.begin(115200);
   Serial.println("IR Receive test");
   IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

void loop()
{
   if (IrReceiver.decode())
   {
      Serial.println(IrReceiver.decodedIRData.command, HEX);
      IrReceiver.resume();
   }
}
*/
