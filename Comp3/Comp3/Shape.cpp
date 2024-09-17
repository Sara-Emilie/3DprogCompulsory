#include "Shape.h"
#include <iostream>

Vertex cubeVerticies[] = {
Vertex{glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec2(0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 1.0f)},
Vertex{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f,  1.0f), glm::vec2(0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 1.0f)},
Vertex{glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 0.0f)},
Vertex{glm::vec3(1.0f, -1.0f,  1.0f), glm::vec3(1.0f, -1.0f,  1.0f), glm::vec2(0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f)},
Vertex{glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec2(0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 1.0f)},
Vertex{glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec2(0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f)},
Vertex{glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(1.0f,  1.0f, -1.0f), glm::vec2(0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 1.0f)},
Vertex{glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(1.0f,  1.0f,  1.0f), glm::vec2(0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f)}
};

unsigned int cubeIndices[] = {
    0, 1, 2,
    0, 2, 3,
    0, 4, 7,
    0, 7, 3,
    3, 7, 6,
    3, 6, 2,
    2, 6, 5,
    2, 5, 1,
    1, 5, 4,
    1, 4, 0,
    4, 5, 6,
    4, 6, 7
};

//Vertex OctaheadronVertices



Shape::Shape(Type shape)
{
    if (shape == CUBE) 
    {
        //std::cout << "Render Cube" << std::endl;

        std::vector<Vertex>vert(cubeVerticies, cubeVerticies + sizeof(cubeVerticies) / sizeof(Vertex));
        std::vector<GLuint>indi(cubeIndices, cubeIndices + sizeof(cubeIndices) / sizeof(GLuint));
        this->vert = vert;
        this->indi = indi;
    }
    else if (shape == OCTAHEDRON)
    {
        //hi

    }
    else if (shape == SUBDIVIDED_OCTAHEDRON)
    {
        //hi
    }
}

std::vector<Vertex>& Shape::getvert()
{
    return this->vert;
}

std::vector<GLuint>& Shape::getindi()
{
    return this->indi;
}