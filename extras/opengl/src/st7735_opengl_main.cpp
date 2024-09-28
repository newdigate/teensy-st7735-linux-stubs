#include <Arduino.h>
#include "st7735_opengl.h"
#include "st7735_opengl_main.h"
volatile bool shouldClose = false;
void *arduinoThread(void *threadid) {
    long tid;
    tid = (long)threadid;
    while(!shouldClose and !arduino_should_exit) {
        loop();
        delay(1);
    }
//    cout << "Hello World! Thread ID, " << tid << endl;
    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    pthread_t thread;
    initialize_mock_arduino();
    st7735_main(argc, argv);
    setup();
    int rc = pthread_create(&thread, NULL, arduinoThread, (void *)0);
    while (!st7735_opengl_window::shouldClose() && !arduino_should_exit) {
        st7735_opengl_window::refresh();
        delay(1);
    }
    shouldClose = true;
}
