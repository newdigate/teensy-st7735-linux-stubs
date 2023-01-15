//
// Created by Moolet on 18/01/2021.
//

#ifndef TEENSY_ST7735_LINUX_ST7735_OPENGL_H
#define TEENSY_ST7735_LINUX_ST7735_OPENGL_H

#include <iostream>
#include <Arduino.h>
#include "View.h"
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

class st7735_opengl_window {
public:
    static bool _initialized;
    static GLFWwindow *window;
    static uint16_t textureImage[128*128];
    static GLuint shader_program, vertex_shader, fragment_shader;
    static GLuint texture;
    static int16_t _frameSize;
    // vao and vbo handle
    static unsigned int VBO, VAO, EBO;
    static inline bool _drawFrame = false;

    static bool InitOpenGL(int16_t frameSize, bool drawFrame = false, GLFWkeyfun key_callback = nullptr, GLFWcharfun character_callback = nullptr) {
        _frameSize = frameSize;
        _drawFrame = drawFrame;
/* Initialize the library */
        if (!glfwInit()) {
            return false;
        }

    #ifdef __APPLE__
        /* We need to explicitly ask for a 3.2 context on OS X */
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); // comment this line in a release build!
    #endif

        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);

        /* Create a windowed mode window and its OpenGL context */
        int width = (_drawFrame)? 128+(_frameSize*2) : 128;
        int height = (_drawFrame)? 128+(_frameSize*2) : 128;
        st7735_opengl_window::window = glfwCreateWindow(width, height, "ST7735_t3", NULL, NULL);
        if (!st7735_opengl_window::window) {
            glfwTerminate();
            return false;
        }

        //glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        glfwSetWindowCloseCallback(st7735_opengl_window::window, collectWindowClose);

        if (key_callback != nullptr)
            glfwSetKeyCallback(st7735_opengl_window::window, key_callback);

        if (character_callback != nullptr)
            glfwSetCharCallback(st7735_opengl_window::window, character_callback);

        /* Make the window's context current */
        glfwMakeContextCurrent(st7735_opengl_window::window);

        std::cout << "GL Version: " << (char *) glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


        int length;

        // create and compiler vertex shader
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        length = strlen(vertexShaderCode);
        glShaderSource(vertex_shader, 1, &vertexShaderCode, &length);
        glCompileShader(vertex_shader);
        if(!check_shader_compile_status(vertex_shader)) {
            glfwDestroyWindow(st7735_opengl_window::window);
            glfwTerminate();
            return false;
        }

        // create and compiler fragment shader
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        length = strlen(fragmentShaderCode);
        glShaderSource(fragment_shader, 1, &fragmentShaderCode, &length);
        glCompileShader(fragment_shader);
        if(!check_shader_compile_status(fragment_shader)) {
            glfwDestroyWindow(st7735_opengl_window::window);
            glfwTerminate();
            return false;
        }

        // create program
        shader_program = glCreateProgram();

        // attach shaders
        glAttachShader(shader_program, vertex_shader);
        glAttachShader(shader_program, fragment_shader);

        // link the program and check for errors
        glLinkProgram(shader_program);
        check_program_link_status(shader_program);

        float vertices[] = {
                // positions          // colors           // texture coords
                1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // top right
                1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // bottom right
                -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // bottom left
                -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f  // top left
        };
        if (_drawFrame) {
            float multiplier = 128.0f/(128.0f + 2.0f * _frameSize);
            vertices[0] *= multiplier;
            vertices[1] *= multiplier;
            vertices[2] *= multiplier;

            vertices[8] *= multiplier;
            vertices[9] *= multiplier;
            vertices[10] *= multiplier;

            vertices[16] *= multiplier;
            vertices[17] *= multiplier;
            vertices[18] *= multiplier;

            vertices[24] *= multiplier;
            vertices[25] *= multiplier;
            vertices[26] *= multiplier;
        }
        unsigned int indices[] = {
                0, 1, 3, // first triangle
                1, 2, 3  // second triangle
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);


        // load and create a texture
        // -------------------------

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // load image, create texture and generate mipmaps

    //    textureImage = new  {0};
        /*
            uint16_t count = 0;
            for (int i = 0; i < 128; i++) {
                for (int j = 0; j < 128; j++) {
                    count ++;
                    if (j % 2 == 0) {
                        textureImage[i * 128 + j ] = 0xFFFF;
                    }
                }
            }
        */
        _initialized = true;
        return true;
    }

    static inline bool shouldClose() {
        if (!_initialized) return false;

        return glfwWindowShouldClose(window);
    }
    
    static void refresh() {
        if (!_initialized) return;
        /*if (_surpressUpdate) return;
        if (_useFramebuffer && !_update_cont) return;
        if (!_needsUpdate) {
            glfwPollEvents();
            return;
        }
    */
        //unsigned long microsStart = micros();
        /*
        if (microsStart - lastUpdate < 100) {
            return;
        }
        */
        //lastUpdate = microsStart;
        glfwPollEvents();

        // use the shader program
        glUseProgram(shader_program);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 128, 128, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, textureImage);
        // render
        // ------
        glClearColor(0.2f, 0.3f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind Texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // render container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
        //int microsStop = micros();
        //Serial.printf("OpenGL update: %u (%i micros)\n", _updateCount, microsStop - microsStart);
    }
    // helper to check and display for shader compiler errors
    static bool check_shader_compile_status(GLuint obj) {
        GLint status;
        glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
        if(status == GL_FALSE) {
            GLint length;
            glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &length);
            char log[length+1];
            glGetShaderInfoLog(obj, length, &length, &log[0]);
            std::cerr << &log[0];
            return false;
        }
        return true;
    }

    // helper to check and display for shader linker error
    static bool check_program_link_status(GLuint obj) {
        GLint status;
        glGetProgramiv(obj, GL_LINK_STATUS, &status);
        if(status == GL_FALSE) {
            GLint length;
            glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &length);
            char log[length+1];
            glGetProgramInfoLog(obj, length, &length, &log[0]);
            std::cerr << &log[0];
            return false;
        }
        return true;
    }
    
    static void collectWindowClose(GLFWwindow* window) {
        arduino_should_exit = true;
    }
};

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

template <typename TEncoder, typename TButton>
class st7735_opengl : public View {
public:
    static TEncoder *_encoderUpDown;
    static TEncoder *_encoderLeftRight;
    static TButton *_button;

    st7735_opengl() : st7735_opengl(false) {
    }

    st7735_opengl(bool drawFrame) : st7735_opengl(drawFrame, 20) {
    }

    st7735_opengl(bool drawFrame, int16_t frameSize, TEncoder *encoderUpDown = nullptr, TEncoder *encoderLeftRight = nullptr, TButton *button = nullptr): View(128, 128){
        _encoderLeftRight = encoderLeftRight;
        _encoderUpDown = encoderUpDown;
        _button = button;
        if (st7735_opengl_window::InitOpenGL(frameSize, drawFrame, key_callback, character_callback))
            return;
    }

    virtual ~st7735_opengl() {
    }

    void Pixel(int16_t x, int16_t y, uint16_t color) override {
        uint16_t index = y * 128 + x;
        if (st7735_opengl_window::textureImage[index] != color) {
            st7735_opengl_window::textureImage[index] = color;
        }
    }

    void updateScreen(void) override { }

    void readRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t *pcolors) override {
        // Use our Origin.
        x+=_originx;
        y+=_originy;
        //BUGBUG:: Should add some validation of X and Y

        if (_useFramebuffer) {
            uint16_t * _pfbtft = st7735_opengl_window::textureImage;
            uint16_t * pfbPixel_row = &_pfbtft[ y*_width + x];
            for (;h>0; h--) {
                uint16_t * pfbPixel = pfbPixel_row;
                for (int i = 0 ;i < w; i++) {
                    *pcolors++ = *pfbPixel++;
                }
                pfbPixel_row += _width;
            }
            return;
        }
    }
    
    void writeRect(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *pcolors) override {
        x += _originx;
        y += _originy;

        uint16_t x_clip_left = 0;  // How many entries at start of colors to skip at start of row
        uint16_t x_clip_right = 0;    // how many color entries to skip at end of row for clipping
        // Rectangular clipping

        // See if the whole thing out of bounds...
        if ((x >= _displayclipx2) || (y >= _displayclipy2)) return;
        if (((x + w) <= _displayclipx1) || ((y + h) <= _displayclipy1)) return;

        // In these cases you can not do simple clipping, as we need to synchronize the colors array with the
        // We can clip the height as when we get to the last visible we don't have to go any farther.
        // also maybe starting y as we will advance the color array.
        if (y < _displayclipy1) {
            int dy = (_displayclipy1 - y);
            h -= dy;
            pcolors += (dy * w); // Advance color array to
            y = _displayclipy1;
        }
        if ((y + h - 1) >= _displayclipy2) h = _displayclipy2 - y;
        // For X see how many items in color array to skip at start of row and likewise end of row
        if (x < _displayclipx1) {
            x_clip_left = _displayclipx1 - x;
            w -= x_clip_left;
            x = _displayclipx1;
        }

        if ((x + w - 1) >= _displayclipx2) {
            x_clip_right = w;
            w = _displayclipx2 - x;
            x_clip_right -= w;
        }

        if (_useFramebuffer) {
            uint16_t *_pfbtft = st7735_opengl_window::textureImage;
            uint16_t *pfbPixel_row = &_pfbtft[y * _width + x];
            for (; h > 0; h--) {
                uint16_t *pfbPixel = pfbPixel_row;
                pcolors += x_clip_left;
                for (int i = 0; i < w; i++) {
                    *pfbPixel++ = *pcolors++;
                }
                pfbPixel_row += _width;
                pcolors += x_clip_right;

            }
            return;
        }
    }



    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (action == GLFW_RELEASE) {
            switch(key) {
                case GLFW_KEY_SPACE: {
                    if (_button != nullptr) {
                        _button->setState(false);
                    }
                    break;
                }
            }
        } else if (action == GLFW_PRESS)
        {
            switch(key) {
                case GLFW_KEY_SPACE: {
                    if (_button != nullptr) {
                        _button->setState(true);
                    }
                    break;
                }
                case GLFW_KEY_UP: {
                    if (_encoderUpDown != nullptr) {
                        _encoderUpDown->increase(-4);
                    }
                    break;
                }
                case GLFW_KEY_DOWN: {
                    if (_encoderUpDown != nullptr) {
                        _encoderUpDown->increase(4);
                    }
                    break;
                }
                case GLFW_KEY_LEFT: {
                    if (_encoderLeftRight != nullptr) {
                        _encoderLeftRight->increase(-4);
                    }
                    break;
                }
                case GLFW_KEY_RIGHT: {
                    if (_encoderUpDown != nullptr) {
                        _encoderLeftRight->increase(4);
                    }
                    break;
                }
                case GLFW_KEY_ENTER: {
                    char *s = new char[_textCharacterInput.length()];
                    memcpy(s, _textCharacterInput.c_str(), _textCharacterInput.length());
                    Serial.queueSimulatedCharacterInput(s, _textCharacterInput.length());
                    delete [] s;

                    char c = '\n';
                    Serial.queueSimulatedCharacterInput(&c, 1);

                    _textCharacterInput.clear();

                    break;
                }
                default:
                    break;
            }
        }
    }

    static void character_callback(GLFWwindow* window, unsigned int codepoint) {
        char c = (char) codepoint;
        _textCharacterInput += c;
    }

private:
    static std::string _textCharacterInput;

};

template <typename TEncoder, typename TButton> 
std::string st7735_opengl<TEncoder, TButton>::_textCharacterInput;
template <typename TEncoder, typename TButton> 
TEncoder *st7735_opengl<TEncoder, TButton>::_encoderUpDown = nullptr;
template <typename TEncoder, typename TButton> 
TEncoder *st7735_opengl<TEncoder, TButton>::_encoderLeftRight = nullptr;
template <typename TEncoder, typename TButton> 
TButton *st7735_opengl<TEncoder, TButton>::_button = nullptr;

class NoEncoder {
public:
    void increase(int32_t value) {}
};

class NoButton {
public:
    void setState(bool value) {}
};

class st7735_opengl_noinput : public st7735_opengl<NoEncoder, NoButton> {
public:
    st7735_opengl_noinput(bool drawFrame) : st7735_opengl<NoEncoder, NoButton>(drawFrame) {}
    st7735_opengl_noinput(bool drawFrame, int16_t frameSize) : st7735_opengl<NoEncoder, NoButton>(drawFrame, frameSize, nullptr, nullptr, nullptr) {}
    st7735_opengl_noinput() : st7735_opengl<NoEncoder, NoButton>() {}
};
#endif //TEENSY_ST7735_LINUX_ST7735_OPENGL_H
