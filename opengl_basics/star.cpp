#include "star.h"

#include <GL/glew.h>

void Star::Initialize(){
    const GLfloat kVertices[] = {
         0.0f,  0.8f, 0.0f, 1.0f,
         0.17f, 0.2f, 0.0f, 1.0f,
         0.65f, 0.2f, 0.0f, 1.0f,
         0.25f, -0.1f, 0.0f, 1.0f,
         0.4f, -0.8f, 0.0f, 1.0f,
         0.0f, -0.3f, 0.0f, 1.0f,
         -0.4f, -0.8f, 0.0f, 1.0f,
         -0.25f, -0.1f, 0.0f, 1.0f,
         -0.65f, 0.2f, 0.0f, 1.0f,
         -0.17f, 0.2f, 0.0f, 1.0f,
         0.0f,  0.8f, 0.0f, 1.0f
    };

    const GLfloat kColors[] = {
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f
    };


    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kVertices), kVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &color_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kColors), kColors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

Star::~Star(){
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDeleteBuffers(1, &color_buffer_);
    glDeleteBuffers(1, &vertex_buffer_);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao_);
}

void Star::Draw(const Program &program){
    glUseProgram(program);
    glBindVertexArray(vao_);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 11);

    glBindVertexArray(0);
    glUseProgram(0);
}

void Star::Draw1(const Program &program){
    glUseProgram(program);
    glBindVertexArray(vao_);

    glDrawArrays(GL_LINE_STRIP, 0, 11);

    glBindVertexArray(0);
    glUseProgram(0);
}
