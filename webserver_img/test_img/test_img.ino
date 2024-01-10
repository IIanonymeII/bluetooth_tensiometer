// ESP32_WROOM_TFT_eSPI_ILI9341_rainbow_scale
// note: rainbow scale original sketch by Bodmer 
//
// March 1, 2021
// Floris Wouterlood 
// public domain
//
// 240*320 TFT 7-pins SPI interface ILI 9341 controller
// microcontroller ESP32 WROOM32 DEV board
// NOTE the display is a 3.3V device. 
// pin wiring to WROOM32 as follows
// GND ----- GND
// VCC ----- 3V3
// SCL ----- D18 yellow wire
// SDA ----- D23 green wire
// RES ----- D4
// DC ------ D2
// CS ------ D15
// BLK ----- D32
//
// uses TFT _eSPI library with modified setup.h file (in the library folder)
// note - a backup of User_setup.h is named Setup_FW_WROOM32_ILI9341_TFT_018_OK.h
// and saved in the User_Setups folder
// note: colors are inverted
//
// in User_Setup uncomment in section 2:
   #define TFT_BL   32            // LED back-light control pin
   #define TFT_BACKLIGHT_ON HIGH  // Level to turn ON back-light (HIGH or LOW)



   #define TFT_RGB_ORDER TFT_BGR                      // color order Blue-Green-Red - for this specific display  
   #include <TFT_eSPI.h>                              // hardware-specific library
   #include <SPI.h>
   TFT_eSPI tft = TFT_eSPI();                         // invoke custom library

// some principal color definitions
// RGB 565 color picker at https://ee-programming-notepad.blogspot.com/2016/10/16-bit-color-generator-picker.html
   #define WHITE       0xFFFF
   #define BLACK       0x0000
   #define BLUE        0x001F
   #define RED         0xF800
   #define GREEN       0x07E0
   #define CYAN        0x07FF
   #define MAGENTA     0xF81F
   #define YELLOW      0xFFE0
   #define GREY        0x2108 
   #define SCALE0      0xC655                                                  // accent color for unused scale segments                                   
   #define SCALE1      0x5DEE                                                  // accent color for unused scale segments
   #define TEXT_COLOR  0xFFFF                                                  // is currently white 

// circular scale color scheme  
   #define RED2RED 0
   #define GREEN2GREEN 1
   #define BLUE2BLUE 2
   #define BLUE2RED 3
   #define GREEN2RED 4
   #define RED2GREEN 5

   uint32_t runTime = -99999;                                                  // time for next update
   int reading = 10;                                                            // value to be displayed in circular scale
   int reading1=20;                                                            // value to be displayed in circular scale 
   boolean alert = 0;
   int8_t ramp = 1;
   int tesmod = 0;
   int xpos= 0;
   float temp_00, temp_01,temp_02, temp_03, temp_04, temp_05,temp_06, temp_07, temp_08, temp_09, temp_10;
   float h,t;                                                                  // DHT11s
   int iteration =0;
   char* label[] = {"","Celsius","%","AMP", "VOLT"};            // some custom gauge labels


   int gaugeposition_x = 20;                                                   // these two variables govern the position
   int gaugeposition_y = 20;                                                   // of the square + gauge on the display



void setup() {
                                                      
   Serial.begin (9600);
   Serial.println ("starting TFT display");
                                                              
   tft.init();  
   tft.setRotation (2);                                                        // display in portrait
   tft.fillScreen (BLACK); 
   tft.drawRect (gaugeposition_x, gaugeposition_y, 200,100, YELLOW);
   tft.drawRect (gaugeposition_x, gaugeposition_y+150, 200,100, GREEN);

}

void loop() {

   iteration ++;
   Serial.print ("iteration ");
   Serial.println (iteration);
   if (iteration >99) {iteration = 0;}
         
   if (millis () - runTime >= 100)     // was 500  = delay                                       
     {                                    
      runTime = millis ();
      if( tesmod==0)
        {
         reading =  99;
        }
      if( tesmod==1)
        {
         reading = iteration; // important: here ring is seeded with value      
        } 
    int xpos = 20, ypos = 50, gap = 100, radius = 85;                           // position of upper ring and proportion
    ringMeter (reading,0,100, (gaugeposition_x+15),(gaugeposition_y+17),radius,label[0],RED2RED);            // draw analog meter
    tesmod = 1;
   }
 
}

// ################################################################################################################################
//  Draw the meter on the screen, returns x coord of righthand side
// ################################################################################################################################

int ringMeter(int value, int vmin, int vmax, int x, int y, int r, char *units, byte scheme) {
// Minimum value of r is about 52 before value text intrudes on ring
// drawing the text first is an option
 
   x += r; y += r;                                                             // calculate coordinates of center of ring
   int w = r / 3;                                                              // width of outer ring is 1/4 of radius
   int angle = 150;                                                            // half the sweep angle of the meter (300 degrees)
   int v = map(value, vmin, vmax, -angle, angle);                              // map the value to an angle v
   byte seg = 3;                                                               // segments are 3 degrees wide = 100 segments for 300 degrees
   byte inc = 6;                                                               // draw segments every 3 degrees, increase to 6 for segmented ring
   int colour = WHITE;                                                          // variable to save "value" text color from scheme and set default
 
 
   char buf [10];                                                              // convert value to a string
   byte len = 2; if (value > 999) len = 4;
   dtostrf (value, len, 0, buf);
   buf[len] = ' '; buf[len] = 0;                                               // add blanking space and terminator, helps to centre text too!
  
   tft.setTextSize (1);
   tft.setTextColor (colour,BLACK);
   tft.setCursor (x-25,y-10);tft.setTextSize (5);
   tft.print (buf);
  
   tft.setTextColor (WHITE,BLACK);
   tft.setCursor (x-39,y+75);tft.setTextSize (2);                              // units position relative to scale
   tft.print (units);                                                          // units display = celsius                                        
   return x + r;                                                               // calculate and return right hand side x coordinate
}


