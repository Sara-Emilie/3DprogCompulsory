#pragma once
#include "glm/glm.hpp"
#include "Shaders/ShaderClass.h"
#include "Mesh.h"
class Object
{
public:
	Mesh mesh;
	glm::vec3 currentPos;
	glm::vec3 prevPos;
	glm::vec3 size;
	glm::vec3 velocity;
	float degree;
	glm::vec3 axis;

	Object();
	Object(Mesh& mesh, glm::vec3 startPos, glm::vec3 size, ShaderClass& shaderProgram);

	void setPos(glm::vec3 newPos);
	void scale(glm::vec3 size);
	void rotate(float degree, glm::vec3 axis);

	void move(glm::vec3 amount);
	virtual void updateMovement(); //so physics class can overide

	void Draw(ShaderClass& shaderProgram);
	void Update(ShaderClass& shaderProgram);


};

