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
#include "pins_arduino.h"
#include <cstdint>
#include "cstdlib"
#include <cstdlib>
#include <cstdio>
#include <cmath>
#ifdef ENABLE_ST77XX_FRAMEBUFFER
//#define DEBUG_ASYNC_UPDATE
//#define DEBUG_ASYNC_LEDS
#ifdef DEBUG_ASYNC_LEDS
  #define DEBUG_PIN_1 0
  #define DEBUG_PIN_2 1
  #define DEBUG_PIN_3 2
#endif

volatile short _dma_dummy_rx;

ST7735_t3 *ST7735_t3::_dmaActiveDisplay[3] = {0, 0, 0};

#if defined(__MK66FX1M0__)
 DMASetting   ST7735_t3::_dmasettings[3][4];
#endif

#if defined(__IMXRT1062__)  // Teensy 4.x
// On T4 Setup the buffers to be used one per SPI buss...
// This way we make sure it is hopefully in uncached memory
ST7735DMA_Data ST7735_t3::_dma_data[3];   // one structure for each SPI buss...
#endif

#endif

// Constructor when using software SPI.  All output pins are configurable.
ST7735_t3::ST7735_t3(uint8_t cs, uint8_t rs, uint8_t sid, uint8_t sclk, uint8_t rst)
{
    _cs   = cs;
    _rs   = rs;
    _sid  = sid;
    _sclk = sclk;
    _rst  = rst;
    _rot = 0xff;
    hwSPI = false;
#ifdef ENABLE_ST77XX_FRAMEBUFFER
    _pfbtft = NULL;
    _use_fbtft = 0;						// Are we in frame buffer mode?
	_we_allocated_buffer = NULL;
	_dma_state = 0;
#endif
    _screenHeight = ST7735_TFTHEIGHT_144;
    _screenWidth = ST7735_TFTWIDTH;

    _width = _screenWidth;
    _height = _screenHeight;

    cursor_y  = cursor_x    = 0;
    textsize_x  = 1;
    textsize_y  = 1;
    textcolor = textbgcolor = 0xFFFF;
    wrap      = true;
    font      = NULL;
    gfxFont   = NULL;
    setClipRect();
    setOrigin();
}


// Constructor when using hardware SPI.  Faster, but must use SPI pins
// specific to each board type (e.g. 11,13 for Uno, 51,52 for Mega, etc.)
ST7735_t3::ST7735_t3(uint8_t cs, uint8_t rs, uint8_t rst)
{
    _cs   = cs;
    _rs   = rs;
    _rst  = rst;
    _rot = 0xff;
    hwSPI = true;
    _sid  = _sclk = (uint8_t)-1;
#ifdef ENABLE_ST77XX_FRAMEBUFFER
    _pfbtft = NULL;
    _use_fbtft = 0;						// Are we in frame buffer mode?
	_we_allocated_buffer = NULL;
	_dma_state = 0;
#endif
    _screenHeight = ST7735_TFTHEIGHT_144;
    _screenWidth = ST7735_TFTWIDTH;

    cursor_y  = cursor_x    = 0;
    textsize_x  = 1;
    textsize_y  = 1;
    textcolor = textbgcolor = 0xFFFF;
    wrap      = true;
    font      = NULL;
    gfxFont   = NULL;
    setClipRect();
    setOrigin();
}


inline void ST7735_t3::spiwrite(uint8_t c)
{
}

inline void ST7735_t3::spiwrite16(uint16_t d)
{

}

void ST7735_t3::writecommand(uint8_t c)
{

}

void ST7735_t3::writecommand_last(uint8_t c) {
}

void ST7735_t3::writedata(uint8_t c)
{
}

void ST7735_t3::writedata_last(uint8_t c)
{
}

void ST7735_t3::writedata16(uint16_t d)
{
}


void ST7735_t3::writedata16_last(uint16_t d)
{
}

void ST7735_t3::setBitrate(uint32_t n)
{
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void ST7735_t3::commandList(const uint8_t *addr)
{
}


// Initialization code common to both 'B' and 'R' type displays
void ST7735_t3::commonInit(const uint8_t *cmdList, uint8_t mode)
{
    _colstart  = _rowstart = 0; // May be overridden in init func
    _ystart = _xstart = 0;
}


// Initialization for ST7735B screens
void ST7735_t3::initB(void)
{
//    commonInit(Bcmd);
}


// Initialization for ST7735R screens (green or red tabs)
void ST7735_t3::initR(uint8_t options)
{
    tabcolor = options;
    setRotation(0);
}
void ST7735_t3::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    AbstractDisplay::drawLine(x0, y0, x1, y1, color);
#ifdef ENABLE_ST77XX_FRAMEBUFFER
    if (!_use_fbtft) {
		writecommand_last(ST7735_NOP);
		endSPITransaction();
  	}
#else
#endif
}

/*!
 @brief   Adafruit_SPITFT Send Command handles complete sending of commands and const data
 @param   commandByte       The Command Byte
 @param   dataBytes         A pointer to the Data bytes to send
 @param   numDataBytes      The number of bytes we should send
 */
void ST7735_t3::sendCommand(uint8_t commandByte, const uint8_t *dataBytes, uint8_t numDataBytes) {

}
// overwrite functions from class Print:

