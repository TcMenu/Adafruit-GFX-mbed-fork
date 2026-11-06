#ifndef TC_ADAFRUIT_SSD1107_SPI_H
#define TC_ADAFRUIT_SSD1107_SPI_H
#include "extras/SPIHelper.h"
#include "AdafruitSH1107.h"

class TcAdaSH1107SPI : public TcAdaSH1107 {
private:
    SPIWithSettings spiSettings;
    pinid_t dcPin;
public:
    TcAdaSH1107SPI(const SPIWithSettings& spi, pinid_t DC, pinid_t RST, int16_t rawHeight,UnicodeEncodingMode encodingMode)
                : TcAdaSH1107(RST, rawHeight, encodingMode), spiSettings(spi), dcPin(DC) {}

    void command(uint8_t c) override {
        internalDigitalDevice().digitalWriteS(dcPin, 0);
        spiSettings.transferSPI(&c, 1);
    }

    void data(uint8_t* c, size_t len) override {
        internalDigitalDevice().digitalWriteS(dcPin, 1);
        spiSettings.transferSPI(c, len);
    }
};

#endif
