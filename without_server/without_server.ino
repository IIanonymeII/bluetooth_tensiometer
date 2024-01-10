#include <TFT_eSPI.h>
//#include "horlogeFont.h"
TFT_eSPI tft = TFT_eSPI();

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


void setup() {
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(BACKGROUND_COLOR);
  tft.setTextSize(5);
  tft.setTextColor(TEXT_COLOR);
}

void loop() {  
  initDisplay();
  delay_systolic = 50;
  delay_diastolic = 100;
  
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
}

void initDisplay(){
  tft.fillRect(0, 0, 320, 240, BACKGROUND_COLOR);  
}

void displayJustSystole(){
  tft.setCursor(20, 70);
  tft.setTextColor(BACKGROUND_COLOR,TEXT_COLOR); tft.print (""); //Effacer des valeurs affichées à l'écran si modifications
  tft.setTextColor(TEXT_COLOR,BACKGROUND_COLOR);
  tft.printf("%03d", systolic);
}

void displayReadings() {
  //tft.fillRect(0, 110, 200, 80, BACKGROUND_COLOR);
  tft.setCursor(20, 110);
  tft.printf("%03d", diastolic);
}
