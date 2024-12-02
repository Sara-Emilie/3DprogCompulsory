#include "BarysentricCalc.h"

BarysentricCalc::BarysentricCalc()
{
}

glm::vec3 BarysentricCalc::calculateBarysentricCoordinates(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 position)
{
	//used code from the math III notes
	p1.y = 0;
	p2.y = 0;
	p3.y = 0;
	position.y = 0;

	glm::vec3 p12 = p2 - p1;
	glm::vec3 p13 = p3 - p1;
	glm::vec3 cross = glm::cross(p13, p12);
	float area_123 = cross.y;
	glm::vec3 baryc;

	// u
	glm::vec3 p = p2 - position;
	glm::vec3 q = p3 - position;
	glm::vec3 nu = glm::cross(q, p);
	baryc.x = nu.y / area_123;

	// v
	p = p3 - position;
	q = p1 - position;
	glm::vec3 nv = glm::cross(q, p);
	baryc.y = nv.y / area_123;

	// w
	p = p1 - position;
	q = p2 - position;
	glm::vec3 nw = (glm::cross(q, p));
	baryc.z = nw.y / area_123;

	return baryc;
}
