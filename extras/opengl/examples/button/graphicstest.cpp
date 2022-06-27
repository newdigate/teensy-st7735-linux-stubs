#include "st7735_opengl.h"
#include "st7735_opengl_main.h"
#include <Bounce2.h>

Bounce2::Button button;
st7735_opengl tft = st7735_opengl(true, 10, nullptr, nullptr, &button);

int st7735_main(int argc, char** argv) {
}

bool buttonWasPressed = false;

void setup() {
    tft.initR(INITR_GREENTAB);
    tft.useFrameBuffer(true);
    tft.updateScreenAsync(true);
    tft.fillScreen(ST7735_BLACK);
}

void loop() {
    button.update();
    delay(50);
    bool isPressed = button.isPressed();
    if (isPressed != buttonWasPressed) {
        if (isPressed) {
            Serial.println("pressed...");
            tft.fillScreen(ST7735_BLACK);
        } else {
            Serial.println("released...");
            tft.fillScreen(ST7735_WHITE);
        }
        buttonWasPressed = isPressed;
    };
}

unsigned __exidx_start;
unsigned __exidx_end;