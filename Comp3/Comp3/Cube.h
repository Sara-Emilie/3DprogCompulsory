#pragma once

#include <glm/glm.hpp>
#include "Shaders/ShaderClass.h"
#include "Shaders/VAO.h"
#include "Shaders/VBO.h"
#include "Shaders/EBO.h"



class Cube
{

public:

	Cube();
	~Cube();

	void Update();
	void Render(const unsigned int height, const unsigned int width);

	glm::vec3 GetPosition() { return position; }

	glm::mat4 model;
	glm::vec3 position;


	unsigned int VBO, VAO;

	unsigned int ourTexture;

private:


	
};