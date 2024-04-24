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
	void Render();

	glm::vec3 GetPosition() { return position; }

	glm::mat4 model = glm::mat4(1.f);
	glm::vec3 position = glm::vec3(0.f);;


	unsigned int VBO, VAO, EBO;

	unsigned int texture;
	unsigned int textureSpecular;

private:


	
};