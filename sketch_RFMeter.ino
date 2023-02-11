/* 20 volt RMS RF Voltmeter using ESP32 and TFT LCD display
based on WB3JOB article in QST 2/2023
modified to work with TFT display
AA6BD
*/
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

#define LABEL_FGCOLOR   TFT_YELLOW                 // color of label text
#define LABEL_BGCOLOR     TFT_BLUE                 // color of label background
#define TITLE             "RF Voltmeter"

int analogPin = 35;              // Analog input pin
float voltInd = 0.0;             // display voltage
float voltPeak = 0.0;            // peak display voltage
int delayPeak = 0;               // delay time since last zero reading
int delayReset = 50;             // when to reset delay timer
float vIn = 0.0;                 // Variable when analog inut is read
float vScale = 0.1;              // scale factor for voltage in
float vLast = 0.0;               // last value of voltInd
float peakLast = 0.0;            // last value of voltPeak
TFT_eSPI tft = TFT_eSPI();       // Invoke custom library
const int f = 7;                 // screen font

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
  tft.setTextColor(TFT_CYAN,TFT_BLACK);      // set to cyan
  tft.setTextSize(2);
  pinMode(analogPin,INPUT);         // Set pin as analog input
}

void loop() {
  // put your main code here, to run repeatedly:
  voltInd = analogRead(analogPin)*vScale; // read digital representation of voltage
  tft.setTextSize(2);
  tft.drawString("RF Volts = ", 40, 60, 4);
  if (voltInd != vLast) {                  //  if voltInd changed
    vLast = voltInd;
    tft.fillRect(0,110,240,60,TFT_BLACK);  // clear field
  }
  tft.setTextSize(1);
  tft.drawFloat(voltInd, 2, 60, 110, 7);
  if (voltPeak < voltInd) {                  // save if new peak
    voltPeak = voltInd;
  }
  tft.setTextSize(2);
  tft.drawString("peak=", 40, 180, 4);
  if (voltPeak > peakLast) {                 // if voltPeak changed
    peakLast = voltPeak;
    tft.fillRect(180,180,240,60,TFT_BLACK);   // clear field
  }
  tft.setTextSize(1);
  tft.drawFloat(voltPeak, 0, 180, 180, 7);
  if (delayPeak < delayReset) {
    delayPeak++;                             // increment ow long peak has been displayed
    if (delayPeak >= delayReset) {           // if peak displayed long enough
      delayPeak = 0;                         // reset
      voltPeak = 0.0;
      peakLast = 0.0;
      tft.fillRect(180,180,240,60,TFT_BLACK);   // clear field
    }
  }
  delay(200);
}
