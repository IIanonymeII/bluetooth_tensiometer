//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"
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
String test;
bool is_data = false;

bool display = false;

bool is_command = true;
String commandFound = "  ";
String command = "";

bool begin_tenso = false;

#define BACKGROUND_COLOR TFT_SILVER
#define TEXT_COLOR TFT_BLACK


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(BACKGROUND_COLOR);
  tft.setTextSize(5);
  tft.setTextColor(TEXT_COLOR);
}

void loop() {
  if (SerialBT.available()) 
  {
    char inChar= SerialBT.read();
    if(is_command)
    {
      if(inChar == '!'){
        is_command= false;
        is_data= true;
        for(int i= 0; i<(commandFound.length()-2); i++){
          command+=commandFound[i+2];
        }
        if(command == "Display"){display=true;initcommmande(true);}
        else if(command == "NotDisplay"){display=false;initcommmande(true);}
        else if(command == "Start"){begin_tenso=true;initcommmande(true);}
        else if(command == "Stop"){begin_tenso=false;initcommmande(true);}
        else{SerialBT.print("? >> Command not Found : ");SerialBT.print(command);SerialBT.print("\n");initcommmande(false);}
        
        }
      else{commandFound+=inChar;}
    }
  }
  if (begin_tenso){
    if (systolic==0){initDisplay();systolic++;}
    else{
      if (systolic==systolicLimit){
        if (diastolic==0){initDisplay();diastolic=systolicLimit;}
        else if (diastolic==diastolicLimit){diastolic=0;systolic=0;}
        else{diastolic--;}
        }
      else{systolic++;}
      }
    }
  else{systolic=0;}

  if(display){SerialBT.print("Systole : ");SerialBT.print(String(systolic));SerialBT.print(" - Diastol : ");SerialBT.print(String(diastolic));SerialBT.print("\n");}
  
  displayJustSystole();
  displayReadings();
  delay(100);
}


void initcommmande(const bool is_exist){
  if(is_exist){SerialBT.print(">> Command Found : ");SerialBT.print(command);SerialBT.print("\n");SerialBT.print("\n");}
  is_command=true;
  command = String();
  commandFound = String();
  is_data= false;
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
  tft.setTextColor(BACKGROUND_COLOR,TEXT_COLOR); tft.print (""); //Effacer des valeurs affichées à l'écran si modifications
  tft.setTextColor(TEXT_COLOR,BACKGROUND_COLOR);
  tft.printf("%03d", diastolic);
}