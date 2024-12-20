# Setting Up the Arduino Nano 33 BLE Sense with the Adafruit 3.5" 320x480 Color TFT Touchscreen Breakout (HX8357D)

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

Modifications to Examples:
* Remove code which calls a function attempting to define an "identifier" (ex: "uint16_t identifier = tft.readID();" )
  This code still does not flow through the library for the Nano and returns 0. Instead set identifier to "0x8357" when working
  with the Adafruit TFT LCD 3.5" Breakout Board.
* In setup(), after calling Serial.begin(9600), make sure to wait for until it has began with a statement like: while(!Serial);

Wiring Photos: 

![alt text](https://github.com/jvelazquez4/335/blob/main/images/Wiring1.jpg)
![alt text](https://github.com/jvelazquez4/335/blob/main/images/Wiring2.jpg)
![alt text](https://github.com/jvelazquez4/335/blob/main/images/Wiring3.jpg)
![alt text](https://github.com/jvelazquez4/335/blob/main/images/Wiring4.jpg)
![alt text](https://github.com/jvelazquez4/335/blob/main/images/Wiring5.jpg)
![alt text](https://github.com/jvelazquez4/335/blob/main/images/20241209_104228.jpg)
![alt text](https://github.com/jvelazquez4/335/blob/main/images/20241209_104236.jpg)

