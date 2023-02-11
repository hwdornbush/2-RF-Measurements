/* sketch_RFMeter_bargraph.ino
20 volt RMS RF Voltmeter using ESP32 and TFT LCD display
based on WB3JOB article in QST 2/2023
modified to work with TFT display using a sprite
and modified to show a bargraph
AA6BD
*/
#include <TFT_eSPI.h>                   // graphic library
#include <SPI.h>

#define LABEL_FGCOLOR   TFT_YELLOW                 // color of label text
#define LABEL_BGCOLOR     TFT_BLUE                 // color of label background
#define TITLE             "RF Voltmeter"

int analogPin = 35;              // Analog input pin
float voltInd = 0.0;             // display voltage
float voltPeak = 0.0;            // peak display voltage
int delayPeak = 0;               // delay time since last zero reading
int delayReset = 30;             // when to reset delay timer
float vIn = 0.0;                 // Variable when analog inut is read
float vScale = 0.1;              // scale factor for voltage in
float vLast = 0.0;               // last value of voltInd
float peakLast = 0.0;            // last value of voltPeak
int volts[10] = {0};             // last nine volt values for bargraph array

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library
TFT_eSprite sprite= TFT_eSprite(&tft);  // also add sprite functions

void setup() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);                       // fill screen with black
  tft.fillRoundRect(0,0,319,30,10,LABEL_BGCOLOR);  // title bar
  tft.drawRoundRect(0,0,319,239,10,TFT_WHITE);     // draw edge of screen
  tft.setTextColor(LABEL_FGCOLOR,LABEL_BGCOLOR);   // set label colors
  tft.drawCentreString(TITLE,160,2,4);             // show sketch title on screen

  sprite.createSprite(316,190);                    //size is limited by ESP32 memory
  pinMode(analogPin,INPUT);                        // Set pin as analog input
}

void loop() {
  sprite.fillSprite(TFT_BLACK);                 // fill sprite with black
  sprite.fillRect(28,0,2,155,TFT_WHITE);        // vertical line
  sprite.fillRect(28,155,270,2,TFT_WHITE);      // horizontal line
  sprite.setTextDatum(0);
  sprite.setTextColor(TFT_CYAN,TFT_BLACK);      // set to cyan
  sprite.setTextSize(1);
  for (int i=0; i<4; i++)
    sprite.drawString(String(i*50), 10, 150-(i*50));  // draw vertical values
  voltInd = analogRead(analogPin)*vScale;       // read digital representation of voltage
  if (voltPeak < voltInd) {                     // save if new peak
    voltPeak = voltInd;
  }
    volts[0] = voltInd;                         // save current value in array[0]
  for (int i=0; i<9; i++) {
    volts[9-i] = volts[8-i];                    // shift volts over one cell
    sprite.fillRect(30+30*(i), 155-volts[i], 20, volts[i], TFT_CYAN);  // show each value in array as bar
    sprite.drawString(String(volts[i]), 10+30*(i+1), 160, 2);  // show each value below bar
  }
  sprite.drawString("peak=", 40, 175, 2);
  sprite.fillRect(120,175,120,20,TFT_BLACK);    // clear voltPeak field
  sprite.drawString(String(voltPeak), 120, 175, 2);  // show peak value
  
  if (delayPeak < delayReset) {
    delayPeak++;                                 // increment how long peak has been displayed
    if (delayPeak >= delayReset) {               // if peak displayed long enough
      delayPeak = 0;                             // reset
      voltPeak = 0.0;
      peakLast = 0.0;
    }
    
  }
  sprite.pushSprite(1,35);                       // display sprite on screen
  delay(200);                                    // wait to get next value
}
