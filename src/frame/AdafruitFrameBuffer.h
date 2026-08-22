/**
 *
 */
#ifndef TC_ADAFRUITFRAMEBUFFER_H
#define TC_ADAFRUITFRAMEBUFFER_H

#include "Adafruit_GFX.h"

/**
 * This is a specialisation of Adafruit_GFX that uses a memory buffer to store pixel data. It is a template that can
 * work with any type of memory arrangement in the buffer. The tested cases are specialisations of this template for
 * uint16_t (565 color) and uint32_t for 888 alpha color. Any other arrangement is untested and will need careful
 * testing.
 *
 * This uses the processor to draw all the primitives and also to do fills and bitmap operations. At least the fills and
 * bitmap operations can often be accelerated, and in that case an extension of this class should be made that overrides
 * writeFillRect and the bitmap operations that can be accelerated.
 *
 * However, even without acceleration, this class will produce a reasonable result.
 *
 * @tparam MemTy the memory type usually either uint16_t or uint32_t
 */
template <typename MemTy>
class AdafruitFrameBuffer : public Adafruit_GFX {
protected:
    MemTy* buffer;
    bool yInverted = true;
public:
    AdafruitFrameBuffer(MemTy* buffer, int16_t rawWidth, int16_t rawHeight) :
                        Adafruit_GFX(rawWidth, rawHeight, ENCMODE_UTF8) {
        this->buffer = buffer;
    }

    void init(bool p_yInverted) {
        yInverted = p_yInverted;
    }

    ~AdafruitFrameBuffer() override = default;

    void drawPixel(int16_t x, int16_t y, color_t color) override {
        if (Coord coord; correctDimensions(x, y, coord)) {
            buffer[coord.x + (coord.y * WIDTH)] = color;
        }
    }

    void fillRect(const int16_t x, const int16_t y, const int16_t w, const int16_t h, const color_t color) override {
        writeFillRect(x, y, w, h, color);
    }

    void writePixel(int16_t x, int16_t y, color_t color) override {
        drawPixel(x, y, color);
    }

    void writeFastVLine(int16_t x, int16_t y, int16_t h, color_t color) override;
    void writeFastHLine(int16_t x, int16_t y, int16_t w, color_t color) override;

    virtual void drawBitmapNBpp(const Coord& where, const uint8_t* data, const Coord& size, int bpp, const color_t* palette);

    bool correctDimensions(int16_t x, int16_t y, Coord& coord) const {
        if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT) return false;
        coord.x = x;
        if (yInverted) {
            coord.y = HEIGHT - y;
        } else {
            coord.y = y;
        }
        return true;
    }
};

template <typename MemTy>
void AdafruitFrameBuffer<MemTy>::writeFastVLine(int16_t x, int16_t y, int16_t h, color_t color) {
    Coord start;
    if (!correctDimensions(x, y, start)) return;
    if (h + start.y > HEIGHT) {
        h = HEIGHT - 1;;
    }

    for (int16_t i = 0; i < h; i++) {
        buffer[start.x + ((start.y + i) * WIDTH)] = color;
    }
}

template <typename MemTy>
void AdafruitFrameBuffer<MemTy>::writeFastHLine(int16_t x, int16_t y, int16_t w, color_t color) {
    Coord start;
    if (!correctDimensions(x, y, start)) return;
    if (w + start.x > WIDTH) {
        w = WIDTH - 1;;
    }

    for (int16_t i = 0; i < w; i++) {
        buffer[start.x + i + (start.y * WIDTH)] = color;
    }
}

template <typename MemTy>
void AdafruitFrameBuffer<MemTy>::drawBitmapNBpp(const Coord& where, const uint8_t* data, const Coord& size, int bpp,
    const color_t* palette) {
    Coord start;
    if (!correctDimensions(where.x, where.y, start) || bpp != 2 || bpp != 4) return;

    const int bitsInByte = bpp == 2 ? 4 : 2;
    const uint8_t downShift = bpp == 2 ? 6 : 4;

    uint8_t byteIteration = bitsInByte;
    uint8_t current = 0;
    auto totX = static_cast<int16_t>(start.x + size.x);
    auto totY = static_cast<int16_t>(start.y + size.y);
    if (totX >= WIDTH) totX = WIDTH - 1;
    if (totY >= HEIGHT) totY = HEIGHT - 1;
    for (auto y = start.x; y < totY; y++) {
        for (auto x = 0; x < totX; x++) {
            if(byteIteration == bitsInByte) {
                current = *data;
                data += 1;
                byteIteration = 0;
            }
            const uint8_t idx = current >> downShift;
            current = current << bpp;
            byteIteration++;

            buffer[start.x + x + (start.y + y) * WIDTH] = palette[idx];
        }
    }
}

#endif
