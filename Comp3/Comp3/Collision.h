#pragma once
#include "glm/glm.hpp"
#include "Object.h"
#include "BoundingBox.h"

class Collision
{


public:

	Collision();

	//void CollideWithBall 
	void CollideWithWall(glm::vec3 Extentball, glm::vec3 Positionball, glm::vec3 Extentwall, glm::vec3 Positionwall, glm::vec3& v);
	
};

