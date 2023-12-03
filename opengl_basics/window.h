#ifndef WINDOW_H
#define WINDOW_H

#include <cstring>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "triangle.h"
#include "star.h"
#include "program.h"
#include "circle.h"

class Window{
 public:
    Window(const char*, int, int);
    void Initialize(int major_gl_version, int minor_gl_version);
    void Resize(int new_width, int new_height);
    void KeyEvent(int key, int scancode, int action, int mods);
    void Run(void);
    operator GLFWwindow*(){return window_;}
 private:
    int width_;
    int height_;
    int mode_;
    const char* title_;
    GLFWwindow* window_;

    Triangle triangle_;
    Star star_;
    Circle circle_;

    Program program_;

    void InitGlfwOrDie(int major_gl_version,
                       int minor_gl_version);
    void InitGlewOrDie();
    void InitModels();
    void InitPrograms();
};


#endif // WINDOW_H

