# teensy st7735 linux stubs
[![Ubuntu-x64](https://github.com/newdigate/teensy-st7735-linux-stubs/workflows/Ubuntu-x64/badge.svg)](https://github.com/newdigate/teensy-st7735-linux-stubs/actions)
[![basic-example](https://github.com/newdigate/teensy-st7735-linux-stubs/workflows/basic-example/badge.svg)](https://github.com/newdigate/teensy-st7735-linux-stubs/actions?query=workflow%3Abasic-example)

experimental library for developing visual components for st7735 display... 

be cautious: work-in-progress!!! 

## structure
* **[src](src)**: core classes for st7735_t3 library to compile on linux.
* **[extras/opengl](extras/opengl)**: crude emulator of st7735_t3 library using opengl framebuffer

## examples
* [extras/opengl/examples/basic](https://github.com/newdigate/teensy-st7735-linux-stubs/tree/main/extras/opengl/examples/basic) 
  * modified version of [ST7735_t3/examples/graphicstest/graphicstest.ino](https://github.com/PaulStoffregen/ST7735_t3/blob/master/examples/graphicstest/graphicstest.ino)
  
    ![opengl emulator](docs/graphicstest.gif)

