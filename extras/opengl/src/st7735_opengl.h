//
// Created by Moolet on 18/01/2021.
//

#ifndef TEENSY_ST7735_LINUX_ST7735_OPENGL_H
#define TEENSY_ST7735_LINUX_ST7735_OPENGL_H

#include <iostream>
#include "../../../src/ST7735_t3.h"
#include "../glfw/glfw3.h"

class st7735_opengl : public ST7735_t3 {
public:
    GLFWwindow *window;

    st7735_opengl() {

        /* Initialize the library */
        if (!glfwInit()) {
            return;
        }

#ifdef __APPLE__
        /* We need to explicitly ask for a 3.2 context on OS X */
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); // comment this line in a release build!
#endif

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
        if (!window) {
            glfwTerminate();
            return;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        std::cout << "GL Version: " << (char *) glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    }

    void Pixel(int16_t x, int16_t y, uint16_t color) {

    }
    virtual int write(uint8_t b) {

    }
};


#endif //TEENSY_ST7735_LINUX_ST7735_OPENGL_H
