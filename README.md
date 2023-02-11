# RF-Measurements
RF Measurements: Field Strength Meter
This session explores equipment we can build to make RF measurements. It is based on the
article by Ken Pollock WB3JOB titled “RF Measurements Using Homemade Equipment” which
he sent to me via EMAIL.
There are three versions of the ESP32 sketches that work on the digital version of the 
RF Field Strength Meter that was built for this workshop.  All versions use an ESP32 and 
a 320x240 TFT LCD display, derived from W8BH NTP Clock.
1 - displays the RF volts from the detector and the peak reading recently.
2 - same display but uses a sprite to remove display flicker
3 - displays RF volts as a series of bars to show progression of the signal strength 
over time.  The bars progress across the display.
The full description of the approach and implementation is in the accompanying PDF file.
