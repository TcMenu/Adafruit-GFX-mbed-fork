#include "TcAdaSH1107SPI.h"

TcAdaSH1107SPI::TcAdaSH1107SPI(const SPIWithSettings& spi, pinid_t DC, pinid_t RST, int16_t rawHeight,
    UnicodeEncodingMode encodingMode) : TcAdaSH1107(RST, rawHeight, encodingMode), spiSettings(spi), dcPin(DC) {

}

void TcAdaSH1107SPI::command(uint8_t c) {
    internalDigitalDevice().digitalWriteS(dcPin, 0);
    spiSettings.transferSPI(&c, 1);
}

void TcAdaSH1107SPI::data(uint8_t* c, size_t len) {
    internalDigitalDevice().digitalWriteS(dcPin, 1);
    spiSettings.transferSPI(c, len);
}
