#pragma once
#include "glm/glm.hpp"

class BarysentricCalc
{
	public:
	BarysentricCalc();

	glm::vec3 calculateBarysentricCoordinates(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 position);
};

