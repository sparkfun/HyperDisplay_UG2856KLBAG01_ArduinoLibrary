/*

A library to control SSD1309 OLED drivers, derived from the SparkFun HyperDisplay library

Created: August 2018
Modified: August 2018

Authors: Owen Lyke

*/

#ifndef _HPYERDISPLAY_UG2856KLBAG01_H_
#define _HPYERDISPLAY_UG2856KLBAG01_H_


////////////////////////////////////////////////////////////
//							Includes    				  //
////////////////////////////////////////////////////////////
#include "HyperDisplay_SSD1309.h"



////////////////////////////////////////////////////////////
//				UG2856KLBAG01 I2C Display Class   		  //
////////////////////////////////////////////////////////////

#define UG2856KLBAG01_WIDTH 	128
#define UG2856KLBAG01_HEIGHT 	64
#define UG2856KLBAG01_START_ROW	0x00
#define UG2856KLBAG01_START_COL	0x00
#define UG2856KLBAG01_STOP_ROW 	0x3F
#define UG2856KLBAG01_STOP_COL	0x7F
#define UG2856KLBAG01_START_PG 	0x00
#define UG2856KLBAG01_STOP_PG	0x07

class UG2856KLBAG01_I2C : public SSD1309_Arduino_I2C{							// This is a particular display from WiseChip. Get it here: 
private:
protected:
public:
	UG2856KLBAG01_I2C( void );	// Note that since the hardware screen choice (UG2856KLBAG01) constrained the dimensions there are no more free parameters to pass to the initialization

	SSD1309_Status_t begin(TwoWire &wirePort = Wire, bool sa0Val = false, uint8_t saoPin = SSD1309_ARD_UNUSED_PIN);
	SSD1309_Status_t defaultConfigure( void ); // The reccomended settings from the datasheet
	void startup( void );		// The default startup for this particular display

	// void getCharInfo(uint8_t val, char_info_t * pchar);

	// Some specialized drawing frunctions
	void clearDisplay( void );

	// Special Functions
	void setWindowDefaults(wind_info_t * pwindow); // Overrrides default implementation in hyperdisplay class
};






////////////////////////////////////////////////////////////
//				UG2856KLBAG01 SPI Display Class   		  //
////////////////////////////////////////////////////////////

#define UG2856KLBAG01_SPI_MODE 		SPI_MODE3
#define UG2856KLBAG01_SPI_ORDER		MSBFIRST

class UG2856KLBAG01_SPI : public SSD1309_Arduino_SPI{							// This is a particular display from WiseChip. Get it here: 
private:
protected:
public:
	UG2856KLBAG01_SPI( void );	// Note that since the hardware screen choice (UG2856KLBAG01) constrained the dimensions there are no more free parameters to pass to the initialization

	SSD1309_Status_t begin( uint8_t csPin, uint8_t dcPin, SPIClass &spiPort = SPI);
	SSD1309_Status_t defaultConfigure( void ); // The reccomended settings from the datasheet
	void startup( void );		// The default startup for this particular display

	// void getCharInfo(uint8_t val, char_info_t * pchar);

	// Some specialized drawing frunctions
	void clearDisplay( void );

	// Special Functions
	void setWindowDefaults(wind_info_t * pwindow); // Overrrides default implementation in hyperdisplay class
};


#endif /* _HPYERDISPLAY_UG2856KLBAG01_H_ */