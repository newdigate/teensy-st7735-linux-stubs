#include "st7735_opengl.h"
#include "st7735_opengl_main.h"
#include <Bounce2.h>
#include <Encoder.h>

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
Encoder encoderLeftRight, encoderUpDown;
st7735_opengl<Encoder, Bounce2::Button> tft = st7735_opengl<Encoder, Bounce2::Button>(true, 10, &encoderUpDown, &encoderLeftRight, &button);


// encoder stuff
long Position = 0, oldPosition = 0;
long PositionY = 0, oldPositionY = 0;

int st7735_main(int argc, char** argv) {
    return 0;
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

    bool left = false, right = false, up = false, down = false;

    Position = encoderLeftRight.read();
    if ((Position - oldPosition) < 0) {
        left = true;
    }
    if ((Position - oldPosition) > 0) {
        right = true;
    }

    PositionY = encoderUpDown.read();
    if ((PositionY - oldPositionY) > 0) {
        up = true;
    }
    if ((PositionY - oldPositionY) < 0) {
        down = true;
    }

    if (right) {
        Serial.println("right");
    } else if (left) {
        Serial.println("left");
    } else if (up) {
        Serial.println("up");
    }
    else if (down) {
        Serial.println("down");
    }

    oldPosition = Position;
    oldPositionY = PositionY;
}

unsigned __exidx_start;
unsigned __exidx_end;