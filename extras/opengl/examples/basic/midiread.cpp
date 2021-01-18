#include "../../src/st7735_opengl.h"

st7735_opengl tft = st7735_opengl();

int main() {
    tft.initR(INITR_GREENTAB);
    tft.setRotation(3);

    tft.fillScreen(ST7735_BLACK);
    tft.println("init...");

    Serial.begin(9600);

    tft.println("sd...");
    tft.println("usb...");

    tft.println("midi1");

    //cs42448_1.inputLevel(1);
    tft.println("cs42448");

    tft.fillScreen(ST7735_BLACK);
    tft.setCursor(0,0);
    tft.println("waiting for akai APC40...");
    //Serial.println("waiting for akai APC40...");

    tft.fillScreen(ST7735_BLACK);
    tft.setCursor(0,0);
    tft.println("connected to APC40...");
}

unsigned __exidx_start;
unsigned __exidx_end;