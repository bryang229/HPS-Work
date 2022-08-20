#include <RotaryDecoder.h>

RotaryDecoder decoder(10,9,8);

void setup() {
  Serial.begin(9600);
}

void loop() {
  decoder.handleRotary();
  decoder.handlePress();
  delay(100);
}
