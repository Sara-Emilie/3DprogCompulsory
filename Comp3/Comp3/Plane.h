#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include <glad/glad.h>

struct Vertex
{
	GLfloat x, y, z;
	GLfloat nx, ny, nz;
	GLfloat tx, ty;

};

struct Triangle
{
	GLuint v1, v2, v3;
};

class Plane
{
public:
	std::vector<Vertex> vertices;
	std::vector<Triangle> triangles;



	unsigned int texture;
	unsigned int textureSpecular;
	unsigned int VBO, VAO, EBO;

	glm::mat4 model = glm::mat4(1.f);;

	Plane();
	~Plane();

	void Render();

};


