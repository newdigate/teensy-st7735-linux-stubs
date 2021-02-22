//
// Created by Moolet on 18/01/2021.
//

#include "st7735_opengl.h"

void st7735_opengl::update() {
    if (_surpressUpdate) return;
    if (_useFramebuffer && !_update_cont) return;
    if (!_needsUpdate) {
        glfwPollEvents();
        return;
    }

    unsigned long microsStart = micros();
    if (microsStart - lastUpdate < 100) {
        return;
    }
    lastUpdate = microsStart;
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
    _needsUpdate = false;
    uint microsStop = micros();
    //Serial.printf("OpenGL update: %u (%i micros)\n", _updateCount, microsStop - microsStart);
    _updateCount++;
}

void st7735_opengl::Pixel(int16_t x, int16_t y, uint16_t color) {
    uint16_t index = y * 128 + x;
    if (textureImage[index] != color) {
        textureImage[index] = color;
        _needsUpdate = true;
    }
}

int st7735_opengl::write(uint8_t c) {
    return write(&c, 1);
}

int st7735_opengl::write(const uint8_t *buffer, size_t size) {
    bool activateSurpress = !_surpressUpdate;
    if (activateSurpress) _surpressUpdate = true;

    // Lets try to handle some of the special font centering code that was done for default fonts.
    if (_center_x_text || _center_y_text ) {
        int16_t x, y;
        uint16_t strngWidth, strngHeight;
        getTextBounds((uint8_t*)buffer, size, 0, 0, &x, &y, &strngWidth, &strngHeight);
        //Serial.printf("_fontwrite bounds: %d %d %u %u\n", x, y, strngWidth, strngHeight);
        // Note we may want to play with the x ane y returned if they offset some
        if (_center_x_text && strngWidth > 0){//Avoid operations for strngWidth = 0
            cursor_x -= ((x + strngWidth) / 2);
        }
        if (_center_y_text && strngHeight > 0){//Avoid operations for strngWidth = 0
            cursor_y -= ((y + strngHeight) / 2);
        }
        _center_x_text = false;
        _center_y_text = false;
    }

    size_t cb = size;
    while (cb) {
        uint8_t c = *buffer++;
        cb--;

        if (font) {
            if (c == '\n') {
                cursor_y += font->line_space;
                if(scrollEnable && isWritingScrollArea){
                    cursor_x  = scroll_x;
                }else{
                    cursor_x  = 0;
                }
            } else {
                drawFontChar(c);
            }
        } else if (gfxFont)  {
            if (c == '\n') {
                cursor_y += (int16_t)textsize_y * gfxFont->yAdvance;
                if(scrollEnable && isWritingScrollArea){
                    cursor_x  = scroll_x;
                }else{
                    cursor_x  = 0;
                }
            } else {
                drawGFXFontChar(c);
            }
        } else {
            if (c == '\n') {
                cursor_y += textsize_y*8;
                if(scrollEnable && isWritingScrollArea){
                    cursor_x  = scroll_x;
                }else{
                    cursor_x  = 0;
                }
            } else if (c == '\r') {
                // skip em
            } else {
                if(scrollEnable && isWritingScrollArea && (cursor_y > (scroll_y+scroll_height - textsize_y*8))){
                    scrollTextArea(textsize_y*8);
                    cursor_y -= textsize_y*8;
                    cursor_x = scroll_x;
                }
                drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x, textsize_y);
                cursor_x += textsize_x*6;
                if(wrap && scrollEnable && isWritingScrollArea && (cursor_x > (scroll_x+scroll_width - textsize_x*6))){
                    cursor_y += textsize_y*8;
                    cursor_x = scroll_x;
                }
                else if (wrap && (cursor_x > (_width - textsize_x*6))) {
                    cursor_y += textsize_y*8;
                    cursor_x = 0;
                }
            }
        }
    }

    if (activateSurpress) {
        _surpressUpdate = false;
        update();
    }
    return size;
}

bool st7735_opengl::check_shader_compile_status(GLuint obj) {
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

bool st7735_opengl::check_program_link_status(GLuint obj) {
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

st7735_opengl::st7735_opengl() : st7735_opengl(false) {

}

st7735_opengl::st7735_opengl(bool drawFrame)  : st7735_opengl(drawFrame, 20) {

}

st7735_opengl::st7735_opengl(bool drawFrame, int16_t frameSize) : ST7735_t3(1,2){

    _drawFrame = drawFrame;
    _frameSize = frameSize;

    initialize_mock_arduino();
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

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    /* Create a windowed mode window and its OpenGL context */
    int width = (_drawFrame)? 128+(_frameSize*2) : 128;
    int height = (_drawFrame)? 128+(_frameSize*2) : 128;
    window = glfwCreateWindow(width, height, "ST7735_t3", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return;
    }

    //glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

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

    textureImage = new uint16_t[128*128] {0};
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
    update();
}

bool st7735_opengl::shouldClose() {
    return glfwWindowShouldClose(window);
}

void st7735_opengl::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    bool activateSurpress = !_surpressUpdate;
    if (activateSurpress) _surpressUpdate = true;

    ST7735_t3::drawCircle(x0, y0, r, color);

    if (activateSurpress) {
        _surpressUpdate = false;
        update();
    }
}

void st7735_opengl::drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color) {
    bool activateSurpress = !_surpressUpdate;
    if (activateSurpress) _surpressUpdate = true;

    ST7735_t3::drawCircleHelper(x0, y0, r, cornername, color);

    if (activateSurpress) {
        _surpressUpdate = false;
        update();
    }
}

void st7735_opengl::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    bool activateSurpress = !_surpressUpdate;
    if (activateSurpress) _surpressUpdate = true;

    ST7735_t3::fillCircle(x0, y0, r, color);

    if (activateSurpress) {
        _surpressUpdate = false;
        update();
    }
}

void
st7735_opengl::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color) {
    bool activateSurpress = !_surpressUpdate;
    if (activateSurpress) _surpressUpdate = true;

    ST7735_t3::fillCircleHelper(x0, y0, r, cornername, delta, color);

    if (activateSurpress) {
        _surpressUpdate = false;
        update();
    }
}

void
st7735_opengl::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
    bool activateSurpress = !_surpressUpdate;
    if (activateSurpress) _surpressUpdate = true;

    ST7735_t3::drawTriangle(x0, y0, x1, y1, x2, y2, color);

    if (activateSurpress) {
        _surpressUpdate = false;
        update();
    }
}

void
st7735_opengl::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
    bool activateSurpress = !_surpressUpdate;
    if (activateSurpress) _surpressUpdate = true;

    ST7735_t3::fillTriangle(x0, y0, x1, y1, x2, y2, color);

    if (activateSurpress) {
        _surpressUpdate = false;
        update();
    }
}

void st7735_opengl::drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color) {
    bool activateSurpress = !_surpressUpdate;
    if (activateSurpress) _surpressUpdate = true;

    ST7735_t3::drawRoundRect(x0, y0, w, h, radius, color);

    if (activateSurpress) {
        _surpressUpdate = false;
        update();
    }
}

void st7735_opengl::fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color) {
    bool activateSurpress = !_surpressUpdate;
    if (activateSurpress) _surpressUpdate = true;

    ST7735_t3::fillRoundRect(x0, y0, w, h, radius, color);

    if (activateSurpress) {
        _surpressUpdate = false;
        update();
    }
}

void st7735_opengl::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) {
    bool activateSurpress = !_surpressUpdate;
    if (activateSurpress) _surpressUpdate = true;

    ST7735_t3::drawBitmap(x, y, bitmap, w, h, color);

    if (activateSurpress) {
        _surpressUpdate = false;
        update();
    }
}

void st7735_opengl::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    bool activateSurpress = !_surpressUpdate;
    if (activateSurpress) _surpressUpdate = true;

    ST7735_t3::drawLine(x0, y0, x1, y1, color);

    if (activateSurpress) {
        _surpressUpdate = false;
        update();
    }
}

void st7735_opengl::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    bool activateSurpress = !_surpressUpdate;
    if (activateSurpress) _surpressUpdate = true;
    ST7735_t3::drawRect(x, y, w, h, color);
    if (activateSurpress) {
        _surpressUpdate = false;
        update();
    }
}

void st7735_opengl::drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x,
                             uint8_t size_y) {
    bool activateSurpress = !_surpressUpdate;
    if (activateSurpress) _surpressUpdate = true;
    ST7735_t3::drawChar(x, y, c, color, bg, size_x, size_y);
    if (activateSurpress) {
        _surpressUpdate = false;
        update();
    }
}

void st7735_opengl::HLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    bool activateSurpress = !_surpressUpdate;
    if (activateSurpress) _surpressUpdate = true;
    ST7735_t3::HLine(x, y, w, color);
    if (activateSurpress) {
        _surpressUpdate = false;
        update();
    }
}
void st7735_opengl::VLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    bool activateSurpress = !_surpressUpdate;
    if (activateSurpress) _surpressUpdate = true;
    ST7735_t3::VLine(x, y, h, color);
    if (activateSurpress) {
        _surpressUpdate = false;
        update();
    }
}

void st7735_opengl::fillScreen(uint16_t color) {
    bool activateSurpress = !_surpressUpdate;
    if (activateSurpress) _surpressUpdate = true;
    ST7735_t3::fillScreen(color);
    if (activateSurpress) {
        _surpressUpdate = false;
        update();
    }
}

void st7735_opengl::drawPixel(int16_t x, int16_t y, uint16_t color) {
    bool activateSurpress = !_surpressUpdate;
    if (activateSurpress) _surpressUpdate = true;
    ST7735_t3::drawPixel(x, y, color);
    if (activateSurpress) {
        _surpressUpdate = false;
        update();
    }
}

void st7735_opengl::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    bool activateSurpress = !_surpressUpdate;
    if (activateSurpress) _surpressUpdate = true;
    ST7735_t3::drawFastVLine(x, y, h, color);
    if (activateSurpress) {
        _surpressUpdate = false;
        update();
    }
}

void st7735_opengl::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    bool activateSurpress = !_surpressUpdate;
    if (activateSurpress) _surpressUpdate = true;
    ST7735_t3::fillRect(x, y, w, h, color);
    if (activateSurpress) {
        _surpressUpdate = false;
        update();
    }
}

void st7735_opengl::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    bool activateSurpress = !_surpressUpdate;
    if (activateSurpress) _surpressUpdate = true;
    ST7735_t3::drawFastHLine(x, y, w, color);
    if (activateSurpress) {
        _surpressUpdate = false;
        update();
    }
}

void st7735_opengl::drawCurve4(float delta, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, float p3x, float p3y, uint16_t color, uint16_t backgroundColor, bool drawAntialiased) {
    bool activateSurpress = !_surpressUpdate;
    if (activateSurpress) _surpressUpdate = true;
    ST7735_t3::drawCurve4(delta, p0x, p0y, p1x, p1y, p2x, p2y, p3x, p3y, color, backgroundColor, drawAntialiased);
    if (activateSurpress) {
        _surpressUpdate = false;
        update();
    }
}

void st7735_opengl::drawCurve3(float delta, float p0x, float p0y, float p1x, float p1y, float p2x, float p2y, uint16_t color, uint16_t backgroundColor, bool drawAntialiased) {
    bool activateSurpress = !_surpressUpdate;
    if (activateSurpress) _surpressUpdate = true;
    ST7735_t3::drawCurve3(delta, p0x, p0y, p1x, p1y, p2x, p2y, color, backgroundColor, drawAntialiased);
    if (activateSurpress) {
        _surpressUpdate = false;
        update();
    }
}

void st7735_opengl::drawLine(float x0, float y0, float x1, float y1, uint16_t color, uint16_t backgroundColor) {
    bool activateSurpress = !_surpressUpdate;
    if (activateSurpress) _surpressUpdate = true;
    ST7735_t3::drawLine(x0, y0, x1, y1, color, backgroundColor);
    if (activateSurpress) {
        _surpressUpdate = false;
        update();
    }
}
