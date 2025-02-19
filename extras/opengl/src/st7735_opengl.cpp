//
// Created by Moolet on 18/01/2021.
//
#include "st7735_opengl.h"

const char* vertexShaderCode = R"glsl(
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

const char* fragmentShaderCode = R"glsl(
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

bool  st7735_opengl_window::_initialized = false;
GLFWwindow *st7735_opengl_window::window = nullptr;
uint16_t st7735_opengl_window::textureImage[128*128] = {0};
GLuint st7735_opengl_window::shader_program;
GLuint st7735_opengl_window::vertex_shader;
GLuint st7735_opengl_window::fragment_shader;
GLuint st7735_opengl_window::texture;
unsigned int st7735_opengl_window::VBO;
unsigned int st7735_opengl_window::VAO;
unsigned int st7735_opengl_window::EBO;
int16_t st7735_opengl_window::_frameSize = 0;