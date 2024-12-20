This README File contains the project documentation from our 335H Colloqium Project. We are attempting to get the Nano 33 BLE Sense Microcontroller to work with the Adafruit TFT LCD 3.5" Breakout Board.

To wire the Arduino Nano 33 BLE Sense Microcontroller to the 8-bit mode, begin by connecting the Arduino's 3V to the power and GND to the ground. Connect the breakout board's GND to ground and Vin to  the power. If you power up the Arduino, the white backlight should turn off. If it does not, double check your wiring. 

* Connect the D0-D7 datalines to the Arduino D2-D9 pins, respectively.
* Connect the reset to the Arduino Reset
* Connect the third pin CS (Chip Select) to Analog 3
* Connect the fourth pin C/D (Command/Data) to Analog 2
* Connect the fifth pin WR (Write) to Analog 1
* Connect the sixth pin RD (Read) to Analog 0

To connect the touchscreen:
* Y+ to Arduino A2
* X+ to Arduino D8
* Y- to Arduino D7
* X- to Arduino A3

Troubleshooting:
* If the test example is not running on the Arduino, try opening the Serial monitor. Otherwise, if there is a random collection of pixels, try replacing the Arduino. 
