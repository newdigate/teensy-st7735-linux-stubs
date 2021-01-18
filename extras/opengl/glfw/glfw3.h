/*************************************************************************
 * GLFW 3.0 - www.glfw.org
 * A library for OpenGL, window and input
 *------------------------------------------------------------------------
 * Copyright (c) 2002-2006 Marcus Geelnard
 * Copyright (c) 2006-2010 Camilla Berglund <elmindreda@elmindreda.org>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would
 *    be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not
 *    be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 *    distribution.
 *
 *************************************************************************/

#ifndef _glfw3_h_
#define _glfw3_h_

#ifdef __cplusplus
extern "C" {
#endif


/*************************************************************************
 * Doxygen documentation
 *************************************************************************/

/*************************************************************************
 * Global definitions
 *************************************************************************/

/* ------------------- BEGIN SYSTEM/COMPILER SPECIFIC -------------------- */

/* Please report any problems that you find with your compiler, which may
 * be solved in this section! There are several compilers that I have not
 * been able to test this file with yet.
 *
 * First: If we are we on Windows, we want a single define for it (_WIN32)
 * (Note: For Cygwin the compiler flag -mwin32 should be used, but to
 * make sure that things run smoothly for Cygwin users, we add __CYGWIN__
 * to the list of "valid Win32 identifiers", which removes the need for
 * -mwin32)
 */
#if !defined(_WIN32) && (defined(__WIN32__) || defined(WIN32) || defined(__CYGWIN__))
#define _WIN32
#endif /* _WIN32 */

/* In order for extension support to be portable, we need to define an
 * OpenGL function call method. We use the keyword APIENTRY, which is
 * defined for Win32. (Note: Windows also needs this for <GL/gl.h>)
 */
#ifndef APIENTRY
#ifdef _WIN32
#define APIENTRY __stdcall
#else
#define APIENTRY
#endif
#endif /* APIENTRY */

/* The following three defines are here solely to make some Windows-based
 * <GL/gl.h> files happy. Theoretically we could include <windows.h>, but
 * it has the major drawback of severely polluting our namespace.
 */

/* Under Windows, we need WINGDIAPI defined */
#if !defined(WINGDIAPI) && defined(_WIN32)
#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__POCC__)
   /* Microsoft Visual C++, Borland C++ Builder and Pelles C */
   #define WINGDIAPI __declspec(dllimport)
  #elif defined(__LCC__)
   /* LCC-Win32 */
   #define WINGDIAPI __stdcall
  #else
   /* Others (e.g. MinGW, Cygwin) */
   #define WINGDIAPI extern
  #endif
  #define GLFW_WINGDIAPI_DEFINED
#endif /* WINGDIAPI */

/* Some <GL/glu.h> files also need CALLBACK defined */
#if !defined(CALLBACK) && defined(_WIN32)
#if defined(_MSC_VER)
   /* Microsoft Visual C++ */
   #if (defined(_M_MRX000) || defined(_M_IX86) || defined(_M_ALPHA) || defined(_M_PPC)) && !defined(MIDL_PASS)
    #define CALLBACK __stdcall
   #else
    #define CALLBACK
   #endif
  #else
   /* Other Windows compilers */
   #define CALLBACK __stdcall
  #endif
  #define GLFW_CALLBACK_DEFINED
#endif /* CALLBACK */

/* Most GL/glu.h variants on Windows need wchar_t
 * OpenGL/gl.h blocks the definition of ptrdiff_t by glext.h on OS X */
#if !defined(GLFW_INCLUDE_NONE)
#include <stddef.h>
#endif

/* Include the chosen client API headers.
 */
#if defined(__APPLE_CC__)
#if defined(GLFW_INCLUDE_GLCOREARB)
#include <OpenGL/gl3.h>
#elif !defined(GLFW_INCLUDE_NONE)
#define GL_GLEXT_LEGACY
#include <OpenGL/gl.h>
#endif
#if defined(GLFW_INCLUDE_GLU)
#include <OpenGL/glu.h>
#endif
#else
#if defined(GLFW_INCLUDE_GLCOREARB)
     #include <GL/glcorearb.h>
   #elif defined(GLFW_INCLUDE_ES1)
     #include <GLES/gl.h>
   #elif defined(GLFW_INCLUDE_ES2)
     #include <GLES2/gl2.h>
   #elif defined(GLFW_INCLUDE_ES3)
     #include <GLES3/gl3.h>
   #elif !defined(GLFW_INCLUDE_NONE)
     #include <GL/gl.h>
   #endif
   #if defined(GLFW_INCLUDE_GLU)
     #include <GL/glu.h>
   #endif
#endif

#if defined(GLFW_DLL) && defined(_GLFW_BUILD_DLL)
/* GLFW_DLL is defined by users of glfw when compiling programs that will link
   * to the DLL version of the glfw library.  _GLFW_BUILD_DLL is defined by the
   * glfw configuration header when compiling the DLL version of the library.
   */
  #error "You must not have both GLFW_DLL and _GLFW_BUILD_DLL defined"
#endif

#if defined(_WIN32) && defined(_GLFW_BUILD_DLL)

/* We are building a Win32 DLL */
  #define GLFWAPI __declspec(dllexport)

#elif defined(_WIN32) && defined(GLFW_DLL)

/* We are calling a Win32 DLL */
  #if defined(__LCC__)
   #define GLFWAPI extern
  #else
   #define GLFWAPI __declspec(dllimport)
  #endif

#elif defined(__GNUC__) && defined(_GLFW_BUILD_DLL)

#define GLFWAPI __attribute__((visibility("default")))

#else

/* We are either building/calling a static lib or we are non-win32 */
#define GLFWAPI

#endif

/* -------------------- END SYSTEM/COMPILER SPECIFIC --------------------- */


/*************************************************************************
 * GLFW API tokens
 *************************************************************************/

#define GLFW_VERSION_MAJOR          3

#define GLFW_VERSION_MINOR          0

#define GLFW_VERSION_REVISION       4

#define GLFW_RELEASE                0

#define GLFW_PRESS                  1

#define GLFW_REPEAT                 2

/* The unknown key */
#define GLFW_KEY_UNKNOWN            -1

/* Printable keys */
#define GLFW_KEY_SPACE              32
#define GLFW_KEY_APOSTROPHE         39  /* ' */
#define GLFW_KEY_COMMA              44  /* , */
#define GLFW_KEY_MINUS              45  /* - */
#define GLFW_KEY_PERIOD             46  /* . */
#define GLFW_KEY_SLASH              47  /* / */
#define GLFW_KEY_0                  48
#define GLFW_KEY_1                  49
#define GLFW_KEY_2                  50
#define GLFW_KEY_3                  51
#define GLFW_KEY_4                  52
#define GLFW_KEY_5                  53
#define GLFW_KEY_6                  54
#define GLFW_KEY_7                  55
#define GLFW_KEY_8                  56
#define GLFW_KEY_9                  57
#define GLFW_KEY_SEMICOLON          59  /* ; */
#define GLFW_KEY_EQUAL              61  /* = */
#define GLFW_KEY_A                  65
#define GLFW_KEY_B                  66
#define GLFW_KEY_C                  67
#define GLFW_KEY_D                  68
#define GLFW_KEY_E                  69
#define GLFW_KEY_F                  70
#define GLFW_KEY_G                  71
#define GLFW_KEY_H                  72
#define GLFW_KEY_I                  73
#define GLFW_KEY_J                  74
#define GLFW_KEY_K                  75
#define GLFW_KEY_L                  76
#define GLFW_KEY_M                  77
#define GLFW_KEY_N                  78
#define GLFW_KEY_O                  79
#define GLFW_KEY_P                  80
#define GLFW_KEY_Q                  81
#define GLFW_KEY_R                  82
#define GLFW_KEY_S                  83
#define GLFW_KEY_T                  84
#define GLFW_KEY_U                  85
#define GLFW_KEY_V                  86
#define GLFW_KEY_W                  87
#define GLFW_KEY_X                  88
#define GLFW_KEY_Y                  89
#define GLFW_KEY_Z                  90
#define GLFW_KEY_LEFT_BRACKET       91  /* [ */
#define GLFW_KEY_BACKSLASH          92  /* \ */
#define GLFW_KEY_RIGHT_BRACKET      93  /* ] */
#define GLFW_KEY_GRAVE_ACCENT       96  /* ` */
#define GLFW_KEY_WORLD_1            161 /* non-US #1 */
#define GLFW_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define GLFW_KEY_ESCAPE             256
#define GLFW_KEY_ENTER              257
#define GLFW_KEY_TAB                258
#define GLFW_KEY_BACKSPACE          259
#define GLFW_KEY_INSERT             260
#define GLFW_KEY_DELETE             261
#define GLFW_KEY_RIGHT              262
#define GLFW_KEY_LEFT               263
#define GLFW_KEY_DOWN               264
#define GLFW_KEY_UP                 265
#define GLFW_KEY_PAGE_UP            266
#define GLFW_KEY_PAGE_DOWN          267
#define GLFW_KEY_HOME               268
#define GLFW_KEY_END                269
#define GLFW_KEY_CAPS_LOCK          280
#define GLFW_KEY_SCROLL_LOCK        281
#define GLFW_KEY_NUM_LOCK           282
#define GLFW_KEY_PRINT_SCREEN       283
#define GLFW_KEY_PAUSE              284
#define GLFW_KEY_F1                 290
#define GLFW_KEY_F2                 291
#define GLFW_KEY_F3                 292
#define GLFW_KEY_F4                 293
#define GLFW_KEY_F5                 294
#define GLFW_KEY_F6                 295
#define GLFW_KEY_F7                 296
#define GLFW_KEY_F8                 297
#define GLFW_KEY_F9                 298
#define GLFW_KEY_F10                299
#define GLFW_KEY_F11                300
#define GLFW_KEY_F12                301
#define GLFW_KEY_F13                302
#define GLFW_KEY_F14                303
#define GLFW_KEY_F15                304
#define GLFW_KEY_F16                305
#define GLFW_KEY_F17                306
#define GLFW_KEY_F18                307
#define GLFW_KEY_F19                308
#define GLFW_KEY_F20                309
#define GLFW_KEY_F21                310
#define GLFW_KEY_F22                311
#define GLFW_KEY_F23                312
#define GLFW_KEY_F24                313
#define GLFW_KEY_F25                314
#define GLFW_KEY_KP_0               320
#define GLFW_KEY_KP_1               321
#define GLFW_KEY_KP_2               322
#define GLFW_KEY_KP_3               323
#define GLFW_KEY_KP_4               324
#define GLFW_KEY_KP_5               325
#define GLFW_KEY_KP_6               326
#define GLFW_KEY_KP_7               327
#define GLFW_KEY_KP_8               328
#define GLFW_KEY_KP_9               329
#define GLFW_KEY_KP_DECIMAL         330
#define GLFW_KEY_KP_DIVIDE          331
#define GLFW_KEY_KP_MULTIPLY        332
#define GLFW_KEY_KP_SUBTRACT        333
#define GLFW_KEY_KP_ADD             334
#define GLFW_KEY_KP_ENTER           335
#define GLFW_KEY_KP_EQUAL           336
#define GLFW_KEY_LEFT_SHIFT         340
#define GLFW_KEY_LEFT_CONTROL       341
#define GLFW_KEY_LEFT_ALT           342
#define GLFW_KEY_LEFT_SUPER         343
#define GLFW_KEY_RIGHT_SHIFT        344
#define GLFW_KEY_RIGHT_CONTROL      345
#define GLFW_KEY_RIGHT_ALT          346
#define GLFW_KEY_RIGHT_SUPER        347
#define GLFW_KEY_MENU               348
#define GLFW_KEY_LAST               GLFW_KEY_MENU

#define GLFW_MOD_SHIFT           0x0001

#define GLFW_MOD_CONTROL         0x0002

#define GLFW_MOD_ALT             0x0004

#define GLFW_MOD_SUPER           0x0008

#define GLFW_MOUSE_BUTTON_1         0
#define GLFW_MOUSE_BUTTON_2         1
#define GLFW_MOUSE_BUTTON_3         2
#define GLFW_MOUSE_BUTTON_4         3
#define GLFW_MOUSE_BUTTON_5         4
#define GLFW_MOUSE_BUTTON_6         5
#define GLFW_MOUSE_BUTTON_7         6
#define GLFW_MOUSE_BUTTON_8         7
#define GLFW_MOUSE_BUTTON_LAST      GLFW_MOUSE_BUTTON_8
#define GLFW_MOUSE_BUTTON_LEFT      GLFW_MOUSE_BUTTON_1
#define GLFW_MOUSE_BUTTON_RIGHT     GLFW_MOUSE_BUTTON_2
#define GLFW_MOUSE_BUTTON_MIDDLE    GLFW_MOUSE_BUTTON_3

#define GLFW_JOYSTICK_1             0
#define GLFW_JOYSTICK_2             1
#define GLFW_JOYSTICK_3             2
#define GLFW_JOYSTICK_4             3
#define GLFW_JOYSTICK_5             4
#define GLFW_JOYSTICK_6             5
#define GLFW_JOYSTICK_7             6
#define GLFW_JOYSTICK_8             7
#define GLFW_JOYSTICK_9             8
#define GLFW_JOYSTICK_10            9
#define GLFW_JOYSTICK_11            10
#define GLFW_JOYSTICK_12            11
#define GLFW_JOYSTICK_13            12
#define GLFW_JOYSTICK_14            13
#define GLFW_JOYSTICK_15            14
#define GLFW_JOYSTICK_16            15
#define GLFW_JOYSTICK_LAST          GLFW_JOYSTICK_16

#define GLFW_NOT_INITIALIZED        0x00010001

#define GLFW_NO_CURRENT_CONTEXT     0x00010002

#define GLFW_INVALID_ENUM           0x00010003

#define GLFW_INVALID_VALUE          0x00010004

#define GLFW_OUT_OF_MEMORY          0x00010005

#define GLFW_API_UNAVAILABLE        0x00010006

#define GLFW_VERSION_UNAVAILABLE    0x00010007

#define GLFW_PLATFORM_ERROR         0x00010008

#define GLFW_FORMAT_UNAVAILABLE     0x00010009

#define GLFW_FOCUSED                0x00020001
#define GLFW_ICONIFIED              0x00020002
#define GLFW_RESIZABLE              0x00020003
#define GLFW_VISIBLE                0x00020004
#define GLFW_DECORATED              0x00020005

#define GLFW_RED_BITS               0x00021001
#define GLFW_GREEN_BITS             0x00021002
#define GLFW_BLUE_BITS              0x00021003
#define GLFW_ALPHA_BITS             0x00021004
#define GLFW_DEPTH_BITS             0x00021005
#define GLFW_STENCIL_BITS           0x00021006
#define GLFW_ACCUM_RED_BITS         0x00021007
#define GLFW_ACCUM_GREEN_BITS       0x00021008
#define GLFW_ACCUM_BLUE_BITS        0x00021009
#define GLFW_ACCUM_ALPHA_BITS       0x0002100A
#define GLFW_AUX_BUFFERS            0x0002100B
#define GLFW_STEREO                 0x0002100C
#define GLFW_SAMPLES                0x0002100D
#define GLFW_SRGB_CAPABLE           0x0002100E
#define GLFW_REFRESH_RATE           0x0002100F

#define GLFW_CLIENT_API             0x00022001
#define GLFW_CONTEXT_VERSION_MAJOR  0x00022002
#define GLFW_CONTEXT_VERSION_MINOR  0x00022003
#define GLFW_CONTEXT_REVISION       0x00022004
#define GLFW_CONTEXT_ROBUSTNESS     0x00022005
#define GLFW_OPENGL_FORWARD_COMPAT  0x00022006
#define GLFW_OPENGL_DEBUG_CONTEXT   0x00022007
#define GLFW_OPENGL_PROFILE         0x00022008

#define GLFW_OPENGL_API             0x00030001
#define GLFW_OPENGL_ES_API          0x00030002

#define GLFW_NO_ROBUSTNESS                   0
#define GLFW_NO_RESET_NOTIFICATION  0x00031001
#define GLFW_LOSE_CONTEXT_ON_RESET  0x00031002

#define GLFW_OPENGL_ANY_PROFILE              0
#define GLFW_OPENGL_CORE_PROFILE    0x00032001
#define GLFW_OPENGL_COMPAT_PROFILE  0x00032002

#define GLFW_CURSOR                 0x00033001
#define GLFW_STICKY_KEYS            0x00033002
#define GLFW_STICKY_MOUSE_BUTTONS   0x00033003

#define GLFW_CURSOR_NORMAL          0x00034001
#define GLFW_CURSOR_HIDDEN          0x00034002
#define GLFW_CURSOR_DISABLED        0x00034003

#define GLFW_CONNECTED              0x00040001
#define GLFW_DISCONNECTED           0x00040002


/*************************************************************************
 * GLFW API types
 *************************************************************************/

typedef void (*GLFWglproc)(void);

typedef struct GLFWmonitor GLFWmonitor;

typedef struct GLFWwindow GLFWwindow;

typedef void (* GLFWerrorfun)(int,const char*);

typedef void (* GLFWwindowposfun)(GLFWwindow*,int,int);

typedef void (* GLFWwindowsizefun)(GLFWwindow*,int,int);

typedef void (* GLFWwindowclosefun)(GLFWwindow*);

typedef void (* GLFWwindowrefreshfun)(GLFWwindow*);

typedef void (* GLFWwindowfocusfun)(GLFWwindow*,int);

typedef void (* GLFWwindowiconifyfun)(GLFWwindow*,int);

typedef void (* GLFWframebuffersizefun)(GLFWwindow*,int,int);

typedef void (* GLFWmousebuttonfun)(GLFWwindow*,int,int,int);

typedef void (* GLFWcursorposfun)(GLFWwindow*,double,double);

typedef void (* GLFWcursorenterfun)(GLFWwindow*,int);

typedef void (* GLFWscrollfun)(GLFWwindow*,double,double);

typedef void (* GLFWkeyfun)(GLFWwindow*,int,int,int,int);

typedef void (* GLFWcharfun)(GLFWwindow*,unsigned int);

typedef void (* GLFWmonitorfun)(GLFWmonitor*,int);

typedef struct GLFWvidmode
{
    int width;
    int height;
    int redBits;
    int greenBits;
    int blueBits;
    int refreshRate;
} GLFWvidmode;

typedef struct GLFWgammaramp
{
    unsigned short* red;
    unsigned short* green;
    unsigned short* blue;
    unsigned int size;
} GLFWgammaramp;


/*************************************************************************
 * GLFW API functions
 *************************************************************************/

GLFWAPI int glfwInit(void);

GLFWAPI void glfwTerminate(void);

GLFWAPI void glfwGetVersion(int* major, int* minor, int* rev);

GLFWAPI const char* glfwGetVersionString(void);

GLFWAPI GLFWerrorfun glfwSetErrorCallback(GLFWerrorfun cbfun);

GLFWAPI GLFWmonitor** glfwGetMonitors(int* count);

GLFWAPI GLFWmonitor* glfwGetPrimaryMonitor(void);

GLFWAPI void glfwGetMonitorPos(GLFWmonitor* monitor, int* xpos, int* ypos);

GLFWAPI void glfwGetMonitorPhysicalSize(GLFWmonitor* monitor, int* width, int* height);

GLFWAPI const char* glfwGetMonitorName(GLFWmonitor* monitor);

GLFWAPI GLFWmonitorfun glfwSetMonitorCallback(GLFWmonitorfun cbfun);

GLFWAPI const GLFWvidmode* glfwGetVideoModes(GLFWmonitor* monitor, int* count);

GLFWAPI const GLFWvidmode* glfwGetVideoMode(GLFWmonitor* monitor);

GLFWAPI void glfwSetGamma(GLFWmonitor* monitor, float gamma);

GLFWAPI const GLFWgammaramp* glfwGetGammaRamp(GLFWmonitor* monitor);

GLFWAPI void glfwSetGammaRamp(GLFWmonitor* monitor, const GLFWgammaramp* ramp);

GLFWAPI void glfwDefaultWindowHints(void);

GLFWAPI void glfwWindowHint(int target, int hint);

GLFWAPI GLFWwindow* glfwCreateWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);

GLFWAPI void glfwDestroyWindow(GLFWwindow* window);

GLFWAPI int glfwWindowShouldClose(GLFWwindow* window);

GLFWAPI void glfwSetWindowShouldClose(GLFWwindow* window, int value);

GLFWAPI void glfwSetWindowTitle(GLFWwindow* window, const char* title);

GLFWAPI void glfwGetWindowPos(GLFWwindow* window, int* xpos, int* ypos);

GLFWAPI void glfwSetWindowPos(GLFWwindow* window, int xpos, int ypos);

GLFWAPI void glfwGetWindowSize(GLFWwindow* window, int* width, int* height);

GLFWAPI void glfwSetWindowSize(GLFWwindow* window, int width, int height);

GLFWAPI void glfwGetFramebufferSize(GLFWwindow* window, int* width, int* height);

GLFWAPI void glfwIconifyWindow(GLFWwindow* window);

GLFWAPI void glfwRestoreWindow(GLFWwindow* window);

GLFWAPI void glfwShowWindow(GLFWwindow* window);

GLFWAPI void glfwHideWindow(GLFWwindow* window);

GLFWAPI GLFWmonitor* glfwGetWindowMonitor(GLFWwindow* window);

GLFWAPI int glfwGetWindowAttrib(GLFWwindow* window, int attrib);

GLFWAPI void glfwSetWindowUserPointer(GLFWwindow* window, void* pointer);

GLFWAPI void* glfwGetWindowUserPointer(GLFWwindow* window);

GLFWAPI GLFWwindowposfun glfwSetWindowPosCallback(GLFWwindow* window, GLFWwindowposfun cbfun);

GLFWAPI GLFWwindowsizefun glfwSetWindowSizeCallback(GLFWwindow* window, GLFWwindowsizefun cbfun);

GLFWAPI GLFWwindowclosefun glfwSetWindowCloseCallback(GLFWwindow* window, GLFWwindowclosefun cbfun);

GLFWAPI GLFWwindowrefreshfun glfwSetWindowRefreshCallback(GLFWwindow* window, GLFWwindowrefreshfun cbfun);

GLFWAPI GLFWwindowfocusfun glfwSetWindowFocusCallback(GLFWwindow* window, GLFWwindowfocusfun cbfun);

GLFWAPI GLFWwindowiconifyfun glfwSetWindowIconifyCallback(GLFWwindow* window, GLFWwindowiconifyfun cbfun);

GLFWAPI GLFWframebuffersizefun glfwSetFramebufferSizeCallback(GLFWwindow* window, GLFWframebuffersizefun cbfun);

GLFWAPI void glfwPollEvents(void);

GLFWAPI void glfwWaitEvents(void);

GLFWAPI int glfwGetInputMode(GLFWwindow* window, int mode);

GLFWAPI void glfwSetInputMode(GLFWwindow* window, int mode, int value);

GLFWAPI int glfwGetKey(GLFWwindow* window, int key);

GLFWAPI int glfwGetMouseButton(GLFWwindow* window, int button);

GLFWAPI void glfwGetCursorPos(GLFWwindow* window, double* xpos, double* ypos);

GLFWAPI void glfwSetCursorPos(GLFWwindow* window, double xpos, double ypos);

GLFWAPI GLFWkeyfun glfwSetKeyCallback(GLFWwindow* window, GLFWkeyfun cbfun);

GLFWAPI GLFWcharfun glfwSetCharCallback(GLFWwindow* window, GLFWcharfun cbfun);

GLFWAPI GLFWmousebuttonfun glfwSetMouseButtonCallback(GLFWwindow* window, GLFWmousebuttonfun cbfun);

GLFWAPI GLFWcursorposfun glfwSetCursorPosCallback(GLFWwindow* window, GLFWcursorposfun cbfun);

GLFWAPI GLFWcursorenterfun glfwSetCursorEnterCallback(GLFWwindow* window, GLFWcursorenterfun cbfun);

GLFWAPI GLFWscrollfun glfwSetScrollCallback(GLFWwindow* window, GLFWscrollfun cbfun);

GLFWAPI int glfwJoystickPresent(int joy);

GLFWAPI const float* glfwGetJoystickAxes(int joy, int* count);

GLFWAPI const unsigned char* glfwGetJoystickButtons(int joy, int* count);

GLFWAPI const char* glfwGetJoystickName(int joy);

GLFWAPI void glfwSetClipboardString(GLFWwindow* window, const char* string);

GLFWAPI const char* glfwGetClipboardString(GLFWwindow* window);

GLFWAPI double glfwGetTime(void);

GLFWAPI void glfwSetTime(double time);

GLFWAPI void glfwMakeContextCurrent(GLFWwindow* window);

GLFWAPI GLFWwindow* glfwGetCurrentContext(void);

GLFWAPI void glfwSwapBuffers(GLFWwindow* window);

GLFWAPI void glfwSwapInterval(int interval);

GLFWAPI int glfwExtensionSupported(const char* extension);

GLFWAPI GLFWglproc glfwGetProcAddress(const char* procname);


/*************************************************************************
 * Global definition cleanup
 *************************************************************************/

/* ------------------- BEGIN SYSTEM/COMPILER SPECIFIC -------------------- */

#ifdef GLFW_WINGDIAPI_DEFINED
#undef WINGDIAPI
  #undef GLFW_WINGDIAPI_DEFINED
#endif

#ifdef GLFW_CALLBACK_DEFINED
#undef CALLBACK
  #undef GLFW_CALLBACK_DEFINED
#endif

/* -------------------- END SYSTEM/COMPILER SPECIFIC --------------------- */


#ifdef __cplusplus
}
#endif

#endif /* _glfw3_h_ */