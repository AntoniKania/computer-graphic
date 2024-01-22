#include "window.h"

#include <iostream>
#include <cstdlib>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glerror.h"

const char* kVertexShader="TextureShader.vertex.glsl";
const char* kFragmentShader="TextureShader.fragment.glsl";


const char* kEarthTextureFile="earthmap1k.tga";
const char* kMoonTextureFile="moonmap1k.tga";
const char* kStarsTextureFile="stars.tga";

const int kPlaneM = 60;
const int kPlaneN = 40;

const int kTorusM = 20;
const int kTorusN = 30;
const float kTorusR = 2;



Window::Window(const char * title, int width, int height){
    title_ = title;
    width_ = width;
    height_ = height;
    last_time_ = 0;

}

void Window::Initialize(int major_gl_version, int minor_gl_version){

    InitGlfwOrDie(major_gl_version, minor_gl_version);
    InitGlewOrDie();


    std::cout << "OpenGL initialized: OpenGL version: " << glGetString(GL_VERSION) << " GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    InitTextures();
    InitModels();
    InitPrograms();

    view_matrix_.Translate(0, -1, -10);
    SetViewMatrix();

    projection_matrix_ = Mat4::CreatePerspectiveProjectionMatrix(60, (float)width_/(float)height_, 0.1f, 100.0f);
    SetProjectionMatrix();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.25f, 0.75f, 0.35f, 0.0f);

}

void Window::InitGlfwOrDie(int major_gl_version, int minor_gl_version){
    if ( !glfwInit() ) {
        std::cerr << "ERROR: Could not initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_gl_version);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_gl_version);
#ifdef DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
    glfwWindowHint(GLFW_SAMPLES, 8);
    glEnable(GL_MULTISAMPLE);
    window_ = glfwCreateWindow(width_, height_, title_, nullptr, nullptr);
    if (!window_){
        std::cerr << "ERROR: Could not create a new rendering window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window_);
}

void Window::InitGlewOrDie(){
    GLenum glew_init_result;
    glewExperimental = GL_TRUE;
    glew_init_result = glewInit();

    if (GLEW_OK != glew_init_result) {
        std::cerr << "Glew ERROR: " << glewGetErrorString(glew_init_result) << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
#ifdef DEBUG
    if(glDebugMessageCallback){
        std::cout << "Register OpenGL debug callback " << std::endl;
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback((GLDEBUGPROC) OpenglCallbackFunction, NULL);
        GLuint unused_ids = 0;
        glDebugMessageControl(GL_DONT_CARE,
            GL_DONT_CARE,
            GL_DONT_CARE,
            0,
            &unused_ids,
            GL_FALSE);
    }
    else
        std::cout << "glDebugMessageCallback not available" << std::endl;
#endif

}

void Window::InitTextures(){
    moon_texture_.Initialize(kMoonTextureFile);
    stars_texture_.Initialize(kStarsTextureFile);
    earth_texture_.Initialize(kEarthTextureFile);
}

void Window::InitModels(){
    plane_.Initialize(kPlaneM, kPlaneN);
    plane_.SetTexture(stars_texture_);
    plane_.SetTextureUnit(2);
    plane_.SetTextureUnit(GL_TEXTURE0);

    torus_.Initialize(kTorusM, kTorusN, kTorusR);
    torus_.SetTexture(earth_texture_);
    torus_.SetTexture_Prim(moon_texture_);
    torus_.SetTextureUnit(GL_TEXTURE0);
}

void Window::InitPrograms(){
    program_.Initialize(kVertexShader, kFragmentShader);
    program_.SetTextureUnit(0);
}

void Window::SetViewMatrix() const{
    glUseProgram(program_);
    program_.SetViewMatrix(view_matrix_);
}

void Window::SetProjectionMatrix() const{
    glUseProgram(program_);
    program_.SetProjectionMatrix(projection_matrix_);
}


void Window::Resize(int new_width, int new_height){
    width_ = new_width;
    height_ = new_height;
    projection_matrix_ = Mat4::CreatePerspectiveProjectionMatrix(60, (float)width_/(float)height_, 0.1f, 100.0f);
    SetProjectionMatrix();
    glViewport(0, 0, width_, height_);
}

void Window::KeyEvent(int key, int /*scancode*/, int action, int /*mods*/){
    if(action == GLFW_PRESS){
        switch (key){
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window_, GLFW_TRUE);
            break;
            case GLFW_KEY_LEFT:
              torus_.SlowDown();
            break;
            case GLFW_KEY_RIGHT:
              torus_.SpeedUp();
            break;
            case GLFW_KEY_SPACE:
              torus_.ToggleAnimated();
            break;
            default:
            break;
        }
    }
    else if(action == GLFW_REPEAT){
        switch (key){
            case GLFW_KEY_LEFT:
              torus_.SlowDown();
            break;
            case GLFW_KEY_RIGHT:
              torus_.SpeedUp();
            break;
            default:
            break;
        }
    }
}

void Window::Run(void){
    while (!glfwWindowShouldClose(window_)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        clock_t now = clock();
        if (last_time_ == 0) last_time_ = now;
        torus_.Move((float)(now - last_time_) / CLOCKS_PER_SEC);
        last_time_ = now;

        torus_.Draw(program_);
        plane_.Draw(program_);

        glfwSwapBuffers(window_);
        glfwPollEvents();
    }

}
