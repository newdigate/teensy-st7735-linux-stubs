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
    st7735_opengl(bool drawFrame);
    st7735_opengl();

    void Pixel(int16_t x, int16_t y, uint16_t color) override;
    int write(uint8_t c) override;

    int write(const uint8_t *buffer, size_t size) override;

    void update() override;
    uint16_t *getFrameBufferPtr() override { return textureImage; };
    bool shouldClose();
    // helper to check and display for shader compiler errors
    bool check_shader_compile_status(GLuint obj);

// helper to check and display for shader linker error
    bool check_program_link_status(GLuint obj);

    void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) override;
    void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color) override;
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) override;
    void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color) override;
    void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) override;
    void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) override;
    void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color) override;
    void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color) override;
    void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) override;
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) override;
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
    void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y) override;
    void HLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;
    void VLine(int16_t x, int16_t y, int16_t h, uint16_t color) override;
    virtual void drawCurve(float delta, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, uint16_t color, uint16_t backgroundColor, bool drawAntialiased) override;
    virtual void drawCurve(float delta, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, uint16_t color, uint16_t backgroundColor, bool drawAntialiased) override;

    void fillScreen(uint16_t color) override;
    void drawPixel(int16_t x, int16_t y, uint16_t color) override;
    void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override;
    void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;

    void drawFrame(bool draw) {
        _drawFrame = draw;
    }
private:
    long lastUpdate = 0;
    bool _surpressUpdate = false;
    bool _drawFrame = false;
};


#endif //TEENSY_ST7735_LINUX_ST7735_OPENGL_H
