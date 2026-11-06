#ifndef TC_ADAFRUIT_SSD1107_H
#define TC_ADAFRUIT_SSD1107_H

#include "Adafruit_GFX.h"
#include "PlatformDetermination.h"
#include "Utf8TextProcessor.h"

// ============================================================================
// Sino Wealth SH1107 Command / Register Definitions
// ============================================================================

// --- Column Address Setting ---
// Set lower 4 bits of column start address: 0x00 - 0x0F
#define SH1107_SET_COL_ADDR_LSB_BASE        0x00
// Set higher 4 bits of column start address: 0x10 - 0x17 (valid up to 128 cols)
#define SH1107_SET_COL_ADDR_MSB_BASE        0x10

// --- Addressing Mode ---
// Set Memory Addressing Mode (Double-byte command: 0x20 followed by mode)
#define SH1107_SET_MEMORY_ADDRESSING_MODE   0x20
#define SH1107_ADDRESSING_PAGE              0x00 // Page addressing mode
#define SH1107_ADDRESSING_VERTICAL          0x01 // Vertical addressing mode

// --- Contrast Control ---
// Set Contrast Control (Double-byte command: 0x81 followed by 0x00-0xFF)
#define SH1107_SET_CONTRAST                 0x81

// --- Segment Remap ---
#define SH1107_SET_SEG_NORMAL               0xA0 // Column 0 is mapped to SEG0
#define SH1107_SET_SEG_REVERSE              0xA1 // Column 127 is mapped to SEG0

// --- Multiplex Ratio ---
// Set Multiplex Ratio (Double-byte command: 0xA8 followed by 0x00-0x7F for 1-128 MUX)
#define SH1107_SET_MULTIPLEX_RATIO          0xA8

// --- Entire Display ON (Resume / Force) ---
#define SH1107_DISPLAY_ENTIRE_OFF           0xA4 // Resume to RAM content
#define SH1107_DISPLAY_ENTIRE_ON            0xA5 // Entire display ON (ignores RAM)

// --- Normal / Inverse Display ---
#define SH1107_DISPLAY_NORMAL               0xA6 // Normal display (1 in RAM = pixel ON)
#define SH1107_DISPLAY_INVERSE              0xA7 // Inverse display (0 in RAM = pixel ON)

// --- Display Offset ---
// Set Display Start Line / Offset (Double-byte command: 0xD3 followed by 0x00-0x7F)
#define SH1107_SET_DISPLAY_OFFSET           0xD3

// --- DC-DC Control ---
// Set DC-DC Converter (Double-byte command: 0xAD followed by sub-command)
#define SH1107_SET_DCDC_CONVERTER           0xAD
#define SH1107_DCDC_OFF                     0x8A // Built-in DC-DC OFF
#define SH1107_DCDC_ON                      0x8B // Built-in DC-DC ON

// --- Display ON / OFF ---
#define SH1107_DISPLAY_OFF                  0xAE // Display sleep mode (OFF)
#define SH1107_DISPLAY_ON                   0xAF // Display normal mode (ON)

// --- Page Address Setting ---
// Set Page Address (0xB0 - 0xBF: Page 0 to Page 15)
#define SH1107_SET_PAGE_ADDR_BASE           0xB0

// --- Common Output Scan Direction ---
#define SH1107_SET_COM_NORMAL               0xC0 // Scan from COM0 to COM[N-1]
#define SH1107_SET_COM_REVERSE              0xC8 // Scan from COM[N-1] to COM0

// --- Clock Divide Ratio & Oscillator Frequency ---
// Set Display Clock Divide / Osc Freq (Double-byte command: 0xD5 followed by 0x00-0xFF)
// [7:4] = Osc Freq (0-15), [3:0] = Divide ratio (0-15)
#define SH1107_SET_DISPLAY_CLOCK_DIV        0xD5

// --- Dis-charge / Pre-charge Period ---
// Set Pre-charge Period (Double-byte command: 0xD9 followed by 0x00-0xFF)
// [7:4] = Dis-charge period (1-15 DCLK), [3:0] = Pre-charge period (1-15 DCLK)
#define SH1107_SET_PRECHARGE_PERIOD         0xD9

// --- VCOMH Deselect Level ---
// Set VCOM Deselect Level (Double-byte command: 0xDB followed by 0x00-0xFF)
#define SH1107_SET_VCOM_DESELECT            0xDB

// --- Display Start Line (RAM line) ---
// Set Display Start Line: 0xDC followed by line number 0x00-0x7F
#define SH1107_SET_DISPLAY_START_LINE       0xDC

// --- Read-Modify-Write Mode ---
#define SH1107_START_READ_MODIFY_WRITE      0xE0
#define SH1107_END_READ_MODIFY_WRITE        0xEE

// --- NOP ---
#define SH1107_NOP                          0xE3

// ============================================================================
// Helper Macros
// ============================================================================
#define SH1107_SET_COL_ADDR_LSB(col)        (SH1107_SET_COL_ADDR_LSB_BASE | ((col) & 0x0F))
#define SH1107_SET_COL_ADDR_MSB(col)        (SH1107_SET_COL_ADDR_MSB_BASE | (((col) >> 4) & 0x07))
#define SH1107_SET_PAGE_ADDR(page)          (SH1107_SET_PAGE_ADDR_BASE    | ((page) & 0x0F))

/**
 * This class implements the SH1107 OLED display driver. It is a clean implementation of the SH1107 display controller
 * that is fully compatible with our fork of the Adafruit GFX class. It implements both fast line functions and pixel
 * drawing, so should perform well. Never use this class directly as it requires command and data functions, use the SPI
 * or I2C variants instead.
 *
 * Should your display require an offset to function properly, use the setOffsets(..) function to configure it before
 * first drawing.
 */
class TcAdaSH1107 : public Adafruit_GFX {
private:
    // the memory buffer for the LCD
    uint8_t* buffer = nullptr;
    size_t buffSize = 0;
    uint8_t rowOffset = 0;
    uint8_t colOffset = 0;

    void drawFastRawVLine(int16_t x, int16_t y, int16_t h, color_t color);
    void drawFastRawHLine(int16_t x, int16_t y, int16_t w, color_t color);

public:
    /**
     * Create a display with a custom height, for this display, the width is always 128. If you have an earlier display
     * that needs an offset to be applied, use the setOffsets() method.
     * @param RST the reset pin or -1 if not used
     * @param rawHeight the height of the display
     * @param encodingMode the encoding mode to use (default UTF8)
     */
    TcAdaSH1107(pinid_t RST, int16_t rawHeight, tccore::UnicodeEncodingMode encodingMode = ENCMODE_UTF8);

    /**
    * Create a display with a default width and height of 128x128. If you have an earlier display
     * that needs an offset to be applied, use the setOffsets() method.
     * @param RST the reset pin or -1 if not used
     * @param encodingMode the encoding mode to use (default UTF8)
     */
    TcAdaSH1107(pinid_t RST, tccore::UnicodeEncodingMode encodingMode);

    ~TcAdaSH1107() override;

    /**
     * Some display drivers require an offset to be set to properly display the data. For example earlier models of
     * the display used a horizontal offset of 96, and 64 line displays sometimes require a vertical offset of 64.
     * Most modern displays do not require an offset.
     * @param rowOffs the vertical offset
     * @param colOffs the horizontal offset
     */
    void setOffsets(uint8_t rowOffs, uint8_t colOffs) {
        this->rowOffset = rowOffs;
        this->colOffset = colOffs;
    }

    /**
     * Actually start the display so it can present data, going through the initialization sequence.
     */
    void begin();

    void drawPixel(int16_t x, int16_t y, color_t color) override;

    void writeFastVLine(int16_t x, int16_t y, int16_t h, color_t color) override;
    void writeFastHLine(int16_t x, int16_t y, int16_t w, color_t color) override;
    void drawFastVLine(int16_t x, int16_t y, int16_t h, color_t color) override { writeFastVLine(x, y, h, color); }
    void drawFastHLine(int16_t x, int16_t y, int16_t w, color_t color) override { writeFastHLine(x, y, w, color); }

    /**
     * Send the display buffer out to the display. Required to "commit" what has been drawn to the display.
     */
    void display();

    /**
     * Clear the display buffer. Requires a display() call at some point afterwards. Sets all pixels to black (0).
     */
    void clearDisplay() {
        if (buffer) {
            memset(buffer, 0, buffSize);
        }
    }

    /**
     * Implemented in specialisations for I2C or SPI to send a command to the device
     * @param c the command to send
     */
    virtual void command(uint8_t c) = 0;

    /**
     * Implemented in specialisations for I2C or SPI to send data to the device
     * @param c the data to send
     * @param dataSize the size of the data to send
     */
    virtual void data(uint8_t* c, size_t dataSize) = 0;
};

#endif
