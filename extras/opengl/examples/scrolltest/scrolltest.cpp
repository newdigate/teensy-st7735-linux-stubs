/***************************************************
  This is our GFX example for the Adafruit ST7735 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "st7735_opengl.h"
#include "st7735_opengl_main.h"
#include "../../../src/st7735_t3_font_ComicSansMS.h"

#define ST7735_BLACK 0x0000
#define ST7735_WHITE 0xFFFF
#define ST7735_RED 0xF800
#define ST7735_GREEN 0x07E0
#define ST7735_BLUE 0x001F
#define ST7735_CYAN 0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW 0xFFE0
#define ST7735_ORANGE 0xFC00

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
st7735_opengl_noinput tft = st7735_opengl_noinput(true, 10);

int st7735_main(int argc, char** argv) {
    return 0;
}

void setup() {

    Serial.begin(9600);

    tft.setRotation(3);
    tft.useFrameBuffer(true);
    tft.updateScreenAsync(true);

    tft.fillScreen(ST7735_BLACK);
    //while (!Serial) ;
    tft.setTextColor(ST7735_WHITE);
    tft.setTextSize(1);
    tft.enableScroll();
    tft.setScrollTextArea(0,0,128,128);
    tft.setScrollBackgroundColor(ST7735_GREEN);

    tft.setCursor(32, 32);

    tft.setFont(ComicSansMS_12);
    tft.print("Fixed text");

    tft.setCursor(0, 0);

    tft.setTextColor(ST7735_BLUE);

    for(int i=0;i<20;i++){
        tft.print("this is line ");
        tft.println(i);

        tft.updateScreen();
        delay(100);
    }

    tft.fillScreen(ST7735_BLACK);
    tft.setScrollTextArea(20,20,128-40,128-40);
    tft.setScrollBackgroundColor(ST7735_GREEN);
    tft.setFont(ComicSansMS_10);

    tft.setTextSize(1);
    tft.setCursor(20, 20);

    for(int i=0;i<20;i++){
        tft.print("this is line ");
        tft.println(i);
        tft.updateScreen();
        delay(500);
    }
}

void loop() {
    tft.updateScreen();
    delay(10);
 }

