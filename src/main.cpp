#include <Arduino.h>
#include <Adafruit_SSD1306.h> //OLed
#include <Wire.h> //OLed
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define cyclePin D6
#define selectPin D7

String SSID = "I.Carlsson_1";
String pass = "92ac437921";
String host = "http://13.53.168.240:1307";

WiFiClient wifi;
Adafruit_SSD1306 oled(128,32, &Wire,-1);


String selBathroom = "ninja_warrior";
String displayRoom = "Ninja Warrior";
unsigned long lastCleaned = 0;
uint32 timediff = 0;
unsigned long prevMillis = 0;

void connectWifi(){
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setCursor(0,0);
  
  oled.println("Ansluter till WiFi:");
  oled.println(SSID);
  oled.display();
  WiFi.begin(SSID,pass);
  delay(0);
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
    oled.print('.');
    oled.display();
    delay(500);
  }
  oled.display();
  
  // Serial.println("CONNECTED");
}



void sendStamp(){
    oled.clearDisplay();
    oled.setCursor(0,0);
    oled.setTextSize(2);
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String sendMsg = selBathroom;
      http.begin(wifi, host + "/InsertStamp");
      if(http.POST(sendMsg) == 200){
        http.end();   
        lastCleaned = 0;
        prevMillis = millis();
        oled.println("Rengjorde ");
        oled.print(displayRoom);
        oled.display();
        delay(5000);
      }
      else{
        lastCleaned = 0;
        prevMillis = millis();
        oled.print("Server fel\nRing Max");
        oled.display();
        delay(5000);
      }
  }
  else{
    lastCleaned = 0;
    prevMillis = millis();
    connectWifi();
    
    oled.clearDisplay();
    oled.setCursor(0,0);
    oled.print("Ansluten! Spara igen");
    oled.display();
    delay(5000);
    
  }
}
// void getUsers(){
  
//   if (WiFi.status() == WL_CONNECTED) {
//     HTTPClient http;
//     String user = "";
//     String userString = "";
//     http.begin(wifi, host + "/GetUsers");
//     if(http.GET() == 200){ 
//       userString += http.getString();
//     }
//     http.end();
    
//     if (userString.length() > 0){
//     for (int i = 0; i<=userString.length()-1; i++){
//       if(userString[i] != ','){
//         user+=userString[i];
//       }
      
//       else{
//       users[userCount] = user;
//       userCount+=1;
//       user = "";
//       }
//     }
//     }
//   }
// }
// void getBathrooms(){
//   if (WiFi.status() == WL_CONNECTED) {
//     HTTPClient http;
//     String wc = "";
//     String wcString = "";
//     http.begin(wifi, host + "/GetBathrooms");
//     if(http.GET() == 200){ 
//       wcString += http.getString();
//     }
//     http.end();
    
//     if (wcString.length() > 0){
//     for (int i = 0; i<=wcString.length()-1;i++){
//       if(wcString[i] != ','){
//         wc+=wcString[i];
//       }
      
//       else{
//         bathrooms[wcCount] = wc;
//         wcCount+=1;
//         wc = "";
//       }
//     }
//     }
//   }
// }
// void selectBathroom(){
//   oled.setTextSize(1);
//   if(wcCount > 0){
//     selBathroom = bathrooms[wcCycleCount%(wcCount)];
//     oled.print(selBathroom);
//     oled.display();
//   }
  
//   else{
//     oled.print("No bathrooms stored");
//     oled.display();
//     delay(2000);
//     return;
//   }
//   delay(500);
//   while(true){
//     yield();
    
//     if (digitalRead(cyclePin) == HIGH){
//       wcCycleCount+=1;
//       selBathroom = bathrooms[wcCycleCount%(wcCount)];
//       oled.clearDisplay();
//       oled.setCursor(0,0);
//       oled.println(selUser);
//       oled.print(selBathroom);
//       oled.display();
//       delay(300);
      
//     }
//     if (digitalRead(selectPin) == HIGH){
//       sendStamp();
//       oled.clearDisplay();
//       oled.display();
//       return;
//     }
// }
// }
// void selectUser(){
//   oled.clearDisplay();
//   oled.setTextSize(1);
//   oled.setCursor(0,0);
//   if(userCount > 0){
//     selUser = users[userCycleCount%(userCount)];
//     oled.print(selUser);
//     oled.display();
//   }
//   else{
//     oled.print("Inga användare sparade");
//     oled.display();
//     delay(1000);
//     return;
//   }
//   delay(500);
//   while(true){
//     yield();
    
//     if (digitalRead(cyclePin) == HIGH){
//       userCycleCount+=1;
//       selUser = users[userCycleCount%(userCount)];
//       oled.clearDisplay();
      
//       oled.setCursor(0,0);
//       oled.println(selUser);
//       oled.display();
//       delay(300);
      
//     }
//     if (digitalRead(selectPin) == HIGH){
//       // selectBathroom(); //does not need to be selected. Will be hardcoded 
//       sendStamp();
//       oled.clearDisplay();
//       oled.display();
//       return;
//     }
//   }
// }

void getLastCleaned(){
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(wifi, host + "/GetLatestStamp?bathroom=" + selBathroom);
    if (http.GET() == 200){
      lastCleaned = http.getString().toInt();
    }
    http.end();
  }
}


void setup() {
  // Serial.begin(9600);
  // Serial.println("SERIAL initialized");
  pinMode(selectPin,INPUT);
  // pinMode(cyclePin,INPUT);
  oled.begin(SSD1306_SWITCHCAPVCC,0x3C);
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(WHITE);
  oled.display();
  connectWifi();
  getLastCleaned();
  
  }


void loop() {
  oled.clearDisplay();
  oled.setCursor(0,0);
  oled.setTextSize(2);

  lastCleaned += millis() - prevMillis;
  prevMillis = millis();
  timediff = lastCleaned;
  


  unsigned long seconds = timediff / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  timediff %= 1000;
  seconds %= 60;
  minutes %= 60;
  hours %= 24;
  // oled.print("Sist rengjord:\n" + String(hours) + "h " + String(minutes) + "min " + String(seconds) + "s sedan");
  oled.print(String(hours) + "h " + String(minutes) + "min");
  oled.display();
  if(digitalRead(selectPin) == HIGH){
    // getUsers();
    // getBathrooms();
    // selectUser();
    sendStamp();
    
  }
  
  delay(100); 
}
