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

Vertex octaheadronVertices[] =
{
Vertex{ glm::vec3(0.0f, 0.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) },
Vertex{ glm::vec3(1.0f, 0.0f,  0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) },
Vertex{ glm::vec3(0.0f, 1.0f,  0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) },
Vertex{ glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) },
Vertex{ glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
Vertex{ glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
};

GLuint octaheadronIndices[] = {
    0, 1, 2,
    0, 1, 4,
    0, 3, 2,
    0, 3, 4,
    5, 1, 2,
    5, 1, 4,
    5, 3, 2,
    5, 3, 4
};

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
        std::vector<Vertex>vert(octaheadronVertices, octaheadronVertices + sizeof(octaheadronVertices) / sizeof(Vertex));
        std::vector<GLuint>indi(octaheadronIndices, octaheadronIndices + sizeof(octaheadronIndices) / sizeof(GLuint));
        this->vert = vert;
        this->indi = indi;

    }
    else if (shape == SUBDIVIDED_OCTAHEDRON)
    {
        std::vector<Vertex>vert(cubeVerticies, cubeVerticies + sizeof(cubeVerticies) / sizeof(Vertex));
        std::vector<GLuint>indi(cubeIndices, cubeIndices + sizeof(cubeIndices) / sizeof(GLuint));
        this->vert = vert;
        this->indi = indi;

        SubDivideOcta(4); //TODO MAKE N CHANGEABLE
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

void Shape::SubDivideOcta(int n)
{
    this->indi.clear();
    SubDivide(octaheadronVertices[0].pos, octaheadronVertices[1].pos, octaheadronVertices[2].pos, n);
    SubDivide(octaheadronVertices[0].pos, octaheadronVertices[1].pos, octaheadronVertices[4].pos, n);
    SubDivide(octaheadronVertices[0].pos, octaheadronVertices[3].pos, octaheadronVertices[2].pos, n);
    SubDivide(octaheadronVertices[0].pos, octaheadronVertices[3].pos, octaheadronVertices[4].pos, n);
    SubDivide(octaheadronVertices[5].pos, octaheadronVertices[1].pos, octaheadronVertices[2].pos, n);
    SubDivide(octaheadronVertices[5].pos, octaheadronVertices[1].pos, octaheadronVertices[4].pos, n);
    SubDivide(octaheadronVertices[5].pos, octaheadronVertices[3].pos, octaheadronVertices[2].pos, n);
    SubDivide(octaheadronVertices[5].pos, octaheadronVertices[3].pos, octaheadronVertices[4].pos, n);
}

void Shape::SubDivide(const glm::vec3 a, const glm::vec3 b, const glm::vec3 c, int n)
{
    if(n > 0) 
    {
        glm::vec3 v1 = a + b;
        v1 = glm::normalize(v1);
        glm::vec3 v2 = a + c;
        v2 = glm::normalize(v2);
        glm::vec3 v3 = c + b;
        v3 = glm::normalize(v3);
    
        SubDivide(a, v1, v2, n - 1);
        SubDivide(c, v2, v3, n - 1);
        SubDivide(b, v3, v1, n - 1);
        SubDivide(v3, v2, v1, n - 1);
    } 
    else
    {
        makeTriangle(a, b, c);
    }
    
}

void Shape::makeTriangle(const glm::vec3 a, const glm::vec3 b, const glm::vec3 c)
{
    //vertex
    Vertex v1{ glm::vec3(a.x, a.y, a.z), glm::vec3(a.x, a.y, a.z), glm::vec2(0,0), glm::vec3(a.x, a.y, a.z) };
    Vertex v2{ glm::vec3(b.x, b.y, b.z), glm::vec3(b.x, b.y, b.z), glm::vec2(0,0), glm::vec3(b.x, b.y, b.z) };
    Vertex v3{ glm::vec3(c.x, c.y, c.z), glm::vec3(c.x, c.y, c.z), glm::vec2(0,0), glm::vec3(c.x, c.y, c.z) };

    this->vert.push_back(v1);
    this->vert.push_back(v2);
    this->vert.push_back(v3);

    //indices 
    //The current size of vertices gives us the starting index for the triangle
    int index = this->vert.size() - 3; //The index of the first vertex of this triangle

    //Add the indices 
    this->indi.push_back(index);    
    this->indi.push_back(index + 1); 
    this->indi.push_back(index + 2); 


    this->vert = vert;
    this->indi = indi;
}
