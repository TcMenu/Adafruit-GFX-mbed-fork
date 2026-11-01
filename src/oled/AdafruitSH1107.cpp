#include "AdafruitSH1107.h"


TcAdaSH1107::TcAdaSH1107(pinid_t RST, int16_t rawHeight, UnicodeEncodingMode encodingMode)
    : Adafruit_GFX(128, rawHeight, encodingMode) {
}

TcAdaSH1107::TcAdaSH1107(pinid_t RST, UnicodeEncodingMode encodingMode) : Adafruit_GFX(128, 128, encodingMode) {
}

void TcAdaSH1107::begin() {
    command(SH1107_DISPLAY_OFF);
    command(SH1107_SET_DISPLAY_START_LINE);command(0);
    command(SH1107_SET_CONTRAST);command(0x2F);
    command(SH1107_SET_SEG_NORMAL); // normal segment mapping
    command(SH1107_SET_COM_NORMAL); // direction mapping
    command(SH1107_SET_MULTIPLEX_RATIO);command(0x7F);
    command(SH1107_SET_DISPLAY_OFFSET);command(0x00);
    command(SH1107_SET_DISPLAY_CLOCK_DIV);command(0x51);
    command(SH1107_SET_PRECHARGE_PERIOD);command(0x22);
    command(SH1107_SET_VCOM_DESELECT);command(0x35);
    command(SH1107_DISPLAY_ENTIRE_OFF);
    command(SH1107_DISPLAY_NORMAL);
    command(SH1107_DISPLAY_ON);
}

void TcAdaSH1107::drawPixel(int16_t x, int16_t y, color_t color) {
    if ((x < 0) || (x >= width()) || (y < 0) || (y >= height())) {
        return;
    }

    // Transform coordinates according to rotation
    switch (getRotation()) {
    case 1:
        // 90 degrees clockwise
        swap(x, y);
        x = WIDTH - x - 1;
        break;
    case 2:
        // 180 degrees
        x = WIDTH - x - 1;
        y = HEIGHT - y - 1;
        break;
    case 3:
        // 270 degrees clockwise (90 counter-clockwise)
        swap(x, y);
        y = HEIGHT - y - 1;
        break;
    case 0:
    default:
        // No rotation
        break;
    }

    // Calculate buffer index and bit mask using the physical display width (WIDTH)
    const uint16_t index = x + ((y / 8) * WIDTH);
    const uint8_t bitMask = 1 << (y % 8);

    if (index > buffSize) return; // do not draw pixel if index is out of bounds

    if (color) {
        buffer[index] |= bitMask;   // Set pixel ON
    } else {
        buffer[index] &= ~bitMask;  // Set pixel OFF
    }
}

void TcAdaSH1107::display() {
    auto displayPages = HEIGHT >> 3;
    uint8_t spiBuffer[128];
    for (int i=0; i < displayPages; i++) {
        command(SH1107_SET_PAGE_ADDR(i));
        command(SH1107_SET_COL_ADDR_LSB(0));
        command(SH1107_SET_COL_ADDR_MSB(0));
        memcpy(spiBuffer, buffer + (i * WIDTH), sizeof spiBuffer);
        data(spiBuffer, sizeof spiBuffer);
    }
}



