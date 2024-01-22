#ifndef TORUS_H
#define TORUS_H

#include "indexmodel.h"
#include "movablemodel.h"
#include "texturemodel.h"
#include "matma.h"
#include "texturecameraprogram.h"

class Torus: public IndexModel, public MovableModel, public TextureModel{
 public:
    Torus(float init_velocity=40, float init_angle=0);
    void SetInitAngle(float angle){angle_= angle;}
    void SetVelocity(float velocity){velocity_= velocity;}
    void Initialize(int n, int m, float R);
    void Draw(const TextureCameraProgram & program) const;
    void Move(float delta_t);
    void SpeedUp();
    void SlowDown();
    void ToggleAnimated();
 private:
    int n_; // mesh parameter
    int m_; // mesh parameter

    float R; //big radius

    float angle_;
    float velocity_;
    bool animated_;

};

#endif // TORUS_H
