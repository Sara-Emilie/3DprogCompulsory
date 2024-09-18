#include "Collision.h"
#include <iostream>
#include "glm/glm.hpp"

Collision::Collision()
{
}

void Collision::CollideWithWall(glm::vec3 Extentball, glm::vec3 Positionball, glm::vec3 Extentwall, glm::vec3 Positionwall, glm::vec3& v)
{
    glm::vec3 min = Positionwall - Extentwall;
    glm::vec3 max = Positionwall + Extentwall;

    glm::vec3 ballmin = Positionball;  
    glm::vec3 closestpoint = glm::clamp(ballmin, min, max);
    glm::vec3 distance = ballmin - closestpoint;

    float length = glm::length(distance);  

    if (length <= Extentball.x)
    {

        glm::vec3 normal = glm::normalize(distance);

        std::cout << length << std::endl;
        
        v = glm::reflect(v, normal);
        //v *= 0.99f;
       

    }
 
}


