/*
  To upload through terminal you can use: curl -F "image=@firmware.bin" esp8266-webupdate.local/update
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

#include <TFT_eSPI.h>
//#include "horlogeFont.h"
TFT_eSPI tft = TFT_eSPI();


const char* host = "tensionmetre-snsm";
const char* ssid = "cestmoi";
const char* password = "123456789";

WebServer server(80);
const char* serverIndex = "<!DOCTYPE html><html><body>"
                          "<h1>Blood Pressure Monitor</h1>"
                          "<form action='/setLimits'>"
                          "Systolic Limit: <input type='number' name='systolicLimit'><br>"
                          "Diastolic Limit: <input type='number' name='diastolicLimit'><br>"
                          "<input type='submit' value='Set Limits'>"
                          "</form>"
                          "</body></html>";


int systolic = 0;
int diastolic = 0;
int systolicLimit = 200;
int diastolicLimit = 80;

int delay_systolic = 50;
int delay_systolic_limit = 300;

int delay_diastolic = 75;
int delay_diastolic_limit = 200;


#define BACKGROUND_COLOR TFT_SILVER
#define TEXT_COLOR TFT_BLACK

void setup(void) {
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(BACKGROUND_COLOR);
  tft.setTextSize(5);
  tft.setTextColor(TEXT_COLOR);
  
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);

  if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    MDNS.begin(host);

    server.on("/", HTTP_GET, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", serverIndex);
    });

    server.begin();
}}
void initDisplay(){
  tft.fillRect(0, 0, 320, 240, BACKGROUND_COLOR);  
}

void displayJustSystole(){
  tft.setCursor(20, 70);
  tft.setTextColor(BACKGROUND_COLOR,TEXT_COLOR); tft.print (""); //Effacer des valeurs affichées à l'écran si modifications
  tft.setTextColor(TEXT_COLOR,BACKGROUND_COLOR);
  tft.printf("%03d", systolic);
  //Serial.printf("dia : %d sys : %d",diastolic,systolic);
}

void displayReadings() {
  //tft.fillRect(0, 110, 200, 80, BACKGROUND_COLOR);
  tft.setCursor(20, 110);
  tft.printf("%03d", diastolic);
  //Serial.printf("dia : %d sys : %d",diastolic,systolic);
}


void loop(void) {
  initDisplay();
  delay_systolic = 50;
  delay_diastolic = 75;
  
  // Increment systolic reading until limit is reached
  for (int i = 0; i <= systolicLimit; i++) {
    systolic = i;
    displayJustSystole();
    delay_systolic = i*delay_systolic_limit/systolicLimit;
    delay(delay_systolic);
  }

  delay(500);

  // Decrement diastolic reading from systolic value to diastolic limit
  for (int i = systolic; i >= diastolicLimit; i--) {
      

    diastolic = i;
    displayReadings();
    delay_diastolic = i*delay_diastolic_limit/diastolicLimit;
    delay(delay_diastolic);
  }

  // Display final readings and pause for 10 seconds
  systolic = systolicLimit;
  diastolic = diastolicLimit;
  displayReadings();
  delay(10000);
  //test();
  //delay(2); // allow the CPU to switch to other tasks
}