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

#include "ST7735_t3.h"
#include <limits.h>
#include <cstdint>
#include "cstdlib"
#include <stdlib.h>
#include <stdio.h>
// Constructor when using software SPI.  All output pins are configurable.
ST7735_t3::ST7735_t3(uint8_t cs, uint8_t rs, uint8_t sid, uint8_t sclk, uint8_t rst)
{
    _rot = 0xff;
    _screenHeight = ST7735_TFTHEIGHT_160;
    _screenWidth = ST7735_TFTWIDTH;

    _width = _screenWidth;
    _height = _screenHeight;

    cursor_y  = cursor_x    = 0;
    textsize_x  = 1;
    textsize_y  = 1;
    textcolor = textbgcolor = 0xFFFF;
    wrap      = true;
//    font      = NULL;
    gfxFont   = NULL;
    setClipRect();
    setOrigin();
}

// Constructor when using hardware SPI.  Faster, but must use SPI pins
// specific to each board type (e.g. 11,13 for Uno, 51,52 for Mega, etc.)
ST7735_t3::ST7735_t3(uint8_t cs, uint8_t rs, uint8_t rst)
{
    _rot = 0xff;
    _screenHeight = ST7735_TFTHEIGHT_160;
    _screenWidth = ST7735_TFTWIDTH;

    cursor_y  = cursor_x    = 0;
    textsize_x  = 1;
    textsize_y  = 1;
    textcolor = textbgcolor = 0xFFFF;
    wrap      = true;
    //font      = NULL;
    gfxFont   = NULL;
    setClipRect();
    setOrigin();
}

// Initialization for ST7735B screens
void ST7735_t3::initB(void)
{
}

// Initialization for ST7735R screens (green or red tabs)
void ST7735_t3::initR(uint8_t options)
{
    if (options == INITR_GREENTAB) {
        _colstart = 2;
        _rowstart = 1;
    } else if(options == INITR_144GREENTAB) {
        _screenHeight = ST7735_TFTHEIGHT_144;
        _colstart = 2;
        _rowstart = 3;
    } else if(options == INITR_144GREENTAB_OFFSET) {
        _screenHeight = ST7735_TFTHEIGHT_144;
        _colstart = 0;
        _rowstart = 32;
    } else if(options == INITR_MINI160x80) {
        _screenHeight   = ST7735_TFTHEIGHT_160;
        _screenWidth    = ST7735_TFTWIDTH_80;
        _colstart = 24;
        _rowstart = 0;
    } else if (options == INITR_MINI160x80_ST7735S) {
        _screenHeight   = 160;
        _screenWidth    = 80;
        _colstart = 26;
        _rowstart = 1;
    } else {
        // _colstart, _rowstart left at default '0' values
    }

    // if black or mini, change MADCTL color filter
    if ((options == INITR_BLACKTAB)  || (options == INITR_MINI160x80)){
    }

    tabcolor = options;
    setRotation(0);
}

void ST7735_t3::pushColor(uint16_t color, bool last_pixel)
{
    // TODO:
}

//#include "glcdfont.c"
extern "C" const unsigned char glcdfont[];

void ST7735_t3::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    x += _originx;
    y += _originy;

    if((x < _displayclipx1) ||(x >= _displayclipx2) || (y < _displayclipy1) || (y >= _displayclipy2)) return;

}


void ST7735_t3::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
    x+=_originx;
    y+=_originy;
    // Rectangular clipping
    if((x < _displayclipx1) || (x >= _displayclipx2) || (y >= _displayclipy2)) return;
    if(y < _displayclipy1) { h = h - (_displayclipy1 - y); y = _displayclipy1;}
    if((y+h-1) >= _displayclipy2) h = _displayclipy2-y;
    if(h<1) return;

    drawLine(x, y, x, y + h, color);
}

void ST7735_t3::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
    x+=_originx;
    y+=_originy;

    // Rectangular clipping
    if((y < _displayclipy1) || (x >= _displayclipx2) || (y >= _displayclipy2)) return;
    if(x<_displayclipx1) { w = w - (_displayclipx1 - x); x = _displayclipx1; }
    if((x+w-1) >= _displayclipx2)  w = _displayclipx2-x;
    if (w<1) return;
    drawLine(x, y, x + w, y, color);
}

void ST7735_t3::fillScreen(uint16_t color)
{
    fillRect(0, 0,  _width, _height, color);
}

// fill a rectangle
void ST7735_t3::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    x+=_originx;
    y+=_originy;

    // Rectangular clipping (drawChar w/big text requires this)
    if((x >= _displayclipx2) || (y >= _displayclipy2)) return;
    if (((x+w) <= _displayclipx1) || ((y+h) <= _displayclipy1)) return;
    if(x < _displayclipx1) {	w -= (_displayclipx1-x); x = _displayclipx1; 	}
    if(y < _displayclipy1) {	h -= (_displayclipy1 - y); y = _displayclipy1; 	}
    if((x + w - 1) >= _displayclipx2)  w = _displayclipx2  - x;
    if((y + h - 1) >= _displayclipy2) h = _displayclipy2 - y;


}


#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

void ST7735_t3::setRotation(uint8_t m)
{
    //Serial.printf("Setting Rotation to %d\n", m);
    rotation = m % 4; // can't be higher than 3
    _rot = rotation;	// remember the rotation...

    setClipRect();
    setOrigin();

    cursor_x = 0;
    cursor_y = 0;
}

void ST7735_t3::setRowColStart(uint16_t x, uint16_t y) {
    _rowstart = x;
    _colstart = y;
    if (_rot != 0xff) setRotation(_rot);
}


void ST7735_t3::invertDisplay(bool i)
{
}

/*!
 @brief   Adafruit_SPITFT Send Command handles complete sending of commands and const data
 @param   commandByte       The Command Byte
 @param   dataBytes         A pointer to the Data bytes to send
 @param   numDataBytes      The number of bytes we should send
 */

uint16_t ST7735_t3::readPixel(int16_t x, int16_t y)
{
    uint16_t colors = 0;
    readRect(x, y, 1, 1, &colors);
    return colors;
}


// Now lets see if we can read in multiple pixels
void ST7735_t3::readRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *pcolors)
{
    // Use our Origin.
    x+=_originx;
    y+=_originy;
    //BUGBUG:: Should add some validation of X and Y
}

// Now lets see if we can writemultiple pixels
void ST7735_t3::writeRect(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *pcolors)
{
    x+=_originx;
    y+=_originy;

    uint16_t x_clip_left = 0;  // How many entries at start of colors to skip at start of row
    uint16_t x_clip_right = 0;    // how many color entries to skip at end of row for clipping
    // Rectangular clipping

    // See if the whole thing out of bounds...
    if((x >= _displayclipx2) || (y >= _displayclipy2)) return;
    if (((x+w) <= _displayclipx1) || ((y+h) <= _displayclipy1)) return;

    // In these cases you can not do simple clipping, as we need to synchronize the colors array with the
    // We can clip the height as when we get to the last visible we don't have to go any farther.
    // also maybe starting y as we will advance the color array.
    if(y < _displayclipy1) {
        int dy = (_displayclipy1 - y);
        h -= dy;
        pcolors += (dy*w); // Advance color array to
        y = _displayclipy1;
    }
    if((y + h - 1) >= _displayclipy2) h = _displayclipy2 - y;
    // For X see how many items in color array to skip at start of row and likewise end of row
    if(x < _displayclipx1) {
        x_clip_left = _displayclipx1-x;
        w -= x_clip_left;
        x = _displayclipx1;
    }

    if((x + w - 1) >= _displayclipx2) {
        x_clip_right = w;
        w = _displayclipx2  - x;
        x_clip_right -= w;
    }
}

///
///
///
// Draw a rectangle
void ST7735_t3::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{

    drawFastHLine(x, y, w, color);
    drawFastHLine(x, y+h-1, w, color);
    drawFastVLine(x, y, h, color);
    drawFastVLine(x+w-1, y, h, color);
}

// Draw a rounded rectangle
void ST7735_t3::drawRoundRect(int16_t x, int16_t y, int16_t w,
                              int16_t h, int16_t r, uint16_t color) {
    // smarter version
    drawFastHLine(x+r  , y    , w-2*r, color); // Top
    drawFastHLine(x+r  , y+h-1, w-2*r, color); // Bottom
    drawFastVLine(x    , y+r  , h-2*r, color); // Left
    drawFastVLine(x+w-1, y+r  , h-2*r, color); // Right
    // draw four corners
    drawCircleHelper(x+r    , y+r    , r, 1, color);
    drawCircleHelper(x+w-r-1, y+r    , r, 2, color);
    drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
    drawCircleHelper(x+r    , y+h-r-1, r, 8, color);
}

// Fill a rounded rectangle
void ST7735_t3::fillRoundRect(int16_t x, int16_t y, int16_t w,
                              int16_t h, int16_t r, uint16_t color) {
    // smarter version
    fillRect(x+r, y, w-2*r, h, color);

    // draw four corners
    fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
    fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
}

// Draw a triangle
void ST7735_t3::drawTriangle(int16_t x0, int16_t y0,
                             int16_t x1, int16_t y1,
                             int16_t x2, int16_t y2, uint16_t color) {
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

// Fill a triangle
void ST7735_t3::fillTriangle ( int16_t x0, int16_t y0,
                               int16_t x1, int16_t y1,
                               int16_t x2, int16_t y2, uint16_t color) {

    int16_t a, b, y, last;

    // Sort coordinates by Y order (y2 >= y1 >= y0)
    if (y0 > y1) {
        st7735_swap(y0, y1); st7735_swap(x0, x1);
    }
    if (y1 > y2) {
        st7735_swap(y2, y1); st7735_swap(x2, x1);
    }
    if (y0 > y1) {
        st7735_swap(y0, y1); st7735_swap(x0, x1);
    }

    if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
        a = b = x0;
        if(x1 < a)      a = x1;
        else if(x1 > b) b = x1;
        if(x2 < a)      a = x2;
        else if(x2 > b) b = x2;
        drawFastHLine(a, y0, b-a+1, color);
        return;
    }

    int32_t
            dx01 = x1 - x0,
            dy01 = y1 - y0,
            dx02 = x2 - x0,
            dy02 = y2 - y0,
            dx12 = x2 - x1,
            dy12 = y2 - y1,
            sa   = 0,
            sb   = 0;

    // For upper part of triangle, find scanline crossings for segments
    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
    // is included here (and second loop will be skipped, avoiding a /0
    // error there), otherwise scanline y1 is skipped here and handled
    // in the second loop...which also avoids a /0 error here if y0=y1
    // (flat-topped triangle).
    if(y1 == y2) last = y1;   // Include y1 scanline
    else         last = y1-1; // Skip it

    for(y=y0; y<=last; y++) {
        a   = x0 + sa / dy01;
        b   = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;
        /* longhand:
        a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if(a > b) st7735_swap(a,b);
        drawFastHLine(a, y, b-a+1, color);
    }

    // For lower part of triangle, find scanline crossings for segments
    // 0-2 and 1-2.  This loop is skipped if y1=y2.
    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);
    for(; y<=y2; y++) {
        a   = x1 + sa / dy12;
        b   = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        /* longhand:
        a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
        */
        if(a > b) st7735_swap(a,b);
        drawFastHLine(a, y, b-a+1, color);
    }
}

// Draw a circle outline
void ST7735_t3::drawCircle(int16_t x0, int16_t y0, int16_t r,
                           uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    drawPixel(x0  , y0+r, color);
    drawPixel(x0  , y0-r, color);
    drawPixel(x0+r, y0  , color);
    drawPixel(x0-r, y0  , color);

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        drawPixel(x0 + x, y0 + y, color);
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 + x, y0 - y, color);
        drawPixel(x0 - x, y0 - y, color);
        drawPixel(x0 + y, y0 + x, color);
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 + y, y0 - x, color);
        drawPixel(x0 - y, y0 - x, color);
    }
}

void ST7735_t3::drawCircleHelper( int16_t x0, int16_t y0,
                                  int16_t r, uint8_t cornername, uint16_t color) {
    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;
        if (cornername & 0x4) {
            drawPixel(x0 + x, y0 + y, color);
            drawPixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2) {
            drawPixel(x0 + x, y0 - y, color);
            drawPixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) {
            drawPixel(x0 - y, y0 + x, color);
            drawPixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) {
            drawPixel(x0 - y, y0 - x, color);
            drawPixel(x0 - x, y0 - y, color);
        }
    }
}

void ST7735_t3::fillCircle(int16_t x0, int16_t y0, int16_t r,
                           uint16_t color) {
    drawFastVLine(x0, y0-r, 2*r+1, color);
    fillCircleHelper(x0, y0, r, 3, 0, color);
}

// Used to do circles and roundrects
void ST7735_t3::fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
                                 uint8_t cornername, int16_t delta, uint16_t color) {

    int16_t f     = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x     = 0;
    int16_t y     = r;

    while (x<y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;

        if (cornername & 0x1) {
            drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
            drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
        }
        if (cornername & 0x2) {
            drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
            drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
        }
    }
}

// Bresenham's algorithm - thx wikpedia
void ST7735_t3::drawLine(int16_t x0, int16_t y0,
                         int16_t x1, int16_t y1, uint16_t color)
{
    if (y0 == y1) {
        if (x1 > x0) {
            drawFastHLine(x0, y0, x1 - x0 + 1, color);
        } else if (x1 < x0) {
            drawFastHLine(x1, y0, x0 - x1 + 1, color);
        } else {
            drawPixel(x0, y0, color);
        }
        return;
    } else if (x0 == x1) {
        if (y1 > y0) {
            drawFastVLine(x0, y0, y1 - y0 + 1, color);
        } else {
            drawFastVLine(x0, y1, y0 - y1 + 1, color);
        }
        return;
    }

    bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        st7735_swap(x0, y0);
        st7735_swap(x1, y1);
    }
    if (x0 > x1) {
        st7735_swap(x0, x1);
        st7735_swap(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    int16_t xbegin = x0;
    if (steep) {
        for (; x0<=x1; x0++) {
            err -= dy;
            if (err < 0) {
                int16_t len = x0 - xbegin;
                if (len) {
                    VLine(y0, xbegin, len + 1, color);
                } else {
                    Pixel(y0, x0, color);
                }
                xbegin = x0 + 1;
                y0 += ystep;
                err += dx;
            }
        }
        if (x0 > xbegin + 1) {
            VLine(y0, xbegin, x0 - xbegin, color);
        }

    } else {
        for (; x0<=x1; x0++) {
            err -= dy;
            if (err < 0) {
                int16_t len = x0 - xbegin;
                if (len) {
                    HLine(xbegin, y0, len + 1, color);
                } else {
                    Pixel(x0, y0, color);
                }
                xbegin = x0 + 1;
                y0 += ystep;
                err += dx;
            }
        }
        if (x0 > xbegin + 1) {
            HLine(xbegin, y0, x0 - xbegin, color);
        }
    }

}

void ST7735_t3::drawBitmap(int16_t x, int16_t y,
                           const uint8_t *bitmap, int16_t w, int16_t h,
                           uint16_t color) {

    int16_t i, j, byteWidth = (w + 7) / 8;

    for(j=0; j<h; j++) {
        for(i=0; i<w; i++ ) {
            if(bitmap[j * byteWidth + i / 8] & (128 >> (i & 7))) {
                drawPixel(x+i, y+j, color);
            }
        }
    }
}

void ST7735_t3::setCursor(int16_t x, int16_t y, bool autoCenter) {
    _center_x_text = autoCenter;	// remember the state.
    _center_y_text = autoCenter;	// remember the state.
    if (x == ST7735_t3::CENTER) {
        _center_x_text = true;
        x = _width/2;
    }
    if (y == ST7735_t3::CENTER) {
        _center_y_text = true;
        y = _height/2;
    }
    if (x < 0) x = 0;
    else if (x >= _width) x = _width - 1;
    cursor_x = x;
    if (y < 0) y = 0;
    else if (y >= _height) y = _height - 1;
    cursor_y = y;

    if(x>=scroll_x && x<=(scroll_x+scroll_width) && y>=scroll_y && y<=(scroll_y+scroll_height)){
        isWritingScrollArea	= true;
    } else {
        isWritingScrollArea = false;
    }
    _gfx_last_char_x_write = 0;	// Don't use cached data here

}

void ST7735_t3::getCursor(int16_t *x, int16_t *y) {
    *x = cursor_x;
    *y = cursor_y;
}

void ST7735_t3::setTextSize(uint8_t s_x, uint8_t s_y) {
    textsize_x = (s_x > 0) ? s_x : 1;
    textsize_y = (s_y > 0) ? s_y : 1;
}

uint8_t ST7735_t3::getTextSize() {
    return textsize_x;	// bug bug 2 values now
}

uint8_t ST7735_t3::getTextSizeX() {
    return textsize_x;
}
uint8_t ST7735_t3::getTextSizeY() {
    return textsize_y;
}

void ST7735_t3::setTextColor(uint16_t c) {
    // For 'transparent' background, we'll set the bg
    // to the same as fg instead of using a flag
    textcolor = textbgcolor = c;
}

void ST7735_t3::setTextColor(uint16_t c, uint16_t b) {
    textcolor   = c;
    textbgcolor = b;
    // pre-expand colors for fast alpha-blending later
    textcolorPrexpanded = (textcolor | (textcolor << 16)) & 0b00000111111000001111100000011111;
    textbgcolorPrexpanded = (textbgcolor | (textbgcolor << 16)) & 0b00000111111000001111100000011111;
}

void ST7735_t3::setTextWrap(bool  w) {
    wrap = w;
}

bool  ST7735_t3::getTextWrap()
{
    return wrap;
}

uint8_t ST7735_t3::getRotation(void) {
    return _rot;
}


/***************************************************************************************
** Function name:           setTextDatum
** Description:             Set the text position reference datum
***************************************************************************************/
void ST7735_t3::setTextDatum(uint8_t d)
{
    textdatum = d;
}


/***************************************************************************************
** Function name:           drawNumber
** Description:             draw a long integer
***************************************************************************************/
int16_t ST7735_t3::drawNumber(long long_num, int poX, int poY)
{
    char str[14];
    sprintf(str, "%d", long_num);
    return drawString(str, poX, poY);
}


int16_t ST7735_t3::drawFloat(float floatNumber, int dp, int poX, int poY)
{
    char str[14];               // Array to contain decimal string
    uint8_t ptr = 0;            // Initialise pointer for array
    int8_t  digits = 1;         // Count the digits to avoid array overflow
    float rounding = 0.5;       // Round up down delta
    char *format = "%%.%df";
    char *format2 = "%%.%df";
    sprintf(format2, format, dp);
    sprintf(str, format2, floatNumber);

    // Finally we can plot the string and return pixel length
    return drawString(str, poX, poY);
}

/***************************************************************************************
** Function name:           drawString (with or without user defined font)
** Description :            draw string with padding if it is defined
***************************************************************************************/
// Without font number, uses font set by setTextFont()
int16_t ST7735_t3::drawString(const string& string, int poX, int poY)
{
    int16_t len = string.length() + 2;
    return drawString1((char*)string.c_str(), string.length(), poX, poY);
}

int16_t ST7735_t3::drawString1(char string[], int16_t len, int poX, int poY)
{
    int16_t sumX = 0;
    uint8_t padding = 1/*, baseline = 0*/;

    uint16_t cwidth = strPixelLen(string); // Find the pixel width of the string in the font
    uint16_t cheight = textsize_y*8;


    if (textdatum || padX)
    {
        switch(textdatum) {
            case TC_DATUM:
                poX -= cwidth/2;
                padding += 1;
                break;
            case TR_DATUM:
                poX -= cwidth;
                padding += 2;
                break;
            case ML_DATUM:
                poY -= cheight/2;
                //padding += 0;
                break;
            case MC_DATUM:
                poX -= cwidth/2;
                poY -= cheight/2;
                padding += 1;
                break;
            case MR_DATUM:
                poX -= cwidth;
                poY -= cheight/2;
                padding += 2;
                break;
            case BL_DATUM:
                poY -= cheight;
                //padding += 0;
                break;
            case BC_DATUM:
                poX -= cwidth/2;
                poY -= cheight;
                padding += 1;
                break;
            case BR_DATUM:
                poX -= cwidth;
                poY -= cheight;
                padding += 2;
                break;
                /*
                 case L_BASELINE:
                   poY -= baseline;
                   //padding += 0;
                   break;
                 case C_BASELINE:
                   poX -= cwidth/2;
                   poY -= baseline;
                   //padding += 1;
                   break;
                 case R_BASELINE:
                   poX -= cwidth;
                   poY -= baseline;
                   padding += 2;
                   break;
               */
        }
        // Check coordinates are OK, adjust if not
        if (poX < 0) poX = 0;
        if (poX+cwidth > width())   poX = width() - cwidth;
        if (poY < 0) poY = 0;
        //if (poY+cheight-baseline >_height) poY = _height - cheight;
    }
    //if(font == NULL){
    //    for(uint8_t i = 0; i < len-2; i++){
    //        drawChar((int16_t) (poX+sumX), (int16_t) poY, string[i], textcolor, textbgcolor, textsize_x, textsize_y);
    //        sumX += cwidth/(len-2) + padding;
    //    }
    //}
    else {
        setCursor(poX, poY);
        for(uint8_t i = 0; i < len-2; i++){
            drawFontChar(string[i]);
            setCursor(cursor_x, cursor_y);
        }
    }
    return sumX;
}


void ST7735_t3::scrollTextArea(uint8_t scrollSize){
    uint16_t awColors[scroll_width];
    for (int y=scroll_y+scrollSize; y < (scroll_y+scroll_height); y++) {
        readRect(scroll_x, y, scroll_width, 1, awColors);
        writeRect(scroll_x, y-scrollSize, scroll_width, 1, awColors);
    }
    fillRect(scroll_x, (scroll_y+scroll_height)-scrollSize, scroll_width, scrollSize, scrollbgcolor);
}

void ST7735_t3::setScrollTextArea(int16_t x, int16_t y, int16_t w, int16_t h){
    scroll_x = x;
    scroll_y = y;
    scroll_width = w;
    scroll_height = h;
}

void ST7735_t3::setScrollBackgroundColor(uint16_t color){
    scrollbgcolor=color;
    fillRect(scroll_x,scroll_y,scroll_width,scroll_height,scrollbgcolor);
}

void ST7735_t3::enableScroll(void){
    scrollEnable = true;
}

void ST7735_t3::disableScroll(void){
    scrollEnable = false;
}

void ST7735_t3::resetScrollBackgroundColor(uint16_t color){
    scrollbgcolor=color;
}


// overwrite functions from class Print:

size_t ST7735_t3::write(uint16_t c) {
    return write(&c, 1);
}

size_t ST7735_t3::write(const uint16_t *buffer, size_t size)
{
    // Lets try to handle some of the special font centering code that was done for default fonts.
    if (_center_x_text || _center_y_text ) {
        int16_t x, y;
        uint16_t strngWidth, strngHeight;
        getTextBounds((uint8_t*)buffer, size, 0, 0, &x, &y, &strngWidth, &strngHeight);
        //Serial.printf("_fontwrite bounds: %d %d %u %u\n", x, y, strngWidth, strngHeight);
        // Note we may want to play with the x ane y returned if they offset some
        if (_center_x_text && strngWidth > 0){//Avoid operations for strngWidth = 0
            cursor_x -= ((x + strngWidth) / 2);
        }
        if (_center_y_text && strngHeight > 0){//Avoid operations for strngWidth = 0
            cursor_y -= ((y + strngHeight) / 2);
        }
        _center_x_text = false;
        _center_y_text = false;
    }

    size_t cb = size;
    while (cb) {
        uint8_t c = *buffer++;
        cb--;

        if (gfxFont)  {
            if (c == '\n') {
                cursor_y += (int16_t)textsize_y * gfxFont->yAdvance;
                if(scrollEnable && isWritingScrollArea){
                    cursor_x  = scroll_x;
                }else{
                    cursor_x  = 0;
                }
            } else {
                drawGFXFontChar(c);
            }
        } else {
            if (c == '\n') {
                cursor_y += textsize_y*8;
                if(scrollEnable && isWritingScrollArea){
                    cursor_x  = scroll_x;
                }else{
                    cursor_x  = 0;
                }
            } else if (c == '\r') {
                // skip em
            } else {
                if(scrollEnable && isWritingScrollArea && (cursor_y > (scroll_y+scroll_height - textsize_y*8))){
                    scrollTextArea(textsize_y*8);
                    cursor_y -= textsize_y*8;
                    cursor_x = scroll_x;
                }
                drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x, textsize_y);
                cursor_x += textsize_x*6;
                if(wrap && scrollEnable && isWritingScrollArea && (cursor_x > (scroll_x+scroll_width - textsize_x*6))){
                    cursor_y += textsize_y*8;
                    cursor_x = scroll_x;
                }
                else if (wrap && (cursor_x > (_width - textsize_x*6))) {
                    cursor_y += textsize_y*8;
                    cursor_x = 0;
                }
            }
        }
    }
    return size;
}

// Draw a character
void ST7735_t3::drawChar(int16_t x, int16_t y, unsigned char c,
                         uint16_t fgcolor, uint16_t bgcolor, uint8_t size_x, uint8_t size_y)
{
    if((x >= _width)            || // Clip right
       (y >= _height)           || // Clip bottom
       ((x + 6 * size_x - 1) < 0) || // Clip left  TODO: is this correct?
       ((y + 8 * size_y - 1) < 0))   // Clip top   TODO: is this correct?
        return;

    if (fgcolor == bgcolor) {
        // This transparent approach is only about 20% faster
        if ((size_x == 1) && (size_y == 1)) {
            uint8_t mask = 0x01;
            int16_t xoff, yoff;
            for (yoff=0; yoff < 8; yoff++) {
                uint8_t line = 0;
                for (xoff=0; xoff < 5; xoff++) {
                    if (glcdfont[c * 5 + xoff] & mask) line |= 1;
                    line <<= 1;
                }
                line >>= 1;
                xoff = 0;
                while (line) {
                    if (line == 0x1F) {
                        drawFastHLine(x + xoff, y + yoff, 5, fgcolor);
                        break;
                    } else if (line == 0x1E) {
                        drawFastHLine(x + xoff, y + yoff, 4, fgcolor);
                        break;
                    } else if ((line & 0x1C) == 0x1C) {
                        drawFastHLine(x + xoff, y + yoff, 3, fgcolor);
                        line <<= 4;
                        xoff += 4;
                    } else if ((line & 0x18) == 0x18) {
                        drawFastHLine(x + xoff, y + yoff, 2, fgcolor);
                        line <<= 3;
                        xoff += 3;
                    } else if ((line & 0x10) == 0x10) {
                        drawPixel(x + xoff, y + yoff, fgcolor);
                        line <<= 2;
                        xoff += 2;
                    } else {
                        line <<= 1;
                        xoff += 1;
                    }
                }
                mask = mask << 1;
            }
        } else {
            uint8_t mask = 0x01;
            int16_t xoff, yoff;
            for (yoff=0; yoff < 8; yoff++) {
                uint8_t line = 0;
                for (xoff=0; xoff < 5; xoff++) {
                    if (glcdfont[c * 5 + xoff] & mask) line |= 1;
                    line <<= 1;
                }
                line >>= 1;
                xoff = 0;
                while (line) {
                    if (line == 0x1F) {
                        fillRect(x + xoff * size_x, y + yoff * size_y,
                                 5 * size_x, size_y, fgcolor);
                        break;
                    } else if (line == 0x1E) {
                        fillRect(x + xoff * size_x, y + yoff * size_y,
                                 4 * size_x, size_y, fgcolor);
                        break;
                    } else if ((line & 0x1C) == 0x1C) {
                        fillRect(x + xoff * size_x, y + yoff * size_y,
                                 3 * size_x, size_y, fgcolor);
                        line <<= 4;
                        xoff += 4;
                    } else if ((line & 0x18) == 0x18) {
                        fillRect(x + xoff * size_x, y + yoff * size_y,
                                 2 * size_x, size_y, fgcolor);
                        line <<= 3;
                        xoff += 3;
                    } else if ((line & 0x10) == 0x10) {
                        fillRect(x + xoff * size_x, y + yoff * size_y,
                                 size_x, size_y, fgcolor);
                        line <<= 2;
                        xoff += 2;
                    } else {
                        line <<= 1;
                        xoff += 1;
                    }
                }
                mask = mask << 1;
            }
        }
    } else {
        // This solid background approach is about 5 time faster
        uint8_t xc, yc;
        uint8_t xr, yr;
        uint8_t mask = 0x01;
        uint16_t color;

        // We need to offset by the origin.
        x += _originx;
        y += _originy;
        int16_t x_char_start = x;  // remember our X where we start outputting...

        if ((x >= _displayclipx2) || // Clip right
            (y >= _displayclipy2) || // Clip bottom
            ((x + 6 * size_x - 1) < _displayclipx1) || // Clip left  TODO: this is not correct
            ((y + 8 * size_y - 1) < _displayclipy1))   // Clip top   TODO: this is not correct
            return;

    }
}


// Maybe support GFX Fonts as well?
void ST7735_t3::setFont(const GFXfont *f) {
    //font = NULL;	// turn off the other font...
    _gfx_last_char_x_write = 0;	// Don't use cached data here
    if (f == gfxFont) return;	// same font or lack of so can bail.

    if(f) {            // Font struct pointer passed in?
        if(!gfxFont) { // And no current font struct?
            // Switching from classic to new font behavior.
            // Move cursor pos down 6 pixels so it's on baseline.
            cursor_y += 6;
        }

        // Test wondering high and low of Ys here...
        int8_t miny_offset = 0;
        for (uint8_t i=0; i <= (f->last - f->first); i++) {
            if (f->glyph[i].yOffset < miny_offset) {
                miny_offset = f->glyph[i].yOffset;
            }
        }

        _gfxFont_min_yOffset = miny_offset;	// Probably only thing we need... May cache?

    } else if(gfxFont) { // NULL passed.  Current font struct defined?
        // Switching from new to classic font behavior.
        // Move cursor pos up 6 pixels so it's at top-left of char.
        cursor_y -= 6;
    }
    gfxFont = f;
}

static uint32_t fetchbit(const uint8_t *p, uint32_t index)
{
    if (p[index >> 3] & (1 << (7 - (index & 7)))) return 1;
    return 0;
}

static uint32_t fetchbits_unsigned(const uint8_t *p, uint32_t index, uint32_t required)
{
    uint32_t val = 0;
    do {
        uint8_t b = p[index >> 3];
        uint32_t avail = 8 - (index & 7);
        if (avail <= required) {
            val <<= avail;
            val |= b & ((1 << avail) - 1);
            index += avail;
            required -= avail;
        } else {
            b >>= avail - required;
            val <<= required;
            val |= b & ((1 << required) - 1);
            break;
        }
    } while (required);
    return val;
}

static uint32_t fetchbits_signed(const uint8_t *p, uint32_t index, uint32_t required)
{
    uint32_t val = fetchbits_unsigned(p, index, required);
    if (val & (1 << (required - 1))) {
        return (int32_t)val - (1 << required);
    }
    return (int32_t)val;
}

// Add in Adafruit versions of text bounds calculations.
void ST7735_t3::getTextBounds(const uint8_t *buffer, uint16_t len, int16_t x, int16_t y,
                              int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h) {
    *x1 = x;
    *y1 = y;
    *w  = *h = 0;

    int16_t minx = _width, miny = _height, maxx = -1, maxy = -1;

    while(len--)
        charBounds(*buffer++, &x, &y, &minx, &miny, &maxx, &maxy);

    if(maxx >= minx) {
        *x1 = minx;
        *w  = maxx - minx + 1;
    }
    if(maxy >= miny) {
        *y1 = miny;
        *h  = maxy - miny + 1;
    }
}
void ST7735_t3::getTextBounds(const char *str, int16_t x, int16_t y,
                              int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h) {
    uint8_t c; // Current character

    *x1 = x;
    *y1 = y;
    *w  = *h = 0;

    int16_t minx = _width, miny = _height, maxx = -1, maxy = -1;

    while((c = *str++))
        charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy);

    if(maxx >= minx) {
        *x1 = minx;
        *w  = maxx - minx + 1;
    }
    if(maxy >= miny) {
        *y1 = miny;
        *h  = maxy - miny + 1;
    }
}

void ST7735_t3::getTextBounds(const string &str, int16_t x, int16_t y,
                              int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h) {
    if (str.length() != 0) {
        getTextBounds(const_cast<char*>(str.c_str()), x, y, x1, y1, w, h);
    }
}



void ST7735_t3::drawFontPixel( uint8_t alpha, uint32_t x, uint32_t y ){
    // Adjust alpha based on the number of alpha levels supported by the font (based on bpp)
    // Note: Implemented look-up table for alpha, but made absolutely no difference in speed (T3.6)
    alpha = (uint8_t)(alpha * fontalphamx);
    uint32_t result = ((((textcolorPrexpanded - textbgcolorPrexpanded) * alpha) >> 5) + textbgcolorPrexpanded) & 0b00000111111000001111100000011111;
    Pixel(x,y,(uint16_t)((result >> 16) | result));
}

void ST7735_t3::drawFontBits(bool opaque, uint32_t bits, uint32_t numbits, int32_t x, int32_t y, uint32_t repeat)
{
    if (bits == 0) {
        if (opaque) {
            fillRect(x, y, numbits, repeat, textbgcolor);
        }
    } else {
        int32_t x1 = x;
        uint32_t n = numbits;
        int w;
        int bgw;

        w = 0;
        bgw = 0;

        do {
            n--;
            if (bits & (1 << n)) {
                if (bgw>0) {
                    if (opaque) {
                        fillRect(x1 - bgw, y, bgw, repeat, textbgcolor);
                    }
                    bgw=0;
                }
                w++;
            } else {
                if (w>0) {
                    fillRect(x1 - w, y, w, repeat, textcolor);
                    w = 0;
                }
                bgw++;
            }
            x1++;
        } while (n > 0);

        if (w > 0) {
            fillRect(x1 - w, y, w, repeat, textcolor);
        }

        if (bgw > 0) {
            if (opaque) {
                fillRect(x1 - bgw, y, bgw, repeat, textbgcolor);
            }
        }
    }
}

void ST7735_t3::drawGFXFontChar(unsigned int c) {
    // Lets do Adafruit GFX character output here as well
    if(c == '\r') 	 return;

    // Some quick and dirty tests to see if we can
    uint8_t first = gfxFont->first;
    if((c < first) || (c > gfxFont->last)) return;

    GFXglyph *glyph  = gfxFont->glyph + (c - first);
    uint8_t   w     = glyph->width,
            h     = glyph->height;

    // wonder if we should look at xo, yo instead?
    if((w == 0 ||  h == 0)  && (c != 32))   return;  // Is there an associated bitmap?

    int16_t xo = glyph->xOffset; // sic
    int16_t yo = glyph->yOffset + gfxFont->yAdvance/2;

    if(wrap && ((cursor_x + textsize_x * (xo + w)) > _width)) {
        cursor_x  = 0;
        cursor_y += (int16_t)textsize_y * gfxFont->yAdvance;
    }

    // Lets do the work to output the font character
    uint8_t  *bitmap = gfxFont->bitmap;

    uint16_t bo = glyph->bitmapOffset;
    uint8_t  xx, yy, bits = 0, bit = 0;
    //Serial.printf("DGFX_char: %c (%d,%d) : %u %u %u %u %d %d %x %x %d\n", c, cursor_x, cursor_y, w, h,
    //			glyph->xAdvance, gfxFont->yAdvance, xo, yo, textcolor, textbgcolor, _use_fbtft);Serial.flush();

    if (textcolor == textbgcolor) {

        //Serial.printf("DGFXChar: %c %u, %u, wh:%d %d o:%d %d\n", c, cursor_x, cursor_y, w, h, xo, yo);

        // NOTE: Adafruit GFX does not support Opaque font output as there
        // are issues with proportionally spaced characters that may overlap
        // So the below is not perfect as we may overwrite a small portion
        // of a letter with the next one, when we blank out...
        // But: I prefer to let each of us decide if the limitations are
        // worth it or not.  If Not you still have the option to not
        // Do transparent mode and instead blank out and blink...
        for(yy=0; yy<h; yy++) {
            uint8_t w_left = w;
            xx = 0;
            while (w_left) {
                if(!(bit & 7)) {
                    bits = bitmap[bo++];
                }
                // Could try up to 8 bits at time, but start off trying up to 4
                uint8_t xCount;
                if ((w_left >= 8) && ((bits & 0xff) == 0xff)) {
                    xCount = 8;
                    //Serial.print("8");
                    fillRect(cursor_x+(xo+xx)*textsize_x, cursor_y+(yo+yy)*textsize_y,
                             xCount * textsize_x, textsize_y, textcolor);
                } else if ((w_left >= 4) && ((bits & 0xf0) == 0xf0)) {
                    xCount = 4;
                    //Serial.print("4");
                    fillRect(cursor_x+(xo+xx)*textsize_x, cursor_y+(yo+yy)*textsize_y,
                             xCount * textsize_x, textsize_y, textcolor);
                } else if ((w_left >= 3) && ((bits & 0xe0) == 0xe0)) {
                    //Serial.print("3");
                    xCount = 3;
                    fillRect(cursor_x+(xo+xx)*textsize_x, cursor_y+(yo+yy)*textsize_y,
                             xCount * textsize_x, textsize_y, textcolor);
                } else if ((w_left >= 2) && ((bits & 0xc0) == 0xc0)) {
                    //Serial.print("2");
                    xCount = 2;
                    fillRect(cursor_x+(xo+xx)*textsize_x, cursor_y+(yo+yy)*textsize_y,
                             xCount * textsize_x, textsize_y, textcolor);
                } else {
                    xCount = 1;
                    if(bits & 0x80) {
                        if((textsize_x == 1) && (textsize_y == 1)){
                            drawPixel(cursor_x+xo+xx, cursor_y+yo+yy, textcolor);
                        } else {
                            fillRect(cursor_x+(xo+xx)*textsize_x, cursor_y+(yo+yy)*textsize_y,
                                     textsize_x, textsize_y, textcolor);
                        }
                    }
                }
                xx += xCount;
                w_left -= xCount;
                bit += xCount;
                bits <<= xCount;
            }

        }
        _gfx_last_char_x_write = 0;
    } else {
        // To Do, properly clipping and offsetting...
        // This solid background approach is about 5 time faster
        // Lets calculate bounding rectangle that we will update
        // We need to offset by the origin.

        // We are going direct so do some offsets and clipping
        int16_t x_offset_cursor = cursor_x + _originx;	// This is where the offseted cursor is.
        int16_t x_start = x_offset_cursor;  // I am assuming no negative x offsets.
        int16_t x_end = x_offset_cursor + (glyph->xAdvance * textsize_x);
        if (glyph->xAdvance < (xo + w)) x_end = x_offset_cursor + ((xo + w)* textsize_x);  // BUGBUG Overlflows into next char position.
        int16_t x_left_fill = x_offset_cursor + xo * textsize_x;
        int16_t x;

        if (xo < 0) {
            // Unusual character that goes back into previous character
            //Serial.printf("GFX Font char XO < 0: %c %d %d %u %u %u\n", c, xo, yo, w, h, glyph->xAdvance );
            x_start += xo * textsize_x;
            x_left_fill = 0;	// Don't need to fill anything here...
        }

        int16_t y_start = cursor_y + _originy + (_gfxFont_min_yOffset * textsize_y)+ gfxFont->yAdvance*textsize_y/2;  // UP to most negative value.
        int16_t y_end = y_start +  gfxFont->yAdvance * textsize_y;  // how far we will update
        int16_t y = y_start;
        //int8_t y_top_fill = (yo - _gfxFont_min_yOffset) * textsize_y;	 // both negative like -10 - -16 = 6...
        int8_t y_top_fill = (yo - gfxFont->yAdvance/2 - _gfxFont_min_yOffset) * textsize_y;

        // See if anything is within clip rectangle, if not bail
        if((x_start >= _displayclipx2)   || // Clip right
           (y_start >= _displayclipy2) || // Clip bottom
           (x_end < _displayclipx1)    || // Clip left
           (y_end < _displayclipy1))  	// Clip top
        {
            // But remember to first update the cursor position
            cursor_x += glyph->xAdvance * (int16_t)textsize_x;
            return;
        }

        // If our y_end > _displayclipy2 set it to _displayclipy2 as to not have to test both  Likewise X
        if (y_end > _displayclipy2) y_end = _displayclipy2;
        if (x_end > _displayclipx2) x_end = _displayclipx2;

        // If we get here and
        if (_gfx_last_cursor_y != (cursor_y + _originy))  _gfx_last_char_x_write = 0;


        // Save away info about this last char
        _gfx_c_last = c;
        _gfx_last_cursor_x = cursor_x + _originx;
        _gfx_last_cursor_y = cursor_y + _originy;
        _gfx_last_char_x_write = x_end;
        _gfx_last_char_textcolor = textcolor;
        _gfx_last_char_textbgcolor = textbgcolor;
    }

    cursor_x += glyph->xAdvance * (int16_t)textsize_x;
}

// Some fonts overlap characters if we detect that the previous
// character wrote out more width than they advanced in X direction
// we may want to know if the last character output a FG or BG at a position.
// Opaque font chracter overlap?
//	unsigned int _gfx_c_last;
//	int16_t   _gfx_last_cursor_x, _gfx_last_cursor_y;
//	int16_t	 _gfx_last_x_overlap = 0;

bool ST7735_t3::gfxFontLastCharPosFG(int16_t x, int16_t y) {
    GFXglyph *glyph  = gfxFont->glyph + (_gfx_c_last -  gfxFont->first);

    uint8_t   w     = glyph->width,
            h     = glyph->height;


    int16_t xo = glyph->xOffset; // sic
    int16_t yo = glyph->yOffset + gfxFont->yAdvance/2;
    if (x >= _gfx_last_char_x_write) return false; 	// we did not update here...
    if (y < (_gfx_last_cursor_y + (yo*textsize_y)))  return false;  // above
    if (y >= (_gfx_last_cursor_y + (yo+h)*textsize_y)) return false; // below


    // Lets compute which Row this y is in the bitmap
    int16_t y_bitmap = (y - ((_gfx_last_cursor_y + (yo*textsize_y))) + textsize_y - 1) / textsize_y;
    int16_t x_bitmap = (x - ((_gfx_last_cursor_x + (xo*textsize_x))) + textsize_x - 1) / textsize_x;
    uint16_t  pixel_bit_offset = y_bitmap * w + x_bitmap;

    return ((gfxFont->bitmap[glyph->bitmapOffset + (pixel_bit_offset >> 3)]) & (0x80 >> (pixel_bit_offset & 0x7)));
}


