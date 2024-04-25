#pragma once

#include <glm/glm.hpp>
#include "Shaders/ShaderClass.h"
#include "BoundingBox.h"



class Cube
{

public:

	Cube();
	~Cube();

	void Update();
	void Render();
	glm::vec3 calculateBarysentricCoordinates(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 position);

	glm::vec3 GetPosition() { return model[3]; }

	glm::mat4 model = glm::mat4(1.f);
	


	unsigned int VBO, VAO, EBO;

	BoundingBox AABB;


	unsigned int texture;
	unsigned int textureSpecular;

private:


	
};