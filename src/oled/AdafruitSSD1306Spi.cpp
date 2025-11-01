#include "AdafruitSSD1306Spi.h"

AdafruitSSD1306Spi::AdafruitSSD1306Spi(const SPIWithSettings &spi, pinid_t DC, pinid_t RST, int16_t rawHeight, int16_t rawWidth,
                                       UnicodeEncodingMode encodingMode, OledType displayType)
    : Adafruit_SSD1306(RST, rawHeight, rawWidth, encodingMode)
            , dcPin(DC), mDisplayType(displayType), mspi(spi) {
    internalDigitalDevice().pinMode(dcPin, OUTPUT);
    internalDigitalDevice().digitalWriteS(dcPin, LOW);
}


void AdafruitSSD1306Spi::command(uint8_t c) {
     internalDigitalDevice().digitalWriteS(dcPin, 0);
     mspi.transferSPI(&c, 1);
}

void AdafruitSSD1306Spi::data(uint8_t c) {
    internalDigitalDevice().digitalWriteS(dcPin, 1);
    mspi.transferSPI(&c, 1);
}

void AdafruitSSD1306Spi::sendDisplayBuffer() {
    uint8_t buff[132];
    auto rows = HEIGHT / 8;
    auto col = 0U;
    for(uint8_t row=0; row < rows; row++) {
        // for each row we go into command mode and send the new row offset, SH1106 cannot do this
        // automatically, so we need to do it for it.
        command(0xB0 + row);//set page address
        command(col & 0xfU);//set lower column address
        command(0x10U | (col >> 4U));//set higher column address

        internalDigitalDevice().digitalWriteS(dcPin, 1);

        // now we prepare a whole row of data, no need to mess around with smaller segments, then dump
        // the lot into i2c.
        for(int i=0;i<WIDTH;i++) {
            buff[i] = buffer[(row * WIDTH) + i];
        }
        mspi.transferSPI(buff, WIDTH);
    }

}