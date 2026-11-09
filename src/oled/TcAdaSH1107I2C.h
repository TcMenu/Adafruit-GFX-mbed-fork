#ifndef TC_ADAFRUIT_SSD1107_SPI_H
#define TC_ADAFRUIT_SSD1107_SPI_H

#include "IoAbstractionWire.h"
#include "AdafruitSH1107.h"

class TcAdaSH1107I2C : public TcAdaSH1107 {
private:
    WireType wireRef;
    uint8_t i2cAddress;
    uint8_t sendBuf[130];
public:
    TcAdaSH1107I2C(const WireType wireRef, uint8_t i2cAddress, int16_t rawHeight,UnicodeEncodingMode encodingMode)
                : TcAdaSH1107(-1, rawHeight, encodingMode), wireRef(wireRef), i2cAddress(i2cAddress) {}

    void command(uint8_t c) override {
        uint8_t buff[2];
        buff[0] = 0; // Command Mode
        buff[1] = c;
        ioaWireWriteWithRetry(wireRef, i2cAddress, buff, sizeof(buff));

    }

    void data(uint8_t* c, size_t len) override {
        if (len > 128) return;
        sendBuf[0] = 0x40;
        memcpy(sendBuf + 1, c, len);
        ioaWireWriteWithRetry(wireRef, i2cAddress, sendBuf, len + 1);
    }
};

#endif
