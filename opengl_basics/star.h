#ifndef STAR_H
#define STAR_H

#include <GL/glew.h>

#include "program.h"

class Star{
 public:
    void Initialize();
    ~Star();
    void Draw(const Program & program);
    void Draw1(const Program & program);
 private:
    GLuint vao_;
    GLuint vertex_buffer_;
    GLuint color_buffer_;
};

#endif // STAR_H
