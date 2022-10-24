#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define cyclePin D6
#define selectPin D5

Adafruit_SSD1306 oled(128,32, &Wire,-1);



void setup() {
  Serial.begin(9600);
  Serial.println("SERIAL initialized");
  pinMode(selectPin,INPUT);
  pinMode(cyclePin,INPUT);
  oled.begin(SSD1306_SWITCHCAPVCC,0x3C);
  oled.clearDisplay();
  oled.setTextSize(3);
  oled.setTextColor(WHITE);
  oled.print("Hej");
  oled.display();
  }


void loop() {
  
  Serial.print("select: ");
  Serial.println(digitalRead(selectPin));
  Serial.print("cycle: " );
  Serial.println(digitalRead(cyclePin));
  oled.setCursor(64,0);
  oled.clearDisplay();
  oled.print(digitalRead(selectPin));
  oled.display();
  delay(500); 

}

