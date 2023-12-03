#include "circle.h"

#include <GL/glew.h>
#include <cmath>

void Circle::Initialize(){
    const int kNumVertices = 32;
    const float kRadius = 0.8f;
    GLfloat kVertices[4 * kNumVertices];
    GLfloat kColors[4 * kNumVertices];

    for (int i = 0; i < kNumVertices; ++i) {
        float angle = 2.0f * M_PI * float(i) / float(kNumVertices);
        kVertices[4 * i] = cos(angle) * kRadius;
        kVertices[4 * i + 1] = sin(angle) * kRadius;
        kVertices[4 * i + 2] = 0.0f;
        kVertices[4 * i + 3] = 1.0f;
        kColors[4 * i] = 1.0f;
        kColors[4 * i + 1] = 0.0f;
        kColors[4 * i + 2] = 0.0f;
        kColors[4 * i + 3] = 1.0f;
    }

        // 1.0f, 0.0f, 0.0f, 1.0f,
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

Circle::~Circle(){
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDeleteBuffers(1, &color_buffer_);
    glDeleteBuffers(1, &vertex_buffer_);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao_);
}

void Circle::DrawFilled(const Program &program){
    glUseProgram(program);
    glBindVertexArray(vao_);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 32);

    glBindVertexArray(0);
    glUseProgram(0);
}

void Circle::Draw(const Program &program){
    glUseProgram(program);
    glBindVertexArray(vao_);

    glDrawArrays(GL_LINE_LOOP, 0, 32);

    glBindVertexArray(0);
    glUseProgram(0);
}
