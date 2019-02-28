/*
  Take a guided tour of the basic capabilities of the Transparent Graphical OLED
  
  By: Owen Lyke
  SparkFun Electronics
  Date: February 28, 2019
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

  Serial.println("Example3_AdvancedFeatures: Transparent Graphical OLED");

#if USE_SPI 
  SPI_PORT.begin();
  myTOLED.begin(CS_PIN, DC_PIN, SPI_PORT);                  // Begin for SPI requires that you provide the CS and DC pin numbers
#else
  WIRE_PORT.begin();
  myTOLED.begin(WIRE_PORT, false, SSD1309_ARD_UNUSED_PIN);  // Begin for I2C has default values for every argument
  Wire.setClock(400000);
#endif /* USSE_SPI */


  wind_info_t defaultWindow, myWindow;  // Create some window objects
  
  // Initialize the windows to defualt settings 
  // (this is a pretty important step unless you are extra
  // careful to manually initialize each and every paramter)
  myTOLED.setWindowDefaults(&defaultWindow);
  myTOLED.setWindowDefaults(&myWindow);

  // We can set the rectangular active area of the window
  myWindow.xMin = 25; 
  myWindow.yMin = 05;
  myWindow.xMax = 73;
  myWindow.yMax = 48;

  // All drawing functions use window-relative coordinates
  // This makes it easy to reposition whole compositions

  // First let's draw a line on the default window (which is equivalent to the whole screen)
  myTOLED.lineSet(0, 0, 15, 43);

  // Now let's change the active window and draw the same line
  delay(1000);
  myTOLED.pCurrentWindow = &myWindow;
  myTOLED.lineSet(0, 0, 15, 43);

  delay(2000);

  // To better illustrate where the window is let's fill it in..
  myTOLED.windowSet();  // Calling windowSet without arguments operates on the current window.
  //  myTOLED.windowClear();

  delay(2000);

  // Remember, drawing functions are relative to the current window,
  // so you don't have to do extra math!
  myTOLED.circleClear(25, 20, 10); // Add some pizzazz!

  delay(2000);
  
  // You can also pass a window to windowSet/Clear to fill it without changing the current window
  myTOLED.windowClear(&defaultWindow); // The default initialization will make this clear the whole screen

}

void loop() {

#if !defined(__AVR_ATmega328P__) && !defined(__AVR_ATmega168__)
  // Now for an example of what you might do with a window...

  wind_info_t mariosWindow, pipeWindow;
  myTOLED.setWindowDefaults(&mariosWindow);
  myTOLED.setWindowDefaults(&pipeWindow);

  // Make the window just big enough for Mario
  mariosWindow.xMax = 21;
  mariosWindow.yMax = 28;

  pipeWindow.xMax = 24;
  pipeWindow.yMax = 24;

  // Move the windows into the starting position
  moveWindow(0, 34, &mariosWindow);
  moveWindow(103, 63-25, &pipeWindow);

  // Draw Pipe
  myTOLED.pCurrentWindow = &pipeWindow;
  showPipe();

  // Draw Mario at the starting position
  myTOLED.pCurrentWindow = &mariosWindow;
  showMario();


  // Move mario to his home
  myTOLED.windowClear();
  moveWindow(15, -15, &mariosWindow);
  showMario();

  myTOLED.windowClear();
  moveWindow(15, 15, &mariosWindow);
  showMario();

  myTOLED.windowClear();
  moveWindow(13, -15, &mariosWindow);
  showMario();

  myTOLED.windowClear();
  moveWindow(13, 15, &mariosWindow);
  showMario();

  myTOLED.windowClear();
  moveWindow(13, -15, &mariosWindow);
  showMario();

  myTOLED.windowClear();
  moveWindow(13, 15, &mariosWindow);
  showMario();

  myTOLED.windowClear();
  moveWindow(5, -23, &mariosWindow);
  showMario();
  
  myTOLED.windowClear();
  moveWindow(17, 0, &mariosWindow);
  showMario();
  
  delay(5000);

  myTOLED.windowClear();

#endif
}



void moveWindow( int8_t x, int8_t y, wind_info_t* pwind){
  pwind->xMin += x;
  pwind->yMin += y;
  pwind->xMax += x;
  pwind->yMax += y;
}


#if !defined(__AVR_ATmega328P__) && !defined(__AVR_ATmega168__)
void showMario( void ){
  myTOLED.pixelSet(6, 2); myTOLED.pixelSet(7, 2); myTOLED.pixelSet(8, 2); myTOLED.pixelSet(9, 2); myTOLED.pixelSet(10, 2); myTOLED.pixelSet(11, 2); myTOLED.pixelSet(12, 2); myTOLED.pixelSet(13, 2); 
  myTOLED.pixelSet(5, 3); myTOLED.pixelSet(6, 3); myTOLED.pixelSet(7, 3); myTOLED.pixelSet(8, 3); myTOLED.pixelSet(9, 3); myTOLED.pixelSet(10, 3); myTOLED.pixelSet(11, 3); myTOLED.pixelSet(12, 3); myTOLED.pixelSet(13, 3); myTOLED.pixelSet(14, 3); myTOLED.pixelSet(15, 3); myTOLED.pixelSet(16, 3); myTOLED.pixelSet(17, 3); myTOLED.pixelSet(18, 3); 
  myTOLED.pixelSet(4, 4); myTOLED.pixelSet(5, 4); myTOLED.pixelSet(6, 4); myTOLED.pixelSet(7, 4); myTOLED.pixelSet(8, 4); myTOLED.pixelSet(9, 4); myTOLED.pixelSet(10, 4); myTOLED.pixelSet(11, 4); myTOLED.pixelSet(12, 4); myTOLED.pixelSet(13, 4); myTOLED.pixelSet(14, 4); myTOLED.pixelSet(15, 4); myTOLED.pixelSet(16, 4); myTOLED.pixelSet(17, 4); myTOLED.pixelSet(18, 4); 
  myTOLED.pixelSet(4, 5); myTOLED.pixelSet(5, 5); myTOLED.pixelSet(6, 5); myTOLED.pixelSet(7, 5); myTOLED.pixelSet(8, 5); myTOLED.pixelSet(9, 5); myTOLED.pixelSet(10, 5); myTOLED.pixelSet(11, 5); myTOLED.pixelSet(12, 5); myTOLED.pixelSet(13, 5); myTOLED.pixelSet(14, 5); myTOLED.pixelSet(15, 5); 
  myTOLED.pixelSet(4, 6); myTOLED.pixelSet(5, 6); myTOLED.pixelSet(6, 6); myTOLED.pixelSet(7, 6); myTOLED.pixelSet(8, 6); myTOLED.pixelSet(9, 6); myTOLED.pixelSet(10, 6); myTOLED.pixelSet(11, 6); myTOLED.pixelSet(12, 6); myTOLED.pixelSet(13, 6); myTOLED.pixelSet(14, 6); myTOLED.pixelSet(15, 6); 
  myTOLED.pixelSet(3, 7); myTOLED.pixelSet(4, 7); myTOLED.pixelSet(5, 7); myTOLED.pixelSet(6, 7); myTOLED.pixelSet(7, 7); myTOLED.pixelSet(8, 7); myTOLED.pixelSet(9, 7); myTOLED.pixelSet(10, 7); myTOLED.pixelSet(11, 7); myTOLED.pixelSet(12, 7); myTOLED.pixelSet(13, 7); myTOLED.pixelSet(14, 7); myTOLED.pixelSet(15, 7); myTOLED.pixelSet(16, 7); myTOLED.pixelSet(17, 7); myTOLED.pixelSet(18, 7); 
  myTOLED.pixelSet(3, 8); myTOLED.pixelSet(4, 8); myTOLED.pixelSet(5, 8); myTOLED.pixelSet(6, 8); myTOLED.pixelSet(7, 8); myTOLED.pixelSet(8, 8); myTOLED.pixelSet(9, 8); myTOLED.pixelSet(10, 8); myTOLED.pixelSet(11, 8); myTOLED.pixelSet(12, 8); myTOLED.pixelSet(13, 8); myTOLED.pixelSet(14, 8); myTOLED.pixelSet(15, 8); myTOLED.pixelSet(16, 8); myTOLED.pixelSet(17, 8); myTOLED.pixelSet(18, 8); 
  myTOLED.pixelSet(3, 9); myTOLED.pixelSet(4, 9); myTOLED.pixelSet(5, 9); myTOLED.pixelSet(6, 9); myTOLED.pixelSet(7, 9); myTOLED.pixelSet(8, 9); myTOLED.pixelSet(9, 9); myTOLED.pixelSet(10, 9); myTOLED.pixelSet(11, 9); myTOLED.pixelSet(12, 9); myTOLED.pixelSet(13, 9); myTOLED.pixelSet(14, 9); myTOLED.pixelSet(15, 9); myTOLED.pixelSet(16, 9); myTOLED.pixelSet(17, 9); myTOLED.pixelSet(18, 9); myTOLED.pixelSet(19, 9); 
  myTOLED.pixelSet(3, 10); myTOLED.pixelSet(4, 10); myTOLED.pixelSet(5, 10); myTOLED.pixelSet(6, 10); myTOLED.pixelSet(7, 10); myTOLED.pixelSet(8, 10); myTOLED.pixelSet(9, 10); myTOLED.pixelSet(10, 10); myTOLED.pixelSet(11, 10); myTOLED.pixelSet(12, 10); myTOLED.pixelSet(13, 10); myTOLED.pixelSet(14, 10); myTOLED.pixelSet(15, 10); myTOLED.pixelSet(16, 10); myTOLED.pixelSet(17, 10); myTOLED.pixelSet(18, 10); 
  myTOLED.pixelSet(3, 11); myTOLED.pixelSet(4, 11); myTOLED.pixelSet(5, 11); myTOLED.pixelSet(6, 11); myTOLED.pixelSet(7, 11); myTOLED.pixelSet(8, 11); myTOLED.pixelSet(9, 11); myTOLED.pixelSet(10, 11); myTOLED.pixelSet(11, 11); myTOLED.pixelSet(12, 11); myTOLED.pixelSet(13, 11); myTOLED.pixelSet(14, 11); myTOLED.pixelSet(15, 11); myTOLED.pixelSet(16, 11); myTOLED.pixelSet(17, 11); myTOLED.pixelSet(18, 11); 
  myTOLED.pixelSet(6, 12); myTOLED.pixelSet(7, 12); myTOLED.pixelSet(8, 12); myTOLED.pixelSet(9, 12); myTOLED.pixelSet(10, 12); myTOLED.pixelSet(11, 12); myTOLED.pixelSet(12, 12); myTOLED.pixelSet(13, 12); myTOLED.pixelSet(14, 12); myTOLED.pixelSet(15, 12); myTOLED.pixelSet(16, 12); 
  myTOLED.pixelSet(5, 13); myTOLED.pixelSet(6, 13); myTOLED.pixelSet(7, 13); myTOLED.pixelSet(8, 13); myTOLED.pixelSet(9, 13); myTOLED.pixelSet(10, 13); myTOLED.pixelSet(11, 13); myTOLED.pixelSet(12, 13); myTOLED.pixelSet(13, 13); 
  myTOLED.pixelSet(4, 14); myTOLED.pixelSet(5, 14); myTOLED.pixelSet(6, 14); myTOLED.pixelSet(7, 14); myTOLED.pixelSet(8, 14); myTOLED.pixelSet(9, 14); myTOLED.pixelSet(10, 14); myTOLED.pixelSet(11, 14); myTOLED.pixelSet(12, 14); myTOLED.pixelSet(13, 14); 
  myTOLED.pixelSet(3, 15); myTOLED.pixelSet(4, 15); myTOLED.pixelSet(5, 15); myTOLED.pixelSet(6, 15); myTOLED.pixelSet(7, 15); myTOLED.pixelSet(8, 15); myTOLED.pixelSet(9, 15); myTOLED.pixelSet(10, 15); myTOLED.pixelSet(11, 15); myTOLED.pixelSet(12, 15); myTOLED.pixelSet(13, 15); myTOLED.pixelSet(14, 15); myTOLED.pixelSet(15, 15); myTOLED.pixelSet(16, 15); myTOLED.pixelSet(17, 15); myTOLED.pixelSet(18, 15); 
  myTOLED.pixelSet(2, 16); myTOLED.pixelSet(3, 16); myTOLED.pixelSet(4, 16); myTOLED.pixelSet(5, 16); myTOLED.pixelSet(6, 16); myTOLED.pixelSet(7, 16); myTOLED.pixelSet(8, 16); myTOLED.pixelSet(9, 16); myTOLED.pixelSet(10, 16); myTOLED.pixelSet(11, 16); myTOLED.pixelSet(12, 16); myTOLED.pixelSet(13, 16); myTOLED.pixelSet(14, 16); myTOLED.pixelSet(15, 16); myTOLED.pixelSet(16, 16); myTOLED.pixelSet(17, 16); myTOLED.pixelSet(18, 16); 
  myTOLED.pixelSet(1, 17); myTOLED.pixelSet(2, 17); myTOLED.pixelSet(3, 17); myTOLED.pixelSet(4, 17); myTOLED.pixelSet(5, 17); myTOLED.pixelSet(6, 17); myTOLED.pixelSet(7, 17); myTOLED.pixelSet(8, 17); myTOLED.pixelSet(9, 17); myTOLED.pixelSet(10, 17); myTOLED.pixelSet(11, 17); myTOLED.pixelSet(12, 17); myTOLED.pixelSet(13, 17); myTOLED.pixelSet(14, 17); myTOLED.pixelSet(15, 17); myTOLED.pixelSet(16, 17); myTOLED.pixelSet(17, 17); myTOLED.pixelSet(18, 17); myTOLED.pixelSet(19, 17); 
  myTOLED.pixelSet(1, 18); myTOLED.pixelSet(2, 18); myTOLED.pixelSet(3, 18); myTOLED.pixelSet(4, 18); myTOLED.pixelSet(5, 18); myTOLED.pixelSet(6, 18); myTOLED.pixelSet(7, 18); myTOLED.pixelSet(8, 18); myTOLED.pixelSet(9, 18); myTOLED.pixelSet(10, 18); myTOLED.pixelSet(11, 18); myTOLED.pixelSet(12, 18); myTOLED.pixelSet(13, 18); myTOLED.pixelSet(14, 18); myTOLED.pixelSet(15, 18); myTOLED.pixelSet(16, 18); myTOLED.pixelSet(17, 18); myTOLED.pixelSet(18, 18); myTOLED.pixelSet(19, 18); 
  myTOLED.pixelSet(1, 19); myTOLED.pixelSet(2, 19); myTOLED.pixelSet(3, 19); myTOLED.pixelSet(4, 19); myTOLED.pixelSet(5, 19); myTOLED.pixelSet(6, 19); myTOLED.pixelSet(7, 19); myTOLED.pixelSet(8, 19); myTOLED.pixelSet(9, 19); myTOLED.pixelSet(10, 19); myTOLED.pixelSet(11, 19); myTOLED.pixelSet(12, 19); myTOLED.pixelSet(13, 19); myTOLED.pixelSet(14, 19); myTOLED.pixelSet(15, 19); myTOLED.pixelSet(16, 19); myTOLED.pixelSet(17, 19); myTOLED.pixelSet(18, 19); myTOLED.pixelSet(19, 19); 
  myTOLED.pixelSet(1, 20); myTOLED.pixelSet(2, 20); myTOLED.pixelSet(3, 20); myTOLED.pixelSet(4, 20); myTOLED.pixelSet(5, 20); myTOLED.pixelSet(6, 20); myTOLED.pixelSet(7, 20); myTOLED.pixelSet(8, 20); myTOLED.pixelSet(9, 20); myTOLED.pixelSet(10, 20); myTOLED.pixelSet(11, 20); myTOLED.pixelSet(12, 20); myTOLED.pixelSet(13, 20); myTOLED.pixelSet(14, 20); myTOLED.pixelSet(15, 20); myTOLED.pixelSet(16, 20); myTOLED.pixelSet(17, 20); myTOLED.pixelSet(18, 20); myTOLED.pixelSet(19, 20); 
  myTOLED.pixelSet(1, 21); myTOLED.pixelSet(2, 21); myTOLED.pixelSet(3, 21); myTOLED.pixelSet(4, 21); myTOLED.pixelSet(5, 21); myTOLED.pixelSet(6, 21); myTOLED.pixelSet(7, 21); myTOLED.pixelSet(8, 21); myTOLED.pixelSet(9, 21); myTOLED.pixelSet(10, 21); myTOLED.pixelSet(11, 21); myTOLED.pixelSet(12, 21); myTOLED.pixelSet(13, 21); myTOLED.pixelSet(14, 21); myTOLED.pixelSet(15, 21); myTOLED.pixelSet(16, 21); myTOLED.pixelSet(17, 21); myTOLED.pixelSet(18, 21); myTOLED.pixelSet(19, 21); 
  myTOLED.pixelSet(1, 22); myTOLED.pixelSet(2, 22); myTOLED.pixelSet(3, 22); myTOLED.pixelSet(4, 22); myTOLED.pixelSet(5, 22); myTOLED.pixelSet(6, 22); myTOLED.pixelSet(7, 22); myTOLED.pixelSet(8, 22); myTOLED.pixelSet(9, 22); myTOLED.pixelSet(10, 22); myTOLED.pixelSet(11, 22); myTOLED.pixelSet(12, 22); myTOLED.pixelSet(13, 22); myTOLED.pixelSet(14, 22); myTOLED.pixelSet(15, 22); myTOLED.pixelSet(16, 22); myTOLED.pixelSet(17, 22); myTOLED.pixelSet(18, 22); myTOLED.pixelSet(19, 22); 
  myTOLED.pixelSet(4, 23); myTOLED.pixelSet(5, 23); myTOLED.pixelSet(6, 23); myTOLED.pixelSet(7, 23); myTOLED.pixelSet(8, 23); myTOLED.pixelSet(12, 23); myTOLED.pixelSet(13, 23); myTOLED.pixelSet(14, 23); myTOLED.pixelSet(15, 23); myTOLED.pixelSet(16, 23); 
  myTOLED.pixelSet(4, 24); myTOLED.pixelSet(5, 24); myTOLED.pixelSet(6, 24); myTOLED.pixelSet(7, 24); myTOLED.pixelSet(13, 24); myTOLED.pixelSet(14, 24); myTOLED.pixelSet(15, 24); myTOLED.pixelSet(16, 24); 
  myTOLED.pixelSet(3, 25); myTOLED.pixelSet(4, 25); myTOLED.pixelSet(5, 25); myTOLED.pixelSet(6, 25); myTOLED.pixelSet(14, 25); myTOLED.pixelSet(15, 25); myTOLED.pixelSet(16, 25); myTOLED.pixelSet(17, 25); myTOLED.pixelSet(18, 25); 
  myTOLED.pixelSet(2, 26); myTOLED.pixelSet(3, 26); myTOLED.pixelSet(4, 26); myTOLED.pixelSet(5, 26); myTOLED.pixelSet(6, 26); myTOLED.pixelSet(14, 26); myTOLED.pixelSet(15, 26); myTOLED.pixelSet(16, 26); myTOLED.pixelSet(17, 26); myTOLED.pixelSet(18, 26); myTOLED.pixelSet(19, 26); 
  myTOLED.pixelSet(1, 27); myTOLED.pixelSet(2, 27); myTOLED.pixelSet(3, 27); myTOLED.pixelSet(4, 27); myTOLED.pixelSet(5, 27); myTOLED.pixelSet(6, 27); myTOLED.pixelSet(14, 27); myTOLED.pixelSet(15, 27); myTOLED.pixelSet(16, 27); myTOLED.pixelSet(17, 27); myTOLED.pixelSet(18, 27); myTOLED.pixelSet(19, 27); myTOLED.pixelSet(20, 27);  
}


void showPipe( void ){
  myTOLED.pixelSet(2, 2); myTOLED.pixelSet(3, 2); myTOLED.pixelSet(4, 2); myTOLED.pixelSet(5, 2); myTOLED.pixelSet(6, 2); myTOLED.pixelSet(7, 2); myTOLED.pixelSet(8, 2); myTOLED.pixelSet(9, 2); myTOLED.pixelSet(10, 2); myTOLED.pixelSet(11, 2); myTOLED.pixelSet(12, 2); myTOLED.pixelSet(13, 2); myTOLED.pixelSet(14, 2); myTOLED.pixelSet(15, 2); myTOLED.pixelSet(16, 2); myTOLED.pixelSet(17, 2); myTOLED.pixelSet(18, 2); myTOLED.pixelSet(19, 2); myTOLED.pixelSet(20, 2); myTOLED.pixelSet(21, 2); myTOLED.pixelSet(22, 2); 
  myTOLED.pixelSet(2, 3); myTOLED.pixelSet(22, 3); 
  myTOLED.pixelSet(2, 4); myTOLED.pixelSet(12, 4); myTOLED.pixelSet(13, 4); myTOLED.pixelSet(14, 4); myTOLED.pixelSet(15, 4); myTOLED.pixelSet(16, 4); myTOLED.pixelSet(22, 4); 
  myTOLED.pixelSet(2, 5); myTOLED.pixelSet(12, 5); myTOLED.pixelSet(13, 5); myTOLED.pixelSet(14, 5); myTOLED.pixelSet(15, 5); myTOLED.pixelSet(16, 5); myTOLED.pixelSet(17, 5); myTOLED.pixelSet(22, 5); 
  myTOLED.pixelSet(2, 6); myTOLED.pixelSet(4, 6); myTOLED.pixelSet(5, 6); myTOLED.pixelSet(9, 6); myTOLED.pixelSet(12, 6); myTOLED.pixelSet(13, 6); myTOLED.pixelSet(14, 6); myTOLED.pixelSet(15, 6); myTOLED.pixelSet(16, 6); myTOLED.pixelSet(22, 6); 
  myTOLED.pixelSet(2, 7); myTOLED.pixelSet(12, 7); myTOLED.pixelSet(13, 7); myTOLED.pixelSet(14, 7); myTOLED.pixelSet(15, 7); myTOLED.pixelSet(16, 7); myTOLED.pixelSet(17, 7); myTOLED.pixelSet(22, 7); 
  myTOLED.pixelSet(2, 8); myTOLED.pixelSet(12, 8); myTOLED.pixelSet(13, 8); myTOLED.pixelSet(14, 8); myTOLED.pixelSet(15, 8); myTOLED.pixelSet(16, 8); myTOLED.pixelSet(22, 8); 
  myTOLED.pixelSet(2, 9); myTOLED.pixelSet(12, 9); myTOLED.pixelSet(13, 9); myTOLED.pixelSet(14, 9); myTOLED.pixelSet(15, 9); myTOLED.pixelSet(16, 9); myTOLED.pixelSet(22, 9); 
  myTOLED.pixelSet(2, 10); myTOLED.pixelSet(12, 10); myTOLED.pixelSet(13, 10); myTOLED.pixelSet(14, 10); myTOLED.pixelSet(15, 10); myTOLED.pixelSet(16, 10); myTOLED.pixelSet(17, 10); myTOLED.pixelSet(22, 10); 
  myTOLED.pixelSet(2, 11); myTOLED.pixelSet(4, 11); myTOLED.pixelSet(5, 11); myTOLED.pixelSet(9, 11); myTOLED.pixelSet(12, 11); myTOLED.pixelSet(13, 11); myTOLED.pixelSet(14, 11); myTOLED.pixelSet(15, 11); myTOLED.pixelSet(16, 11); myTOLED.pixelSet(17, 11); myTOLED.pixelSet(18, 11); myTOLED.pixelSet(22, 11); 
  myTOLED.pixelSet(2, 12); myTOLED.pixelSet(3, 12); myTOLED.pixelSet(4, 12); myTOLED.pixelSet(5, 12); myTOLED.pixelSet(6, 12); myTOLED.pixelSet(7, 12); myTOLED.pixelSet(8, 12); myTOLED.pixelSet(9, 12); myTOLED.pixelSet(10, 12); myTOLED.pixelSet(11, 12); myTOLED.pixelSet(12, 12); myTOLED.pixelSet(13, 12); myTOLED.pixelSet(14, 12); myTOLED.pixelSet(15, 12); myTOLED.pixelSet(16, 12); myTOLED.pixelSet(17, 12); myTOLED.pixelSet(18, 12); myTOLED.pixelSet(19, 12); myTOLED.pixelSet(20, 12); myTOLED.pixelSet(21, 12); myTOLED.pixelSet(22, 12); 
  myTOLED.pixelSet(3, 13); myTOLED.pixelSet(6, 13); myTOLED.pixelSet(7, 13); myTOLED.pixelSet(11, 13); myTOLED.pixelSet(13, 13); myTOLED.pixelSet(14, 13); myTOLED.pixelSet(15, 13); myTOLED.pixelSet(16, 13); myTOLED.pixelSet(17, 13); myTOLED.pixelSet(20, 13); myTOLED.pixelSet(21, 13); 
  myTOLED.pixelSet(3, 14); myTOLED.pixelSet(6, 14); myTOLED.pixelSet(14, 14); myTOLED.pixelSet(20, 14); myTOLED.pixelSet(21, 14); 
  myTOLED.pixelSet(3, 15); myTOLED.pixelSet(6, 15); myTOLED.pixelSet(14, 15); myTOLED.pixelSet(15, 15); myTOLED.pixelSet(20, 15); myTOLED.pixelSet(21, 15); 
  myTOLED.pixelSet(3, 16); myTOLED.pixelSet(6, 16); myTOLED.pixelSet(14, 16); myTOLED.pixelSet(15, 16); myTOLED.pixelSet(20, 16); myTOLED.pixelSet(21, 16); 
  myTOLED.pixelSet(3, 17); myTOLED.pixelSet(6, 17); myTOLED.pixelSet(14, 17); myTOLED.pixelSet(15, 17); myTOLED.pixelSet(20, 17); myTOLED.pixelSet(21, 17); 
  myTOLED.pixelSet(3, 18); myTOLED.pixelSet(6, 18); myTOLED.pixelSet(14, 18); myTOLED.pixelSet(15, 18); myTOLED.pixelSet(20, 18); myTOLED.pixelSet(21, 18); 
  myTOLED.pixelSet(3, 19); myTOLED.pixelSet(6, 19); myTOLED.pixelSet(14, 19); myTOLED.pixelSet(20, 19); myTOLED.pixelSet(21, 19); 
  myTOLED.pixelSet(3, 20); myTOLED.pixelSet(6, 20); myTOLED.pixelSet(14, 20); myTOLED.pixelSet(15, 20); myTOLED.pixelSet(20, 20); myTOLED.pixelSet(21, 20); 
  myTOLED.pixelSet(3, 21); myTOLED.pixelSet(6, 21); myTOLED.pixelSet(14, 21); myTOLED.pixelSet(15, 21); myTOLED.pixelSet(20, 21); myTOLED.pixelSet(21, 21); 
  myTOLED.pixelSet(3, 22); myTOLED.pixelSet(6, 22); myTOLED.pixelSet(14, 22); myTOLED.pixelSet(15, 22); myTOLED.pixelSet(20, 22); myTOLED.pixelSet(21, 22); 
  myTOLED.pixelSet(3, 23); myTOLED.pixelSet(6, 23); myTOLED.pixelSet(14, 23); myTOLED.pixelSet(15, 23); myTOLED.pixelSet(20, 23); myTOLED.pixelSet(21, 23); 
  myTOLED.pixelSet(3, 24); myTOLED.pixelSet(6, 24); myTOLED.pixelSet(14, 24); myTOLED.pixelSet(20, 24); myTOLED.pixelSet(21, 24); 
}

#else
void showMario( void ){
  Serial.println(F("Sorry, Uno and other low-memory microcontrollers may not be able to run the Mario code due to program memory size constraints."))
}

void showPipe( void ){
  Serial.println(F("Sorry, Uno and other low-memory microcontrollers may not be able to run the Mario code due to program memory size constraints."))
}
#endif
