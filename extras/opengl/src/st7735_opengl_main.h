#ifndef TEENSY_ST7735_LINUX_ST7735_OPENGL_MAIN_H
#define TEENSY_ST7735_LINUX_ST7735_OPENGL_MAIN_H

void setup();
void loop();
extern volatile bool shouldClose;

void *arduinoThread(void *threadid);
extern int st7735_main(int argc, char** argv);
int main(int argc, char** argv);

#endif //TEENSY_ST7735_LINUX_ST7735_OPENGL_MAIN_H
