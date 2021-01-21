# teensy st7735 linux stubs
[![Ubuntu-x64](https://github.com/newdigate/teensy-st7735-linux-stubs/workflows/Ubuntu-x64/badge.svg)](https://github.com/newdigate/teensy-st7735-linux-stubs/actions)
[![ubuntu-opengl](https://github.com/newdigate/teensy-st7735-linux-stubs/workflows/ubuntu-opengl/badge.svg)](https://github.com/newdigate/teensy-st7735-linux-stubs/actions?query=workflow%3Aubuntu-opengl)
[![basic-example](https://github.com/newdigate/teensy-st7735-linux-stubs/workflows/basic-example/badge.svg)](https://github.com/newdigate/teensy-st7735-linux-stubs/actions?query=workflow%3Abasic-example)

experimental library for developing visual components for st7735 display... 

be cautious: work-in-progress!!! 

## structure
* **[src](src)**: abstract st7735_t3 class for linux.
* **[extras/opengl](extras/opengl)**: implementation of st7735_t3 using opengl framebuffer

## examples
* [extras/opengl/examples/basic](https://github.com/newdigate/teensy-st7735-linux-stubs/tree/main/extras/opengl/examples/basic) 
  * modified version of [ST7735_t3/examples/graphicstest/graphicstest.ino](https://github.com/PaulStoffregen/ST7735_t3/blob/master/examples/graphicstest/graphicstest.ino)
  
    ![opengl emulator](docs/graphicstest.gif)

## installation
### st7735_t3 class
``` sh
> mkdir cmake-build-debug && cd cmake-build-debug && cmake -DCMAKE_BUILD_TYPE=Debug && sudo make install
```

### opengl st7735 emulator
``` sh
> cd extras/opengl && mkdir cmake-build-debug && cd cmake-build-debug && cmake -DCMAKE_BUILD_TYPE=Debug && sudo make install
```

## usage
### st7735_t3
* once installed, add to your CMakeLists.txt
``` cmake
find_package(teensy_st7735_linux_stubs)
include_directories(${teensy_st7735_linux_stubs_INCLUDE_DIR})
...
target_link_libraries(your_application ${teensy_st7735_linux_stubs_LIBS})
```
* include ST7735.h in your c++ code:
``` c++
#include "ST7735_t3.h"
```

### opengl st7735_t3
* once installed, add to your CMakeLists.txt
``` cmake
find_package(teensy_st7735_linux_opengl)
include_directories(${teensy_st7735_linux_opengl_INCLUDE_DIR})
...
target_link_libraries(your_application ${teensy_st7735_linux_opengl_LIBS})
```
* include st7735_opengl.h in your c++ code:
``` c++
#include "st7735_opengl.h"
#include "st7735_opengl.h"
st7735_opengl tft = st7735_opengl();

void testlines(uint16_t color) {
    tft.fillScreen(ST7735_BLACK);
    tft.loop();
    for (int16_t x=0; x < tft.width(); x+=6) {
        tft.drawLine(0, 0, x, tft.height()-1, color);
        tft.loop();
        delay(200);
}

int main() {
    initialize_mock_arduino();
    testlines(ST7735_RED);
    
    // keep window open until user closes 
    while(!tft.shouldClose()) {
        tft.loop();
    }
}
```
* 

_LIBS})
```
* include ST7735.h in your c++ code:
``` c++
#include "ST7735_t3.h"
```


## dependencies
### **[src](src)**
* [teensy-x86-stubs](https://github.com/newdigate/teensy-x86-stubs)
### **[extras/opengl](extras/opengl)** 
* [teensy-x86-stubs](https://github.com/newdigate/teensy-x86-stubs)
* [teensy-st7735-linux-stubs/src](https://github.com/newdigate/teensy-st7735-linux-stubs/src) (i.e. [src](src))
* Ubuntu specific: 
    * libglfw3, libglfw3-dev, libglu1-mesa-dev, freeglut3-dev, mesa-common-dev, libx11-dev, libxmu-dev, libxi-dev, libgl-dev, libglew-dev,
```sh
> sudo apt-get install -yq libglfw3 libglfw3-dev libglu1-mesa-dev freeglut3-dev mesa-common-dev libx11-dev libxmu-dev libxi-dev libgl-dev libglew-dev
```
* MacOS specific
    * glfw, glew
```sh
> brew install glfw
> brew install glew
```
