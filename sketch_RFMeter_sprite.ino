/* sketch_RFMeter_sprite.ino
20 volt RMS RF Voltmeter using ESP32 and TFT LCD display
based on WB3JOB article in QST 2/2023
modified to work with TFT display
and modified to use sprite
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
int delayReset = 20;             // when to reset delay timer
float vIn = 0.0;                 // Variable when analog inut is read
float vScale = 0.1;              // scale factor for voltage in
float vLast = 0.0;               // last value of voltInd
float peakLast = 0.0;            // last value of voltPeak

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library
TFT_eSprite sprite= TFT_eSprite(&tft);  // also add sprite functions

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.fillRoundRect(0,0,319,30,10,LABEL_BGCOLOR);  // title bar
  tft.drawRoundRect(0,0,319,239,10,TFT_WHITE);     // draw edge screen
  tft.setTextColor(LABEL_FGCOLOR,LABEL_BGCOLOR);   // set label colors
  tft.drawCentreString(TITLE,160,2,4);             // show sketch title on screen

  sprite.createSprite(316,190);                    //size is limited by ESP32 memory
  pinMode(analogPin,INPUT);         // Set pin as analog input
}

void loop() {
  // put your main code here, to run repeatedly:
  sprite.setTextDatum(0);
  sprite.setTextColor(TFT_CYAN,TFT_BLACK);      // set to cyan
  sprite.setTextSize(2);
  voltInd = analogRead(analogPin)*vScale; // read digital representation of voltage
  sprite.drawString("RF 'Volts' = ", 40, 20, 4);
  if (voltInd != vLast) {                  //  if voltInd changed
    vLast = voltInd;
  }
  if (voltPeak > peakLast) {                 // if voltPeak changed
    peakLast = voltPeak;
  }
  sprite.setTextSize(1);  
  sprite.fillRect(0,70,316,60,TFT_BLACK);   // clear voltInd field
  sprite.drawString(String(voltInd), 60, 70, 7);
  if (voltPeak < voltInd) {                  // save if new peak
    voltPeak = voltInd;
  }
  sprite.setTextSize(2);
  sprite.drawString("peak=", 40, 130, 4);
  sprite.setTextSize(1);
  sprite.fillRect(180,120,180,80,TFT_BLACK);  // clear voltPeak field
  sprite.drawString(String(voltPeak), 180, 130, 7);
  
  if (delayPeak < delayReset) {
    delayPeak++;                             // increment how long peak has been displayed
    if (delayPeak >= delayReset) {           // if peak displayed long enough
      delayPeak = 0;                         // reset
      voltPeak = 0.0;
      peakLast = 0.0;
    }
    
  }
  sprite.pushSprite(1,35);
  delay(200);
}
