#include "../../src/st7735_opengl.h"

st7735_opengl tft = st7735_opengl();

float p = 3.1415926;

void testcurves();
void testlines();

void drawTestCurve(const float *points, uint16_t color, uint16_t bgcolor, uint16_t pointscolor, bool antiliased);
void drawTestLines(const float *points, uint16_t color, uint16_t bgcolor, uint16_t pointscolor, bool antiliased);

uint16_t color = ST7735_WHITE;
uint16_t bgcolor = ST7735_BLACK;
uint16_t pointscolor = ST7735_RED;

int main() {
    initialize_mock_arduino();

    tft.initR(INITR_GREENTAB);
    tft.setRotation(3);
    tft.fillScreen(bgcolor);

    testcurves();
    testlines();

    Serial.print("done\n\n");
    delay(1000);
    while(!tft.shouldClose()) {
        tft.update();
        delay(10);
    }
}

void testcurves() {
    float points[8] = {10.0, 10.0,
                       20.0, 0.0,
                       30.0, 10.0,
                       40.0, 20.0};
    drawTestCurve(points, color, bgcolor, pointscolor, false);

    float points2[8] = {10.0, 40.0,
                       20.0, 30.0,
                       30.0, 40.0,
                       40.0, 50.0};
    drawTestCurve(points2, color, bgcolor, pointscolor, true);

}
void testlines() {
    float points[4] = {10.0, 70.0,
                       20.0, 60.0};
    drawTestLines(points, color, bgcolor, pointscolor, false);

    float points3[4] = {40.0, 70.0,
                       50.0, 70.0};
    drawTestLines(points3, color, bgcolor, pointscolor, false);


    float points2[4] = {10.0, 100.0,
                        20.0, 90.0};
    drawTestLines(points2, color, bgcolor, pointscolor, true);

    float points4[4] = {40.0, 100.0,
                        50.0, 100.0};
    drawTestLines(points4, color, bgcolor, pointscolor, true);

}

void drawTestCurve(const float *points, uint16_t color, uint16_t bgcolor, uint16_t pointscolor, bool antiliased) {
    if (antiliased)
        tft.drawCurve(0.3,
                      points[0], points[1],
                      points[2], points[3],
                      points[4], points[5],
                      points[6], points[7],
                      color, bgcolor);
    else
        tft.drawCurve(0.3,
                  points[0], points[1],
                  points[2], points[3],
                  points[4], points[5],
                  points[6], points[7],
                  color);
    tft.drawPixel(points[0], points[1], pointscolor);
    tft.drawPixel(points[2], points[3], pointscolor);
    tft.drawPixel(points[4], points[5], pointscolor);
    tft.drawPixel(points[6], points[7], pointscolor);
}

void drawTestLines(const float *points, uint16_t color, uint16_t bgcolor, uint16_t pointscolor, bool antiliased) {
    if (antiliased)
        tft.drawLine(points[0], points[1],
                      points[2], points[3],
                      color, bgcolor);
    else
        tft.drawLine( points[0], points[1],
                      points[2], points[3],
                      color);
    tft.drawPixel(points[0], points[1], pointscolor);
    tft.drawPixel(points[2], points[3], pointscolor);
}
