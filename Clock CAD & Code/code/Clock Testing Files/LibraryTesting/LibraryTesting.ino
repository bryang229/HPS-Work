int switchPin = 10;                        // button pin
int switchState = HIGH;                    // button value

int pinA = 9;                              // Rotary encoder Pin A
int pinB = 3;                              // Rotary encoder Pin B
int pinAstateCurrent = LOW;                // Current state of Pin A
int pinAStateLast = pinAstateCurrent;      // Last read value of Pin A

void setup() {
  Serial.begin (9600);                     // Initialise the serial monitor

  pinMode (switchPin, INPUT_PULLUP);       // Enable the switchPin as input with a PULLUP resistor
    
  pinMode (pinA, INPUT);                   // Set PinA as input
  pinMode (pinB, INPUT);                   // Set PinB as input

  // Atach a CHANGE interrupt to PinB and exectute the update function when this change occurs.
  attachInterrupt(digitalPinToInterrupt(pinB), updateRot, CHANGE);
  
}

void loop() {

  // BUTTON
  switchState = digitalRead(switchPin);    // Read the digital value of the switch (LOW/HIGH)

  // If the switch is pressed (LOW), print message
  if (switchState == LOW) {
      Serial.println("Switch pressed");
  }
    
}

void updateRot() {

  /* WARNING: For this example I've used Serial.println within the interrupt callback. The Serial 
   * library already uses interrupts which could cause errors. Therefore do not use functions 
   * of the Serial libray in your interrupt callback.
   */

  // ROTATION DIRECTION
  pinAstateCurrent = digitalRead(pinA);    // Read the current state of Pin A
  
  // If there is a minimal movement of 1 step
  if ((pinAStateLast == LOW) && (pinAstateCurrent == HIGH)) {
    
    if (digitalRead(pinB) == HIGH) {      // If Pin B is HIGH
      Serial.println("Right");             // Print on screen
    } else {
      Serial.println("Left");            // Print on screen
    }
    
  }
  
  pinAStateLast = pinAstateCurrent;        // Store the latest read value in the currect state variable
  
}
