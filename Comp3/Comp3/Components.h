#pragma once
#include "glm/glm.hpp"
#include <vector>
#include <iostream>
#include "Mesh.h"

struct PositionComponent {
    float x[2];
    float y[2];
    float z[2];
};

struct VelocityComponent {
    float vx[2];
    float vy[2];
    float vz[2];
};

struct RadiusComponent 
{
    std::vector<float> radii; 
};

struct MeshComponent
{
    Mesh meshes[2];
};