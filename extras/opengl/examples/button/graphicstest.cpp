#include "st7735_opengl.h"
#include "st7735_opengl_main.h"
#include <Bounce2.h>

#define ST7735_BLACK 0x0000
#define ST7735_WHITE 0xFFFF
#define ST7735_RED 0xF800
#define ST7735_GREEN 0x07E0
#define ST7735_BLUE 0x001F
#define ST7735_CYAN 0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW 0xFFE0
#define ST7735_ORANGE 0xFC00

Bounce2::Button button;
st7735_opengl<NoEncoder, Bounce2::Button> tft = st7735_opengl<NoEncoder, Bounce2::Button>(true, 10, nullptr, nullptr, &button);

int st7735_main(int argc, char** argv) {
}

bool buttonWasPressed = false;

void setup() {

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