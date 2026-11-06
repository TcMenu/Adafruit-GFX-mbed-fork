
#ifndef _ADAFRUIT_MBEDFORK_SSD1306_SPI_H
#define _ADAFRUIT_MBEDFORK_SSD1306_SPI_H

#include "IoAbstraction.h"
#include "extras/SPIHelper.h"
#include "../Adafruit_GFX.h"
#include "../Adafruit_SSD1306.h"

/** This is the SPI SSD1306 display driver transport class
 *
 */
class AdafruitSSD1306Spi : public Adafruit_SSD1306
{
public:
    /** Create a SSD1306 SPI transport display driver instance with the specified DC, RST, and CS pins, as well as the display dimentions
     *
     * Required parameters
     * @param spi - a reference to an initialized SPI object
     * @param DC (Data/Command) pin name
     * @param RST (Reset) pin name
     * @param CS (Chip Select) pin name
     *
     * Optional parameters
     * @param rawHeight - the vertical number of pixels for the display, defaults to 32
     * @param rawWidth - the horizonal number of pixels for the display, defaults to 128
     */
    AdafruitSSD1306Spi(const SPIWithSettings &spi, pinid_t DC, pinid_t RST, int16_t rawHeight, int16_t rawWidth,
                       UnicodeEncodingMode encodingMode, OledType displayType = SSD_1306);
    void command(uint8_t c) override;
    void data(uint8_t c) override;

protected:
    void sendDisplayBuffer() override;

    pinid_t dcPin;
    OledType mDisplayType;
    SPIWithSettings mspi;
};

#endif //_ADAFRUIT_MBEDFORK_SSD1306_SPI_H
