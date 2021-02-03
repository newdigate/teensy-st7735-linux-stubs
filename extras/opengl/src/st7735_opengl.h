//
// Created by Moolet on 18/01/2021.
//

#ifndef TEENSY_ST7735_LINUX_ST7735_OPENGL_H
#define TEENSY_ST7735_LINUX_ST7735_OPENGL_H

#include <iostream>
#include "ST7735_t3.h"
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#else
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif
static const char* vertexShaderCode = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
)glsl";

static const char* fragmentShaderCode = R"glsl(
#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;

void main()
{

	FragColor = texture(texture1, TexCoord);
}
)glsl";

class st7735_opengl : public ST7735_t3 {
public:
    GLFWwindow *window;
    uint16_t *textureImage;
    GLuint shader_program, vertex_shader, fragment_shader;
    GLuint texture;
    // vao and vbo handle
    unsigned int VBO, VAO, EBO;
    st7735_opengl();

    void Pixel(int16_t x, int16_t y, uint16_t color);
    int write(uint8_t c);

    int write(const uint8_t *buffer, size_t size);

    virtual void update();
    virtual uint16_t *getFrameBufferPtr() { return textureImage; };
    bool shouldClose();
    // helper to check and display for shader compiler errors
    bool check_shader_compile_status(GLuint obj);

// helper to check and display for shader linker error
    bool check_program_link_status(GLuint obj);

    virtual void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    virtual void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
    virtual void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    virtual void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
    virtual void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
    virtual void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
    virtual void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
    virtual void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
    virtual void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
    virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    virtual void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);
    virtual void HLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    void VLine(int16_t x, int16_t y, int16_t h, uint16_t color) override;

    virtual void fillScreen(uint16_t color);
    virtual void drawPixel(int16_t x, int16_t y, uint16_t color);
    virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
private:
    long lastUpdate = 0;
    bool _surpressUpdate = false;
};


#endif //TEENSY_ST7735_LINUX_ST7735_OPENGL_H
