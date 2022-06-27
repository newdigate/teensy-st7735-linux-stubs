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

#ifndef __ST7735_t3_H_
#define __ST7735_t3_H_

#include "Arduino.h"
#include "AbstractDisplay.h"

#include "ILI9341_fonts.h"
#include <cmath>
#define INITR_GREENTAB 0x0
#define INITR_REDTAB   0x1
#define INITR_BLACKTAB 0x2

#define INITR_18GREENTAB    INITR_GREENTAB
#define INITR_18REDTAB      INITR_REDTAB
#define INITR_18BLACKTAB    INITR_BLACKTAB
#define INITR_144GREENTAB   0x1
#define INITR_144GREENTAB_OFFSET   0x4
#define INITR_MINI160x80  0x05
#define INITR_MINI160x80_ST7735S 0x06

#define ST7735_TFTWIDTH  128
#define ST7735_TFTWIDTH_80     80 // for mini
// for 1.44" display
#define ST7735_TFTHEIGHT_144 128
// for 1.8" display and mini
#define ST7735_TFTHEIGHT_160  160 // for 1.8" and mini display

#define ST7735_NOP     0x00
#define ST7735_RAMWR   0x2C
// Color definitions
#define ST7735_BLACK   0x0000
#define ST7735_BLUE    0x001F
#define ST7735_RED     0xF800
#define ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0xFFE0
#define ST7735_WHITE   0xFFFF

// Also define them in a non specific ST77XX specific name
#define ST77XX_BLACK      0x0000
#define ST77XX_WHITE      0xFFFF
#define ST77XX_RED        0xF800
#define ST77XX_GREEN      0x07E0
#define ST77XX_BLUE       0x001F
#define ST77XX_CYAN       0x07FF
#define ST77XX_MAGENTA    0xF81F
#define ST77XX_YELLOW     0xFFE0
#define ST77XX_ORANGE     0xFC00
#define ST77XX_PINK       0xF81F

// Map fonts that were modified back to the ILI9341 font
#define ST7735_t3_font_t ILI9341_t3_font_t

#ifndef st7735_swap
#define st7735_swap(a, b) { typeof(a) t = a; a = b; b = t; }
#endif



#define CL(_r,_g,_b) ((((_r)&0xF8)<<8)|(((_g)&0xFC)<<3)|((_b)>>3))

//These enumerate the text plotting alignment (reference datum point)

//#define L_BASELINE  9 // Left character baseline (Line the 'A' character would sit on)
//#define C_BASELINE 10 // Centre character baseline
//#define R_BASELINE 11 // Right character baseline
#ifdef __cplusplus

#define ST7735_min(a, b) ((a < b)? a : b )
#define ST7735_max(a, b) ((a > b)? a : b )

class ST7735_t3 : public AbstractDisplay
{

public:

    ST7735_t3(uint8_t CS, uint8_t RS, uint8_t SID, uint8_t SCLK, uint8_t RST = -1);
    ST7735_t3(uint8_t CS, uint8_t RS, uint8_t RST = -1);

    void     initB(void),                             // for ST7735B displays
            initR(uint8_t options = INITR_GREENTAB); // for ST7735R


 

    //////
    int write(uint8_t) override;
    int write(const uint8_t *buffer, size_t size) override;
    virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) override;
////
protected:
    void sendCommand(uint8_t commandByte, const uint8_t *dataBytes, uint8_t numDataBytes);



   
    void setBitrate(uint32_t n);



protected:

    virtual void writedata16(uint16_t d) override;
    virtual void writedata16_last(uint16_t d) override;
    void    spiwrite(uint8_t),
            spiwrite16(uint16_t d),
            writecommand(uint8_t c),
            writecommand_last(uint8_t c),
            writedata(uint8_t d),

            writedata_last(uint8_t d),
            
            commandList(const uint8_t *addr),
            commonInit(const uint8_t *cmdList, uint8_t mode=0);
    
//uint8_t  spiread(void);

    



    uint8_t  _cs, _rs, _rst, _sid, _sclk;
    uint8_t pcs_data, pcs_command;
    uint32_t ctar;
    volatile uint8_t *datapin, *clkpin, *cspin, *rspin;

    virtual void Pixel(int16_t x, int16_t y, uint16_t color) override
    {
        x+=_originx;
        y+=_originy;

        setAddr(x, y, x, y);
        writecommand(ST7735_RAMWR);
        writedata16(color);
    }
};

#define Adafruit_GFX_Button ST7735_Button
class ST7735_Button {
public:
    ST7735_Button(void) { _gfx = NULL; }
    void initButton(ST7735_t3 *gfx, int16_t x, int16_t y,
                    uint8_t w, uint8_t h,
                    uint16_t outline, uint16_t fill, uint16_t textcolor,
                    const char *label, uint8_t textsize_x, uint8_t textsize_y) {
        _x = x;
        _y = y;
        _w = w;
        _h = h;
        _outlinecolor = outline;
        _fillcolor = fill;
        _textcolor = textcolor;
        _textsize_x = textsize_x;
        _textsize_y = textsize_y;
        _gfx = gfx;
        strncpy(_label, label, 9);
        _label[9] = 0;

    }
    void drawButton(bool inverted = false) {
        uint16_t fill, outline, text;

        if (! inverted) {
            fill = _fillcolor;
            outline = _outlinecolor;
            text = _textcolor;
        } else {
            fill =  _textcolor;
            outline = _outlinecolor;
            text = _fillcolor;
        }
        _gfx->fillRoundRect(_x - (_w/2), _y - (_h/2), _w, _h, ST7735_min(_w,_h)/4, fill);
        _gfx->drawRoundRect(_x - (_w/2), _y - (_h/2), _w, _h, ST7735_min(_w,_h)/4, outline);
        _gfx->setCursor(_x - strlen(_label)*3*_textsize_x, _y-4*_textsize_y);
        _gfx->setTextColor(text);
        _gfx->setTextSize(_textsize_x, _textsize_y);
        _gfx->print(_label);
    }

    bool contains(int16_t x, int16_t y) {
        if ((x < (_x - _w/2)) || (x > (_x + _w/2))) return false;
        if ((y < (_y - _h/2)) || (y > (_y + _h/2))) return false;
        return true;
    }

    void press(bool p) {
        laststate = currstate;
        currstate = p;
    }
    bool isPressed() { return currstate; }
    bool justPressed() { return (currstate && !laststate); }
    bool justReleased() { return (!currstate && laststate); }

private:
    ST7735_t3 *_gfx;
    int16_t _x, _y;
    uint16_t _w, _h;
    uint8_t _textsize_x, _textsize_y;
    uint16_t _outlinecolor, _fillcolor, _textcolor;
    char _label[10];
    bool currstate, laststate;

};

#endif	 //end cplus

#endif