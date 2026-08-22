/***********************************
This is a our graphics core library, for all our displays. 
We'll be adapting all the
existing libaries to use this core to make updating, support 
and upgrading easier!

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above must be included in any redistribution
****************************************/

/*
 *  Modified by Neal Horman 7/14/2012 for use in mbed
 */

#ifndef ADAFRUIT_GFX_H
#define ADAFRUIT_GFX_H

#include "Adafruit_GFX_Config.h"
#include "tcUnicodeHelper.h"
#include <PrintCompat.h>

#ifndef color_t
#ifdef NEED_32BIT_COLOR_T_ALPHA
typedef uint32_t color_t;
#else
typedef uint16_t color_t;
#endif
#endif

#ifndef pgm_read_byte
#define pgm_read_byte(x) (*x)
#endif //pgm_read_byte

static inline void swap(int16_t &a, int16_t &b) {
    int16_t t = a;

    a = b;
    b = t;
}

#ifndef _BV
#define _BV(bit) (1<<(bit))
#endif

#define BLACK 0
#define WHITE 1

/// A generic graphics superclass that can handle all sorts of drawing. At a
/// minimum you can subclass and provide drawPixel(). At a maximum you can do a
/// ton of overriding to optimize. Used for any/all Adafruit displays!

class Adafruit_GFX : public Print {
public:
    Adafruit_GFX(int16_t w, int16_t h, UnicodeEncodingMode encodingMode); // Constructor
    Adafruit_GFX(int16_t w, int16_t h, UnicodeFontHandler* fontHandler); // Constructor

    /**********************************************************************/
    /*!
      @brief  Draw to the screen/framebuffer/etc.
      Must be overridden in subclass.
      @param  x    X coordinate in pixels
      @param  y    Y coordinate in pixels
      @param color  16-bit pixel color.
    */
    /**********************************************************************/
    virtual void drawPixel(int16_t x, int16_t y, color_t color) = 0;

    void setTextSize(int sz, int sz1=0) {}

    // TRANSACTION API / CORE DRAW API
    // These MAY be overridden by the subclass to provide device-specific
    // optimized code.  Otherwise 'generic' versions are used.
    virtual void startWrite();

    virtual void writePixel(int16_t x, int16_t y, color_t color);

    virtual void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                               color_t color);

    virtual void writeFastVLine(int16_t x, int16_t y, int16_t h, color_t color);

    virtual void writeFastHLine(int16_t x, int16_t y, int16_t w, color_t color);

    virtual void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                           color_t color);

    virtual void endWrite();

    // CONTROL API
    // These MAY be overridden by the subclass to provide device-specific
    // optimized code.  Otherwise 'generic' versions are used.
    virtual void setRotation(uint8_t r);

    virtual void invertDisplay(bool i);

    // BASIC DRAW API
    // These MAY be overridden by the subclass to provide device-specific
    // optimized code.  Otherwise 'generic' versions are used.

    // It's good to implement those, even if using transaction API
    virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, color_t color);

    virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, color_t color);

    virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                          color_t color);

    virtual void fillScreen(color_t color);

    // Optional and probably not necessary to change
    virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                          color_t color);

    virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h,
                          color_t color);

    // These exist only with Adafruit_GFX (no subclass overrides)
    void drawCircle(int16_t x0, int16_t y0, int16_t r, color_t color);

    void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
                          color_t color);

    void fillCircle(int16_t x0, int16_t y0, int16_t r, color_t color);

    void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
                          int16_t delta, color_t color);

    void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2,
                      int16_t y2, color_t color);

    void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2,
                      int16_t y2, color_t color);

    void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
                       int16_t radius, color_t color);

    void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
                       int16_t radius, color_t color);

    void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w,
                    int16_t h, color_t color);

    void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w,
                    int16_t h, color_t color, color_t bg);

    void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h,
                    color_t color);

    void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h,
                    color_t color, color_t bg);

    void drawXBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w,
                     int16_t h, color_t color);

    void drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[],
                             int16_t w, int16_t h);

    void drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w,
                             int16_t h);

    void drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[],
                             const uint8_t mask[], int16_t w, int16_t h);

    void drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap, uint8_t *mask,
                             int16_t w, int16_t h);

    void drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w,
                       int16_t h);

    void drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w,
                       int16_t h);

    void drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[],
                       const uint8_t mask[], int16_t w, int16_t h);

    void drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, uint8_t *mask,
                       int16_t w, int16_t h);

    UnicodeFontHandler* getFontHandler() { return fontHandler; }

    void getTextBounds(const char *str, int16_t x, int16_t y, int16_t *x1,
                       int16_t *y1, uint16_t *w, uint16_t *h) {
        int baseline;
        Coord coord = fontHandler->textExtents(str, &baseline, false);
        *x1 = x;
        *y1 = y;
        *w = coord.x;
        *h = coord.y;
    }

    Coord getTextExtents(const char* data, int* baseline = nullptr) {
        return fontHandler->textExtents(data, baseline, false);
    }

    void setFont(const GFXfont *f = nullptr) {
        fontHandler->setFont(f);
    }
    void setFont(const UnicodeFont *f) {
        fontHandler->setFont(f);
    }

    void setTextWrap(bool wrap) {}

    /**********************************************************************/
    /*!
      @brief  Set text cursor location
      @param  x    X coordinate in pixels
      @param  y    Y coordinate in pixels
    */
    /**********************************************************************/
    void setCursor(int16_t x, int16_t y) {
        cursorX = x;
        cursorY = y;
    }

    /**********************************************************************/
    /*!
      @brief   Set text font color with transparant background
      @param   c   16-bit 5-6-5 Color to draw text with
      @note    For 'transparent' background, background and foreground
               are set to same color rather than using a separate flag.
    */
    /**********************************************************************/
    void setTextColor(color_t c) {
        fontHandler->setDrawColor(c);
    }

    void setTextColor(color_t c, color_t bg) {
        fontHandler->setDrawColor(c);
    }

    /**********************************************************************/
    /*!
      @brief  Enable (or disable) Code Page 437-compatible charset.
              There was an error in glcdfont.c for the longest time -- one
              character (#176, the 'light shade' block) was missing -- this
              threw off the index of every character that followed it.
              But a TON of code has been written with the erroneous
              character indices. By default, the library uses the original
              'wrong' behavior and old sketches will still work. Pass
              'true' to this function to use correct CP437 character values
              in your code.
      @param  x  true = enable (new behavior), false = disable (old behavior)
    */
    /**********************************************************************/

    using Print::write;

    virtual size_t write(uint8_t);

    /************************************************************************/
    /*!
      @brief      Get width of the display, accounting for current rotation
      @returns    Width in pixels
    */
    /************************************************************************/
    int16_t width() const { return _width; };

    /************************************************************************/
    /*!
      @brief      Get height of the display, accounting for current rotation
      @returns    Height in pixels
    */
    /************************************************************************/
    int16_t height() const { return _height; }

    /************************************************************************/
    /*!
      @brief      Get rotation setting for display
      @returns    0 thru 3 corresponding to 4 cardinal rotations
    */
    /************************************************************************/
    uint8_t getRotation() const { return rotation; }

    int16_t getCursorX() const { return cursorX; }
    int16_t getCursorY() const { return cursorY; }

protected:
    void charBounds(unsigned char c, int16_t *x, int16_t *y, int16_t *minx,
                    int16_t *miny, int16_t *maxx, int16_t *maxy);

    UnicodeFontHandler* fontHandler;
    int16_t WIDTH;        ///< This is the 'raw' display width - never changes
    int16_t HEIGHT;       ///< This is the 'raw' display height - never changes
    int16_t _width;       ///< Display width as modified by current rotation
    int16_t _height;      ///< Display height as modified by current rotation
    uint8_t rotation;     ///< Display rotation (0 thru 3)
    int16_t cursorX=0;
    int16_t cursorY=0;
};

/// A GFX 1-bit canvas context for graphics
class GFXcanvas1 : public Adafruit_GFX {
public:
    GFXcanvas1(uint16_t w, uint16_t h, UnicodeFontHandler* fh);

    ~GFXcanvas1(void);

    void drawPixel(int16_t x, int16_t y, color_t color) override;

    void fillScreen(color_t color) override;

    void drawFastVLine(int16_t x, int16_t y, int16_t h, color_t color) override;

    void drawFastHLine(int16_t x, int16_t y, int16_t w, color_t color) override;

    bool getPixel(int16_t x, int16_t y) const;
    /**********************************************************************/
    /*!
      @brief    Get a pointer to the internal buffer memory
      @returns  A pointer to the allocated buffer
    */
    /**********************************************************************/
    uint8_t *getBuffer() const { return buffer; }

protected:
    bool getRawPixel(int16_t x, int16_t y) const;

    void drawFastRawVLine(int16_t x, int16_t y, int16_t h, color_t color);

    void drawFastRawHLine(int16_t x, int16_t y, int16_t w, color_t color);

private:
    uint8_t *buffer;
};

#endif // ADAFRUIT_GFX_H

