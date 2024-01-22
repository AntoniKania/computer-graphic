#ifndef WINDOW_H
#define WINDOW_H

#include <cstring>
#include <ctime>


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "torus.h"
#include "plane.h"
#include "texturecameraprogram.h"
#include "matma.h"
#include "texture.h"

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
    const char* title_;
    GLFWwindow* window_;

    Torus torus_;
    Plane plane_;

    TextureCameraProgram  program_;

    Texture stars_texture_;
    Texture earth_texture_;
    Texture moon_texture_;

    clock_t last_time_;

    Mat4 view_matrix_;
    Mat4 projection_matrix_;

    void InitModels();
    void InitPrograms();
    void InitTextures();

    void SetViewMatrix() const;
    void SetProjectionMatrix() const;

    void InitGlfwOrDie(int major_gl_version,
                       int minor_gl_version);
    void InitGlewOrDie();
};


#endif // WINDOW_H

