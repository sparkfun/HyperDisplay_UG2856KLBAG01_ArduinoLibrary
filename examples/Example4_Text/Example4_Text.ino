/*
  Try out text on your Qwiic Transparent Grahical OLED.
  
  By: Owen Lyke
  SparkFun Electronics
  Date: November 27, 2019
  License: MIT. See license file for more information but you can
  basically do whatever you want with this code.
  Feel like supporting open source hardware?
  Buy a board from SparkFun! https://www.sparkfun.com/products/15173

  The graphics library is like a 3-layer cake. Here they are from top-down
  https://github.com/sparkfun/SparkFun_HyperDisplay
  https://github.com/sparkfun/HyperDisplay_SSD1309_ArduinoLibrary
  https://github.com/sparkfun/HyperDisplay_UG2856KLBAG01_ArduinoLibrary

  Hardware Compatibility
    - The IO pins on this board are designed for use with 3.3V so if you are using a 5V microcontroller
      please use a level shifter. Note: Qwiic connectors on SparkFun dev boards are already at 3.3V
    - This display relies on a copy of graphics data in your microcontroller, a total of 1024 bytes. 
      That is half the RAM available on an Uno so it is easy to run into sinister low-memory related
      bugs. We reccomend using a micro with more memory like a SAMD21, Esp32, Teensy, etc.
  
  Hardware Connections:
  Option 1 (I2C):
    Connect using a Qwiic jumper if you have a Qwiic compatible board and you plan to use I2C

  Option 2 (SPI):
    Connect SCLK and MOSI to the SPI port of your choice (13 and 11 for SPI on Uno-like boards) 
    Also connect D/C and CS to two unused GPIO pins of your choice (and set the proper pin definitions below)
    Don't forget power - connect 3.3V and GND
*/

#include "HyperDisplay_UG2856KLBAG01.h"   // Your library can be installed here: http://librarymanager/All#SparkFun_Transparent_Graphical_OLED
                                          // The rest of the Layer Cake:         http://librarymanager/All#SparkFun_HyperDisplay_SSD1309
                                          //                                     http://librarymanager/All#SparkFun_HyperDisplay
//////////////////////////
//      User Setup      //
//////////////////////////
#define SERIAL_PORT Serial  
#define WIRE_PORT Wire      // Used if USE_SPI == 0
#define SPI_PORT SPI        // Used if USE_SPI == 1

#define RES_PIN 2           // Optional
#define CS_PIN 4            // Used only if USE_SPI == 1
#define DC_PIN 5            // Used only if USE_SPI == 1

#define USE_SPI 0           // Choose your interface. 0 = I2C, 1 = SPI

// END USER SETUP


// Object Declaration. A class exists for each interface option
#if USE_SPI
  UG2856KLBAG01_SPI myTOLED;  // Declare a SPI-based Transparent OLED object called myTOLED
#else
  UG2856KLBAG01_I2C myTOLED;  // Declare a I2C-based Transparent OLED object called myTOLED
#endif /* USE_SPI */


void setup() {
  Serial.begin(9600);

  Serial.println(F("Example4_Text: Transparent Graphical OLED"));

#if USE_SPI 
  SPI_PORT.begin();
  myTOLED.begin(CS_PIN, DC_PIN, SPI_PORT);                  // Begin for SPI requires that you provide the CS and DC pin numbers
#else
  WIRE_PORT.begin();
  myTOLED.begin(WIRE_PORT, false, SSD1309_ARD_UNUSED_PIN);  // Begin for I2C has default values for every argument
  Wire.setClock(400000);
#endif /* USSE_SPI */
}

void loop() {
  static bool inverted = false;
  if(inverted){
    // Black Text Blue Background
    myTOLED.windowSet();
    myTOLED.setWindowColorClear();
  }else{
    // Blue Text Black Background
    myTOLED.windowClear();
    myTOLED.setWindowColorSet();
  }
  myTOLED.setTextCursor(0,0);
  myTOLED.print("Hello world");
  delay(3000);
  inverted = !inverted;
}
