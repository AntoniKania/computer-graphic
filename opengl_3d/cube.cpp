#include <GL/glew.h>
#include <iostream>

#include "cube.h"
#include "vertices.h"

Cube::Cube(float init_velocity, float init_angle){
    velocity_ = init_velocity;
    angle_ = init_angle;
    animated_ = true;
}


void Cube::Move(float delta_t){
    if (!animated_) return;
    angle_ += delta_t * velocity_;
    if(angle_>360) angle_ -= 360;
    if(angle_<-360) angle_ += 360;
    model_matrix_.SetUnitMatrix();
    model_matrix_.RotateAboutX(angle_);
    model_matrix_.RotateAboutY(angle_);
}



void Cube::SpeedUp(){
    velocity_ *= 1.09544511501;
}

void Cube::SlowDown(){
    velocity_ /= 1.09544511501;
}

void Cube::ToggleAnimated(){
    animated_ = ! animated_;
}

void Cube::Initialize(){
    const ColorVertex kVertices[] =
    {
        { { -1.0f, -1.0f,  1.0f, 1.0f }, { 1, 1, 1, 1 } },
        { { 0, -1.0f,  1.0f, 1.0f }, { 0, 1, 1, 1 } },
        { { 1.0f, -1.0f,  1.0f, 1.0f }, { 1, 0, 1, 1 } },
        { { 1.0f, -1.0f,  -1.0f, 1.0f }, { 1, 1, 0, 1 } },
        { { 1.0f, 0,  -1.0f, 1.0f }, { 0, 1, 0, 1 } },
        { { 0, 1.0f,  -1.0f, 1.0f }, { 1, 0, 1, 1 } },
        { { 1.0f, 0,  0, 1.0f }, { 1, 1, 0, 1 } },
        { { 1.0f, 0,  1.0f, 1.0f }, { 1, 1, 1, 1 } },
        { { -1.0f, 0,  -1.0f, 1.0f }, { 1, 1, 1, 1 } },
        { { -1.0f, -1.0f,  -1.0f, 1.0f }, { 0, 1, 1, 1 } },
        { { -1.0f, 0,  0, 1.0f }, { 0, 1, 1, 1 } },
        { { -1.0f, 0,  1.0f, 1.0f }, { 1, 1, 0, 1 } }
    };

    const GLuint kIndices[] =
    {
      2,7,6, 2,6,3, 3,6,4,
      0,11,1,
      1,7,2,
      9,8,10,9,10,0, 0,10,11,
      3,4,8, 4,5,8, 3,8,9,
      9,0,3, 3,0,2,
      11,10,1,
      1,6,7,
      10,5,6, 1,10,6,
      6,5,4,
      8,5,10
    };


    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kVertices), kVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(kVertices[0]), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(kVertices[0]),
                          (GLvoid*) sizeof(kVertices[0].position));
    glEnableVertexAttribArray(1);


    glGenBuffers(1, &index_buffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(kIndices), kIndices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void Cube::Draw(const ModelProgram &program) const{

    glUseProgram(program);
    glBindVertexArray(vao_);

    program.SetModelMatrix(model_matrix_);

    glDrawElements(GL_TRIANGLES, 57, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);


}

void Cube::Right() {
    model_matrix_.RotateAboutY(4);
}

void Cube::Left() {
    model_matrix_.RotateAboutY(-4);
}

void Cube::Up() {
    model_matrix_.RotateAboutX(4);
}

void Cube::Down() {
    model_matrix_.RotateAboutX(-4);
}