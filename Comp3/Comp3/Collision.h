#pragma once
#include "glm/glm.hpp"
#include "Object.h"
#include "BoundingBox.h"

class Collision
{


public:

	Collision();

	void CollideWithBall(glm::vec3 Extentball, glm::vec3 Positionball, glm::vec3 Extentotherball, glm::vec3 Positionotherball, glm::vec3& v, float radius);

	void CollideWithWall(glm::vec3 Extentball, glm::vec3 Positionball, glm::vec3 Extentwall, glm::vec3 Positionwall, glm::vec3& v);
	
};

