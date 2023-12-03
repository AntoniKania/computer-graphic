#ifndef CIRCLE_H
#define CIRCLE_H

#include <GL/glew.h>

#include "program.h"

class Circle{
 public:
    void Initialize();
    ~Circle();
    void DrawFilled(const Program & program);
    void Draw(const Program & program);
 private:
    GLuint vao_;
    GLuint vertex_buffer_;
    GLuint color_buffer_;
};

#endif // CIRCLE_H
