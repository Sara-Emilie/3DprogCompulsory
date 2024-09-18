#ifndef MESH_CLASS_H
#define MESH_CLASS_H
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include <glad/glad.h>

#include "Shaders/ShaderClass.h"
#include "Camera.h"
#include "Shaders/VAO.h"
#include "Shaders/EBO.h"
#include "Shaders/VBO.h"

struct Triangle 
{
	std::vector<Vertex> verticies;
};

class Mesh
{
public:
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;

	VAO VAO;


	Mesh();
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices);

	void Draw(ShaderClass& shader);
};
#endif
