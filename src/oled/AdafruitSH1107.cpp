#include "AdafruitSH1107.h"


TcAdaSH1107::TcAdaSH1107(pinid_t RST, int16_t rawHeight, UnicodeEncodingMode encodingMode)
    : Adafruit_GFX(128, rawHeight, encodingMode) {
    buffSize = (rawHeight / 8) * 128;
    buffer = new uint8_t[buffSize];
}

TcAdaSH1107::TcAdaSH1107(pinid_t RST, UnicodeEncodingMode encodingMode) : Adafruit_GFX(128, 128, encodingMode) {
    buffSize = (128 / 8) * 128;
    buffer = new uint8_t[buffSize];
}

TcAdaSH1107::~TcAdaSH1107() {
    delete[] buffer;
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

    if (!buffer || index >= buffSize) return; // do not draw pixel if index is out of bounds

    if (color) {
        buffer[index] |= bitMask;   // Set pixel ON
    } else {
        buffer[index] &= ~bitMask;  // Set pixel OFF
    }
}

void TcAdaSH1107::writeFastVLine(int16_t x, int16_t y, int16_t h, color_t color) {
    if ((x < 0) || (x >= width()) || (h == 0)) {
        return;
    }

    if (h < 0) {
        y += h + 1;
        h = -h;
    }

    if (y < 0) {
        h += y;
        y = 0;
    }

    if (y >= height() || h <= 0) {
        return;
    }

    if (y + h > height()) {
        h = height() - y;
    }

    switch (getRotation()) {
        case 0:
            drawFastRawVLine(x, y, h, color);
            break;
        case 1:
            drawFastRawHLine(WIDTH - y - h, x, h, color);
            break;
        case 2:
            drawFastRawVLine(WIDTH - 1 - x, HEIGHT - y - h, h, color);
            break;
        case 3:
            drawFastRawHLine(y, HEIGHT - 1 - x, h, color);
            break;
    }
}

void TcAdaSH1107::writeFastHLine(int16_t x, int16_t y, int16_t w, color_t color) {
    if ((y < 0) || (y >= height()) || (w == 0)) {
        return;
    }

    if (w < 0) {
        x += w + 1;
        w = -w;
    }

    if (x < 0) {
        w += x;
        x = 0;
    }

    if (x >= width() || w <= 0) {
        return;
    }

    if (x + w > width()) {
        w = width() - x;
    }

    switch (getRotation()) {
        case 0:
            drawFastRawHLine(x, y, w, color);
            break;
        case 1:
            drawFastRawVLine(WIDTH - 1 - y, x, w, color);
            break;
        case 2:
            drawFastRawHLine(WIDTH - x - w, HEIGHT - 1 - y, w, color);
            break;
        case 3:
            drawFastRawVLine(y, HEIGHT - x - w, w, color);
            break;
    }
}

void TcAdaSH1107::drawFastRawVLine(int16_t x, int16_t y, int16_t h, color_t color) {
    if (!buffer || (x < 0) || (x >= WIDTH) || (h <= 0)) {
        return;
    }
    if (y < 0) {
        h += y;
        y = 0;
    }
    if ((y >= HEIGHT) || (h <= 0)) {
        return;
    }
    if (y + h > HEIGHT) {
        h = HEIGHT - y;
    }

    int16_t start_page = y / 8;
    int16_t end_page = (y + h - 1) / 8;
    uint8_t start_bit = y & 7;
    uint8_t end_bit = (y + h - 1) & 7;

    uint8_t *pBuf = &buffer[x + start_page * WIDTH];

    if (start_page == end_page) {
        uint8_t mask = ((1U << h) - 1) << start_bit;
        if (color) {
            *pBuf |= mask;
        } else {
            *pBuf &= ~mask;
        }
    } else {
        uint8_t first_mask = 0xFF << start_bit;
        if (color) {
            *pBuf |= first_mask;
        } else {
            *pBuf &= ~first_mask;
        }
        pBuf += WIDTH;

        int16_t middle_pages = end_page - start_page - 1;
        if (color) {
            while (middle_pages-- > 0) {
                *pBuf = 0xFF;
                pBuf += WIDTH;
            }
        } else {
            while (middle_pages-- > 0) {
                *pBuf = 0x00;
                pBuf += WIDTH;
            }
        }

        uint8_t last_mask = (1U << (end_bit + 1)) - 1;
        if (color) {
            *pBuf |= last_mask;
        } else {
            *pBuf &= ~last_mask;
        }
    }
}

void TcAdaSH1107::drawFastRawHLine(int16_t x, int16_t y, int16_t w, color_t color) {
    if (!buffer || (y < 0) || (y >= HEIGHT) || (w <= 0)) {
        return;
    }
    if (x < 0) {
        w += x;
        x = 0;
    }
    if ((x >= WIDTH) || (w <= 0)) {
        return;
    }
    if (x + w > WIDTH) {
        w = WIDTH - x;
    }

    uint8_t *pBuf = &buffer[x + (y / 8) * WIDTH];
    uint8_t mask = 1 << (y & 7);
    if (color) {
        while (w--) {
            *pBuf++ |= mask;
        }
    } else {
        mask = ~mask;
        while (w--) {
            *pBuf++ &= mask;
        }
    }
}

void TcAdaSH1107::display() {
    const auto displayPages = HEIGHT >> 3;
    const uint8_t colOffsetLoc = colOffset;

    // 1. Set Display Start Line (Two-byte sequence)
    // First byte calls the command; second byte assigns the 0-127 RAM row mapping
    command(SH1107_SET_DISPLAY_START_LINE);
    command(rowOffset & 0x7F);

    for (int i=0; i < displayPages; i++) {
        command(SH1107_SET_PAGE_ADDR(i));
        command(SH1107_SET_COL_ADDR_LSB(colOffsetLoc));
        command(SH1107_SET_COL_ADDR_MSB(colOffsetLoc));
        data(buffer + (i * WIDTH), 128);
    }
}



