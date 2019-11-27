#include "HyperDisplay_UG2856KLBAG01.h"

////////////////////////////////////////////////////////////
//				UG6464TDDBG01 Implementation			  //
////////////////////////////////////////////////////////////
char_info_t UG6464TDDBG01_Default_CharInfo;
wind_info_t UG6464TDDBG01_Default_Window;

UG2856KLBAG01_I2C::UG2856KLBAG01_I2C( void ) : hyperdisplay(UG2856KLBAG01_WIDTH, UG2856KLBAG01_HEIGHT), SSD1309_Arduino_I2C(UG2856KLBAG01_WIDTH, UG2856KLBAG01_HEIGHT)
{

}

SSD1309_Status_t UG2856KLBAG01_I2C::begin(TwoWire &wirePort, bool sa0Val, uint8_t saoPin)
{
	// Call the functions to setup the super classes
// Associate 
	_sa0 = saoPin;
	_i2c = &wirePort;
	_sa0val = sa0Val;

	// Set pinmodes
	if(_sa0 != SSD1309_ARD_UNUSED_PIN){ pinMode(_sa0, OUTPUT); }

	// Set pins to default positions
	if(_sa0 != SSD1309_ARD_UNUSED_PIN){ digitalWrite(_sa0, (uint8_t)_sa0val); }

	// _i2c->begin(); // Moved into user's sketch

	// Setup the default window
	setWindowDefaults(pCurrentWindow);

	// Power up the device
	startup();	
	defaultConfigure();

	return SSD1309_Nominal;
}

SSD1309_Status_t UG2856KLBAG01_I2C::defaultConfigure( void )
{
	// This is the suggested initialization routine from WiseChip (pg. 9 of the module datasheet)
	setCommandLock(false);
	setPower(false); 

	setDisplayClockDivideRatio(0xA0);
	setMultiplexRatio(0x3F);
	setDisplayOffset(0x00);
	setDisplayStartLine(0x00);
	setMemoryAddressingMode( 0x00 ); // Horizontal Addressing Mode
	setSegmentMapping(true);
	setCOMoutputDirection(true);
	setCOMpinsHWconfig(0x12);
	setContrastControl(0x8F);
	setPreChargePeriod(0x25);
	setVCOMHdeselectLevel(0x34);
	overrideRam(false);
	setInversion(false);
	
	// Clear the ram mirror and output to the display
	memset( ramMirror, 0x00, (SSD1309_RAM_BYTES)*sizeof(SSD1309_Bite_t));
	updateRefreshZone( UG2856KLBAG01_START_COL, UG2856KLBAG01_STOP_COL, UG2856KLBAG01_START_ROW, UG2856KLBAG01_STOP_ROW);		// Tell where we need updates (the whole screen)   
	refreshDisplay();								// Perform updates

	setPower(true); 

  	return SSD1309_Nominal;
}

void UG2856KLBAG01_I2C::startup( void )
{
	// Assume that VDD and VCC are stable when this function is called
	delay(20);
	digitalWrite(_rst, LOW);
	// delayMicroseconds(10);
	delay(5);
	digitalWrite(_rst, HIGH);
	delay(200);
	// Now you can do initialization
}

// void UG6464TDDBG01::getCharInfo(uint8_t val, char_info_t * pchar)
// {
// 	char_info_t * pcharinfo = &UG6464TDDBG01_Default_CharInfo;
// 	// Do stuff to fill out the default char info structure, then return a pointer to it
// 	pcharinfo->data = NULL;						// Color information for each pixel to be drawn
// 	pcharinfo->xLoc = NULL;						// X location wrt upper-left corner of char location(cursor) for each pixel
// 	pcharinfo->yLoc = NULL;						// Y location wrt upper-left corner of char location(cursor) for each pixel
//     pcharinfo->numPixels = 0;					// Number of pixels contained in the data, xLoc, and yLoc arrays
// 	pcharinfo->show = false;					// Whether or not to actually show the character
// 	pcharinfo->causedNewline = false;			// Whether or not the character triggered/triggers a new line
// }


void UG2856KLBAG01_I2C::clearDisplay( void )
{
	// Store the old window pointer: 
	wind_info_t * ptempWind = pCurrentWindow;

	// Make a new default window
	wind_info_t window;
	pCurrentWindow = &window;

	// Ensure the window is set up so that you can clear the whole screen
	setWindowDefaults(&window);

	// Make a local 'black' color 
	SSD1309_Bite_t blk;
	blk.bite = 0x00;
	
	// Fill the temporary window with black
	fillWindow((color_t)&blk);		

	// Restore the previous window
	pCurrentWindow = ptempWind;
}

void UG2856KLBAG01_I2C::setWindowDefaults(wind_info_t * pwindow)
{
	// Fills out the default window structure with more or less reasonable defaults
	pwindow->xMin = UG2856KLBAG01_START_COL;
	pwindow->yMin = UG2856KLBAG01_START_ROW;
	pwindow->xMax = UG2856KLBAG01_STOP_COL;
	pwindow->yMax = UG2856KLBAG01_STOP_ROW;
	pwindow->cursorX = 0;							// cursor values are in window coordinates
	pwindow->cursorY = 0;
	pwindow->xReset = 0;
	pwindow->yReset = 0;
	
	pwindow->lastCharacter.data = NULL;
	pwindow->lastCharacter.xLoc = NULL;
	pwindow->lastCharacter.yLoc = NULL;
	pwindow->lastCharacter.xDim = 0;
	pwindow->lastCharacter.yDim = 0;
	pwindow->lastCharacter.numPixels = 0;
	pwindow->lastCharacter.show = false;
	pwindow->lastCharacter.causesNewline = false;
	
	pwindow->bufferMode = false;			// Start out in direct mode
	pwindow->data = NULL;							// No window data yet
	pwindow->numPixels = 0;
	pwindow->dynamic = false;
	setWindowColorSet(pwindow);				// Set the default window color to 'Set' pixels
}























UG2856KLBAG01_SPI::UG2856KLBAG01_SPI( void ) : hyperdisplay(UG2856KLBAG01_WIDTH, UG2856KLBAG01_HEIGHT), SSD1309_Arduino_SPI(UG2856KLBAG01_WIDTH, UG2856KLBAG01_HEIGHT)
{

}

SSD1309_Status_t UG2856KLBAG01_SPI::begin( uint8_t csPin, uint8_t dcPin, SPIClass &spiPort)
{
	// Call the functions to setup the super classes
// Associate 
	_cs = csPin;
	_dc = dcPin;
	_spi = &spiPort;

	// Set pinmodes
	pinMode(_cs, OUTPUT);
	pinMode(_dc, OUTPUT);

	// Set pins to default positions
	digitalWrite(_cs, HIGH);
	digitalWrite(_dc, HIGH);

	// Setup the default window
	setWindowDefaults(pCurrentWindow);

	// Kick the SPI hardware into the right mode
	_spi->beginTransaction(SPISettings(UG2856KLBAG01_SPI_FREQ_MAX, UG2856KLBAG01_SPI_ORDER, UG2856KLBAG01_SPI_MODE));
	_spi->transfer(0x00);
	_spi->endTransaction();

	// Power up the device
	startup();	
	defaultConfigure();

	return SSD1309_Nominal;
}

SSD1309_Status_t UG2856KLBAG01_SPI::defaultConfigure( void )
{
	// This is the suggested initialization routine from WiseChip (pg. 9 of the module datasheet)
	setCommandLock(false);
	setPower(false); 

	setDisplayClockDivideRatio(0xA0);
	setMultiplexRatio(0x3F);
	setDisplayOffset(0x00);
	setDisplayStartLine(0x00);
	setMemoryAddressingMode( 0x00 ); // Horizontal Addressing Mode
	setSegmentMapping(true);
	setCOMoutputDirection(true);
	setCOMpinsHWconfig(0x12);
	setContrastControl(0x8F);
	setPreChargePeriod(0x25);
	setVCOMHdeselectLevel(0x34);
	overrideRam(false);
	setInversion(false);
	
	// Clear the ram mirror and output to the display
	memset( ramMirror, 0x00, (SSD1309_RAM_BYTES)*sizeof(SSD1309_Bite_t));
	updateRefreshZone( UG2856KLBAG01_START_COL, UG2856KLBAG01_STOP_COL, UG2856KLBAG01_START_ROW, UG2856KLBAG01_STOP_ROW);		// Tell where we need updates (the whole screen)   
	refreshDisplay();								// Perform updates

	setPower(true); 

  	return SSD1309_Nominal;
}

void UG2856KLBAG01_SPI::startup( void )
{
	// Assume that VDD and VCC are stable when this function is called
	if(_rst )
	delay(20);
	digitalWrite(_rst, LOW);
	// delayMicroseconds(10);
	delay(5);
	digitalWrite(_rst, HIGH);
	delay(200);
	// Now you can do initialization
}

// void UG6464TDDBG01::getCharInfo(uint8_t val, char_info_t * pchar)
// {
// 	char_info_t * pcharinfo = &UG6464TDDBG01_Default_CharInfo;
// 	// Do stuff to fill out the default char info structure, then return a pointer to it
// 	pcharinfo->data = NULL;						// Color information for each pixel to be drawn
// 	pcharinfo->xLoc = NULL;						// X location wrt upper-left corner of char location(cursor) for each pixel
// 	pcharinfo->yLoc = NULL;						// Y location wrt upper-left corner of char location(cursor) for each pixel
//     pcharinfo->numPixels = 0;					// Number of pixels contained in the data, xLoc, and yLoc arrays
// 	pcharinfo->show = false;					// Whether or not to actually show the character
// 	pcharinfo->causedNewline = false;			// Whether or not the character triggered/triggers a new line
// }


void UG2856KLBAG01_SPI::clearDisplay( void )
{
	// Store the old window pointer: 
	wind_info_t * ptempWind = pCurrentWindow;

	// Make a new default window
	wind_info_t window;
	pCurrentWindow = &window;

	// Ensure the window is set up so that you can clear the whole screen
	setWindowDefaults(&window);

	// Make a local 'black' color 
	SSD1309_Bite_t blk;
	blk.bite = 0x00;
	
	// Fill the temporary window with black
	fillWindow((color_t)&blk);		

	// Restore the previous window
	pCurrentWindow = ptempWind;
}

void UG2856KLBAG01_SPI::setWindowDefaults(wind_info_t * pwindow)
{
	// Fills out the default window structure with more or less reasonable defaults
	pwindow->xMin = UG2856KLBAG01_START_COL;
	pwindow->yMin = UG2856KLBAG01_START_ROW;
	pwindow->xMax = UG2856KLBAG01_STOP_COL;
	pwindow->yMax = UG2856KLBAG01_STOP_ROW;
	pwindow->cursorX = 0;							// cursor values are in window coordinates
	pwindow->cursorY = 0;
	pwindow->xReset = 0;
	pwindow->yReset = 0;
	
	pwindow->lastCharacter.data = NULL;
	pwindow->lastCharacter.xLoc = NULL;
	pwindow->lastCharacter.yLoc = NULL;
	pwindow->lastCharacter.xDim = 0;
	pwindow->lastCharacter.yDim = 0;
	pwindow->lastCharacter.numPixels = 0;
	pwindow->lastCharacter.show = false;
	pwindow->lastCharacter.causesNewline = false;
	
	pwindow->bufferMode = false;			// Start out in direct mode
	pwindow->data = NULL;				// No window data yet
	pwindow->numPixels = 0;
	pwindow->dynamic = false;
	setWindowColorSequence(pwindow, NULL, 1, 0);	// Setup the default color (Which is NULL, so that you know it is not set yet)
}