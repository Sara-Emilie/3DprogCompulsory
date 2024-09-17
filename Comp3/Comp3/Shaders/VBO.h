#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glad/glad.h>
#include "glm/glm.hpp"
#include"vector"

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 tex;
	glm::vec3 col;

	//GLfloat nx, ny, nz;
	//GLfloat tx, ty;
	//GLfloat r, g, b;

};


class VBO
{
public:
	// Reference ID of the Vertex Buffer Object
	GLuint ID;
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	//VBO(GLfloat* vertices, GLsizeiptr size);
	VBO(std::vector<Vertex>& verticies);

	// Binds the VBO
	void Bind();
	// Unbinds the VBO
	void Unbind();
	// Deletes the VBO
	void Delete();
};

#endif