
#ifndef _ADAFRUIT_MBEDFORK_SSD1306_I2C_H
#define _ADAFRUIT_MBEDFORK_SSD1306_I2C_H

#include <PlatformDeterminationWire.h>
#include "../Adafruit_GFX.h"
#include "../Adafruit_SSD1306.h"

/** This is the I2C SSD1306 display driver transport class
 *
 */
class AdafruitSSD1306I2c : public Adafruit_SSD1306
{
public:
    /** Create a SSD1306 I2C transport display driver instance with the specified RST pin name, the I2C address, as well as the display dimensions
     *
     * Required parameters
     * @param i2c - A reference to an initialized I2C object
     * @param RST - The Reset pin name
     *
     * Optional parameters
     * @param i2cAddress - The i2c address of the display
     * @param rawHeight - The vertical number of pixels for the display, defaults to 32
     * @param rawWidth - The horizonal number of pixels for the display, defaults to 128
     */
    AdafruitSSD1306I2c(WireType wirePtr, pinid_t RST, uint8_t i2cAddress, uint8_t rawHeight, uint8_t rawWidth,
                       UnicodeEncodingMode encodingMode, OledType displayType = SSD_1306)
            : Adafruit_SSD1306(RST, rawHeight, rawWidth, encodingMode)
            , wireInstance(wirePtr)
            , mi2cAddress(i2cAddress)
            , mDisplayType(displayType) { }

    virtual ~AdafruitSSD1306I2c() = default;

    void command(uint8_t c) override;
    void data(uint8_t c) override;

protected:
    void sendDisplayBuffer() override;
    WireType wireInstance;
    uint8_t mi2cAddress;
    OledType mDisplayType;
};

#endif //_ADAFRUIT_MBEDFORK_SSD1306_I2C_H
