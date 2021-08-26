#ifndef TEENSY_ST7735_LINUX_ST7735_OPENGL_MAIN_H
#define TEENSY_ST7735_LINUX_ST7735_OPENGL_MAIN_H

#include "st7735_opengl.h"
#include <unistd.h>

void setup();
void loop();
volatile bool shouldClose = false;

void *arduinoThread(void *threadid) {
    long tid;
    tid = (long)threadid;
    while(!shouldClose) {
        loop();
        usleep(1000);
    }
//    cout << "Hello World! Thread ID, " << tid << endl;
    pthread_exit(NULL);
}
extern int st7735_main(int argc, char** argv);

int main(int argc, char** argv) {
    pthread_t thread;
    initialize_mock_arduino();
    st7735_main(argc, argv);
    setup();
    int rc = pthread_create(&thread, NULL, arduinoThread, (void *)0);
    while (!st7735_opengl::shouldClose()) {
        st7735_opengl::refresh();
        usleep(1000);
    }
    shouldClose = true;
}

#endif //TEENSY_ST7735_LINUX_ST7735_OPENGL_MAIN_H
