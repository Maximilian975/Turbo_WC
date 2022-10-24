#include <Arduino.h>

#define cyclePin D0
#define selectPin D1

void setup() {
  Serial.begin(9600);
  Serial.println("SERIAL initialized");
}

void loop() {
  
  Serial.print(digitalRead(selectPin));
  delay(500);

}