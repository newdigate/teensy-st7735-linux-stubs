//
// Created by Moolet on 18/01/2021.
//

#ifndef TEENSY_ST7735_LINUX_ST7735_OPENGL_H
#define TEENSY_ST7735_LINUX_ST7735_OPENGL_H

#include <iostream>
#include "ST7735_t3.h"
#include <Encoder.h>
#include <Bounce2.h>
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
    static GLFWwindow *window;
    static uint16_t textureImage[128*128];
    static GLuint shader_program, vertex_shader, fragment_shader;
    static GLuint texture;
    static int16_t _frameSize;
    // vao and vbo handle
    static unsigned int VBO, VAO, EBO;
    static Encoder *_encoderUpDown;
    static Encoder *_encoderLeftRight;
    static Bounce2::Button *_button;

    st7735_opengl(bool drawFrame);
    st7735_opengl(bool drawFrame, int16_t frameSize, Encoder *encoderUpDown = nullptr, Encoder *encoderLeftRight = nullptr, Bounce2::Button *button=nullptr);
    st7735_opengl();

    void Pixel(int16_t x, int16_t y, uint16_t color) override;
    int write(uint8_t c) override;

    int write(const uint8_t *buffer, size_t size) override;

    static bool shouldClose();
    // helper to check and display for shader compiler errors
    bool check_shader_compile_status(GLuint obj);

// helper to check and display for shader linker error
    bool check_program_link_status(GLuint obj);

    void  updateScreen(void) override;

    void readRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *pcolors) override;

    void writeRect(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *pcolors) override;

    static void refresh(void);
    static void collectWindowClose(GLFWwindow* window);
private:
    bool _drawFrame = false;
    static string _textCharacterInput;
    static bool _initialized;
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void character_callback(GLFWwindow* window, unsigned int codepoint);

};


#endif //TEENSY_ST7735_LINUX_ST7735_OPENGL_H
