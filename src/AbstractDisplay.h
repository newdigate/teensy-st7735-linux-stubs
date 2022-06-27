/***************************************************
  This is a library for the Adafruit 1.8" SPI display.
  This library works with the Adafruit 1.8" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/358
  as well as Adafruit raw 1.8" TFT display
  ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#ifndef __ST7735_t3_ABSTRACTDISPLAY_H_
#define __ST7735_t3_ABSTRACTDISPLAY_H_

#include "Arduino.h"
#include <cmath>
#include "ILI9341_fonts.h"

// Map fonts that were modified back to the ILI9341 font
#define ST7735_t3_font_t ILI9341_t3_font_t

/// Font data stored PER GLYPH
typedef struct {
    uint16_t bitmapOffset;     ///< Pointer into GFXfont->bitmap
    uint8_t  width;            ///< Bitmap dimensions in pixels
    uint8_t  height;           ///< Bitmap dimensions in pixels
    uint8_t  xAdvance;         ///< Distance to advance cursor (x axis)
    int8_t   xOffset;          ///< X dist from cursor pos to UL corner
    int8_t   yOffset;          ///< Y dist from cursor pos to UL corner
} GFXglyph;

/// Data stored for FONT AS A WHOLE
typedef struct {
    uint8_t  *bitmap;      ///< Glyph bitmaps, concatenated
    GFXglyph *glyph;       ///< Glyph array
    uint8_t   first;       ///< ASCII extents (first char)
    uint8_t   last;        ///< ASCII extents (last char)
    uint8_t   yAdvance;    ///< Newline distance (y axis)
} GFXfont;

#ifndef st7735_swap
#define st7735_swap(a, b) { typeof(a) t = a; a = b; b = t; }
#endif

#ifdef __cplusplus

#define ST7735_min(a, b) ((a < b)? a : b )
#define ST7735_max(a, b) ((a > b)? a : b )

#define TL_DATUM 0 // Top left (default)
#define TC_DATUM 1 // Top centre
#define TR_DATUM 2 // Top right
#define ML_DATUM 3 // Middle left
#define CL_DATUM 3 // Centre left, same as above
#define MC_DATUM 4 // Middle centre
#define CC_DATUM 4 // Centre centre, same as above
#define MR_DATUM 5 // Middle right
#define CR_DATUM 5 // Centre right, same as above
#define BL_DATUM 6 // Bottom left
#define BC_DATUM 7 // Bottom centre
#define BR_DATUM 8 // Bottom right

class AbstractDisplay : public Print {
public:
    AbstractDisplay() : Print() {
    }
    virtual ~AbstractDisplay() {

    }
    virtual void
            fillScreen(uint16_t color),
            drawPixel(int16_t x, int16_t y, uint16_t color),
            drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
            drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
            fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

    inline void fillWindow(uint16_t color) {fillScreen(color);}
    virtual void setRotation(uint8_t r);
    void     invertDisplay(bool i);
    void     setRowColStart(uint16_t x, uint16_t y);
    uint16_t  rowStart() {return _rowstart;}
    uint16_t  colStart() {return _colstart;}

    // from Adafruit_GFX.h
    int16_t width(void) const { return _width; };
    int16_t height(void) const { return _height; }
    uint8_t getRotation(void);

    void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
    void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
    void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
    void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
    void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
    void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
    virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);
    void inline drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size) { drawChar(x, y, c, color, bg, size, size);}

    static const int16_t CENTER = 9998;
    void setCursor(int16_t x, int16_t y, bool autoCenter=false);
    void getCursor(int16_t *x, int16_t *y);
    void setTextColor(uint16_t c);
    void setTextColor(uint16_t c, uint16_t bg);
    void setTextSize(uint8_t sx, uint8_t sy);
    void inline setTextSize(uint8_t s) { setTextSize(s,s); }
    uint8_t getTextSizeX();
    uint8_t getTextSizeY();
    uint8_t getTextSize();
    void setTextWrap(bool w);
    bool getTextWrap();

    int16_t getCursorX(void) const { return cursor_x; }
    int16_t getCursorY(void) const { return cursor_y; }
    void setFont(const ILI9341_t3_font_t &f);
    void setFont(const GFXfont *f = NULL);
    void setFontAdafruit(void) { setFont(); }
    void drawFontChar(unsigned int c);
    void drawGFXFontChar(unsigned int c);

    void getTextBounds(const uint8_t *buffer, uint16_t len, int16_t x, int16_t y,
                       int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
    void getTextBounds(const char *string, int16_t x, int16_t y,
                       int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
    void getTextBounds(const String &str, int16_t x, int16_t y,
                       int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
    int16_t strPixelLen(const char * str);

    // added support for drawing strings/numbers/floats with centering
    // modified from tft_ili9341_ESP github library
    // Handle numbers
    int16_t  drawNumber(long long_num,int poX, int poY);
    int16_t  drawFloat(float floatNumber,int decimal,int poX, int poY);
    // Handle char arrays
    int16_t drawString(const String& string, int poX, int poY);
    int16_t drawString1(char string[], int16_t len, int poX, int poY);

    void setTextDatum(uint8_t datum);

    // added support for scrolling text area
    // https://github.com/vitormhenrique/ILI9341_t3
    // Discussion regarding this optimized version:
    //http://forum.pjrc.com/threads/26305-Highly-optimized-ILI9341-%28320x240-TFT-color-display%29-library
    //
    void setScrollTextArea(int16_t x, int16_t y, int16_t w, int16_t h);
    void setScrollBackgroundColor(uint16_t color);
    void enableScroll(void);
    void disableScroll(void);
    void scrollTextArea(uint8_t scrollSize);
    void resetScrollBackgroundColor(uint16_t color);
    uint16_t readPixel(int16_t x, int16_t y);
    virtual void readRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *pcolors);

    // setOrigin sets an offset in display pixels where drawing to (0,0) will appear
    // for example: setOrigin(10,10); drawPixel(5,5); will cause a pixel to be drawn at hardware pixel (15,15)
    void setOrigin(int16_t x = 0, int16_t y = 0) {
        _originx = x; _originy = y;
        //if (Serial) Serial.printf("Set Origin %d %d\n", x, y);
        updateDisplayClip();
    }
    void getOrigin(int16_t* x, int16_t* y) { *x = _originx; *y = _originy; }

    // setClipRect() sets a clipping rectangle (relative to any set origin) for drawing to be limited to.
    // Drawing is also restricted to the bounds of the display

    void setClipRect(int16_t x1, int16_t y1, int16_t w, int16_t h)
    { _clipx1 = x1; _clipy1 = y1; _clipx2 = x1+w; _clipy2 = y1+h;
        //if (Serial) Serial.printf("Set clip Rect %d %d %d %d\n", x1, y1, w, h);
        updateDisplayClip();
    }
    void setClipRect() {
        _clipx1 = 0; _clipy1 = 0; _clipx2 = _width; _clipy2 = _height;
        //if (Serial) Serial.printf("clear clip Rect\n");
        updateDisplayClip();
    }
    // Useful methods added from ili9341_t3
    virtual void writeRect(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *pcolors);

    // added support to use optional Frame buffer
    void  setFrameBuffer(uint16_t *frame_buffer) {return;}
    uint8_t useFrameBuffer(bool b) { _useFramebuffer = b; return 0;};    // use the frame buffer?  First call will allocate
    void  freeFrameBuffer(void) {return;}      // explicit call to release the buffer
    virtual void  updateScreen(void) { }       // call to say update the screen now.
    bool  updateScreenAsync(bool update_cont = false) { _update_cont = update_cont; return _update_cont; }  // call to say update the screen optinoally turn into continuous mode.
    void  waitUpdateAsyncComplete(void) {return;}
    void  endUpdateAsync() {return;}      // Turn of the continueous mode fla
    void  dumpDMASettings() {return;}

    void charBounds(char c, int16_t *x, int16_t *y,
                    int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy);
    int16_t _width, _height;
    
    bool gfxFontLastCharPosFG(int16_t x, int16_t y);

    void drawFontBits(bool opaque, uint32_t bits, uint32_t numbits, int32_t x, int32_t y, uint32_t repeat);
    void drawFontPixel( uint8_t alpha, uint32_t x, uint32_t y );
    uint32_t fetchpixel(const uint8_t *p, uint32_t index, uint32_t x);

    virtual void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    virtual void pushColor(uint16_t color, bool last_pixel=false);
    
    void setAddr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1){}
    inline uint16_t Color565(uint8_t r, uint8_t g, uint8_t b) {
        return ((b & 0xF8) << 8) | ((g & 0xFC) << 3) | (r >> 3);
    }
protected:
    // Pass 8-bit (each) R,G,B, get back 16-bit packed color

    uint16_t _colstart, _rowstart, _xstart, _ystart, _rot, _screenHeight, _screenWidth;

    bool _useFramebuffer = false;
    uint8_t  tabcolor;
    bool _update_cont = false;
    uint16_t  _windowx0 = 0,
              _windowy0 = 0,
              _windowx1 = 128,
              _windowy1 = 128;
    bool  hwSPI;
    ////
    int16_t  cursor_x, cursor_y;
    bool 	 _center_x_text = false;
    bool 	 _center_y_text = false;
    int16_t  _clipx1, _clipy1, _clipx2, _clipy2;
    int16_t  _originx, _originy;
    int16_t  _displayclipx1, _displayclipy1, _displayclipx2, _displayclipy2;
    bool _invisible = false;
    bool _standard = true; // no bounding rectangle or origin set.

    inline void updateDisplayClip() {
        _displayclipx1 = ST7735_max(0,ST7735_min(_clipx1+_originx, width()));
        _displayclipx2 = ST7735_max(0,ST7735_min(_clipx2+_originx, width()));

        _displayclipy1 = ST7735_max(0,ST7735_min(_clipy1+_originy, height()));
        _displayclipy2 = ST7735_max(0,ST7735_min(_clipy2+_originy, height()));
        _invisible = (_displayclipx1 == _displayclipx2 || _displayclipy1 == _displayclipy2);
        _standard =  (_displayclipx1 == 0) && (_displayclipx2 == _width) && (_displayclipy1 == 0) && (_displayclipy2 == _height);
    }

    int16_t scroll_x, scroll_y, scroll_width, scroll_height;
    bool scrollEnable,isWritingScrollArea; // If set, 'wrap' text at right edge of display

    uint16_t textcolor, textbgcolor, scrollbgcolor;
    uint32_t textcolorPrexpanded, textbgcolorPrexpanded;
    uint8_t textsize_x, textsize_y, rotation, textdatum;
    bool wrap; // If set, 'wrap' text at right edge of display
    const ILI9341_t3_font_t *font;
    // Anti-aliased font support
    uint8_t fontbpp = 1;
    uint8_t fontbppindex = 0;
    uint8_t fontbppmask = 1;
    uint8_t fontppb = 8;
    uint8_t* fontalphalut;
    float fontalphamx = 1;

    uint32_t padX;

    // GFX Font support
    const GFXfont *gfxFont = nullptr;
    int8_t _gfxFont_min_yOffset = 0;

    // Opaque font chracter overlap?
    unsigned int _gfx_c_last;
    int16_t   _gfx_last_cursor_x, _gfx_last_cursor_y;
    int16_t	 _gfx_last_char_x_write = 0;
    uint16_t _gfx_last_char_textcolor;
    uint16_t _gfx_last_char_textbgcolor;

    virtual void Pixel(int16_t x, int16_t y, uint16_t color) {

    };

    virtual void HLine(int16_t x, int16_t y, int16_t w, uint16_t color)
    {
        x+=_originx;
        y+=_originy;

        // Rectangular clipping
        if((y < _displayclipy1) || (x >= _displayclipx2) || (y >= _displayclipy2)) return;
        if(x<_displayclipx1) { w = w - (_displayclipx1 - x); x = _displayclipx1; }
        if((x+w-1) >= _displayclipx2)  w = _displayclipx2-x;
        if (w<1) return;

        for (int i = 0; i < w; ++i) {
            drawPixel(x + i, y, color);
        }
    }

    virtual void VLine(int16_t x, int16_t y, int16_t h, uint16_t color)
    {
        x+=_originx;
        y+=_originy;

        // Rectangular clipping
        if((x < _displayclipx1) || (x >= _displayclipx2) || (y >= _displayclipy2)) return;
        if(y < _displayclipy1) { h = h - (_displayclipy1 - y); y = _displayclipy1;}
        if((y+h-1) >= _displayclipy2) h = _displayclipy2-y;
        if(h<1) return;

        for (int i = 0; i < h; ++i) {
            drawPixel(x, y+i, color);
        }

    }

    /**
     * Found in a pull request for the Adafruit framebuffer library. Clever!
     * https://github.com/tricorderproject/arducordermini/pull/1/files#diff-d22a481ade4dbb4e41acc4d7c77f683d
     * Converts  0000000000000000rrrrrggggggbbbbb
     *     into  00000gggggg00000rrrrr000000bbbbb
     * with mask 00000111111000001111100000011111
     * This is useful because it makes space for a parallel fixed-point multiply
     * This implements the linear interpolation formula: result = bg * (1.0 - alpha) + fg * alpha
     * This can be factorized into: result = bg + (fg - bg) * alpha
     * alpha is in Q1.5 format, so 0.0 is represented by 0, and 1.0 is represented by 32
     * @param	fg		Color to draw in RGB565 (16bit)
     * @param	bg		Color to draw over in RGB565 (16bit)
     * @param	alpha	Alpha in range 0-255
     **/
    uint16_t alphaBlendRGB565( uint32_t fg, uint32_t bg, uint8_t alpha )
    {
        alpha = ( alpha + 4 ) >> 3; // from 0-255 to 0-31
        bg = (bg | (bg << 16)) & 0b00000111111000001111100000011111;
        fg = (fg | (fg << 16)) & 0b00000111111000001111100000011111;
        uint32_t result = ((((fg - bg) * alpha) >> 5) + bg) & 0b00000111111000001111100000011111;
        return (uint16_t)((result >> 16) | result); // contract result
    }
    /**
     * Same as above, but fg and bg are premultiplied, and alpah is already in range 0-31
     */
    uint16_t alphaBlendRGB565Premultiplied( uint32_t fg, uint32_t bg, uint8_t alpha )
    {
        uint32_t result = ((((fg - bg) * alpha) >> 5) + bg) & 0b00000111111000001111100000011111;
        return (uint16_t)((result >> 16) | result); // contract result
    }

    virtual void writedata16(uint16_t d) {
    }

    virtual void writedata16_last(uint16_t d) {

    }

};
#endif	 //end cplus

#endif