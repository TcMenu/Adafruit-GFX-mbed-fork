/*********************************************************************
This is a library for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

These displays use SPI to communicate, 4 or 5 pins are required to  
interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

/*
 *  Modified by Neal Horman 7/14/2012 for use in mbed
 */

#ifndef _ADAFRUIT_SSD1306_H_
#define _ADAFRUIT_SSD1306_H_

#include <PlatformDetermination.h>
#include "Adafruit_GFX.h"

#ifndef PROGMEM
#define PROGMEM
#endif // PROGMEM

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

enum OledType { SSD_1306, SH_1106 };

/** The pure base class for the SSD1306 display driver.
 *
 * You should derive from this for a new transport interface type,
 * such as the SPI and I2C drivers.
 */
class Adafruit_SSD1306 : public Adafruit_GFX
{
protected:
    virtual void sendDisplayBuffer() = 0;
    pinid_t rst;

    // the memory buffer for the LCD
    uint8_t* buffer;
    size_t buffSize;
public:
	Adafruit_SSD1306(pinid_t RST, int16_t rawHeight, int16_t rawWidth, UnicodeEncodingMode encodingMode);

	void begin(uint8_t switchvcc = SSD1306_SWITCHCAPVCC);
	
	// These must be implemented in the derived transport driver
	virtual void command(uint8_t c) = 0;
	virtual void data(uint8_t c) = 0;
	void drawPixel(int16_t x, int16_t y, color_t color) override;

	/// Clear the display buffer    
	void clearDisplay();
	virtual void invertDisplay(bool i);

	/// Cause the display to be updated with the buffer content.
	void display();
	/// Fill the buffer with the AdaFruit splash screen.
	virtual void splash();
};

#endif