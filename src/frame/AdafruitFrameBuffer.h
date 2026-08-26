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
    if (data == nullptr || palette == nullptr) return;
    if (!(bpp == 2 || bpp == 4)) return;
    if (size.x <= 0 || size.y <= 0) return;

    const int pixelsPerByte = 8 / bpp;
    const int rowBytes = ((size.x * bpp) + 7) / 8;

    for (int16_t srcY = 0; srcY < size.y; ++srcY) {
        const int16_t dstY = static_cast<int16_t>(where.y + srcY);
        if (dstY < 0 || dstY >= HEIGHT) continue;

        const uint8_t* rowData = data + (srcY * rowBytes);

        for (int16_t srcX = 0; srcX < size.x; ++srcX) {
            const int16_t dstX = static_cast<int16_t>(where.x + srcX);
            if (dstX < 0 || dstX >= WIDTH) continue;

            const uint8_t packedByte = rowData[srcX / pixelsPerByte];
            const uint8_t shift = static_cast<uint8_t>(8 - bpp - ((srcX % pixelsPerByte) * bpp));
            const uint8_t mask = static_cast<uint8_t>((1U << bpp) - 1U);
            const uint8_t idx = static_cast<uint8_t>((packedByte >> shift) & mask);

            drawPixel(dstX, dstY, palette[idx]);
        }
    }
}

#endif
