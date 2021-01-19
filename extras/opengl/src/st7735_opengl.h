//
// Created by Moolet on 18/01/2021.
//

#ifndef TEENSY_ST7735_LINUX_ST7735_OPENGL_H
#define TEENSY_ST7735_LINUX_ST7735_OPENGL_H

#include <iostream>
#include "../../../src/ST7735_t3.h"
#include "../glfw/glfw3.h"
#include <OpenGL/gl3.h>
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
    u_char *textureImage;
    GLuint shader_program, vertex_shader, fragment_shader;
    GLuint texture;
    // vao and vbo handle
    unsigned int VBO, VAO, EBO;

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
        window = glfwCreateWindow(128, 128, "ST7735_t3", NULL, NULL);
        if (!window) {
            glfwTerminate();
            return;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        std::cout << "GL Version: " << (char *) glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


        int length;

        // create and compiler vertex shader
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        length = strlen(vertexShaderCode);
        glShaderSource(vertex_shader, 1, &vertexShaderCode, &length);
        glCompileShader(vertex_shader);
        if(!check_shader_compile_status(vertex_shader)) {
            glfwDestroyWindow(window);
            glfwTerminate();
            return;
        }

        // create and compiler fragment shader
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        length = strlen(fragmentShaderCode);
        glShaderSource(fragment_shader, 1, &fragmentShaderCode, &length);
        glCompileShader(fragment_shader);
        if(!check_shader_compile_status(fragment_shader)) {
            glfwDestroyWindow(window);
            glfwTerminate();
            return;
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
                1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
                1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
                -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
                -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
        };
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
        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.







        textureImage = new u_char[128*128*4] {0};


        for (int i = 0; i < 128; i++) {
            for (int j = 0; j < 128; j++) {
                if (j % 2 == 0) {
                    textureImage[i * (128 * 4) + (j * 4)] = 255;
                    textureImage[i * (128 * 4) + (j * 4) + 1] = i * 2;
                    textureImage[i * (128 * 4) + (j * 4) + 2] = j * 2;
                    textureImage[i * (128 * 4) + (j * 4) + 3] = 255;
                }
            }
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImage);
        //glGenerateMipmap(GL_TEXTURE_2D);

    }

    void Pixel(int16_t x, int16_t y, uint16_t color) {

    }
    virtual int write(uint8_t b) {
        return 0;
    }

    void loop() {
        glfwPollEvents();

        // use the shader program
        glUseProgram(shader_program);

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

    }
    bool shouldClose() {
        return glfwWindowShouldClose(window);
    }
    // helper to check and display for shader compiler errors
    bool check_shader_compile_status(GLuint obj) {
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
    bool check_program_link_status(GLuint obj) {
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

};


#endif //TEENSY_ST7735_LINUX_ST7735_OPENGL_H
