#pragma once
#include "glm/glm.hpp"
#include "Shaders/ShaderClass.h"
#include "Mesh.h"
#include "Collision.h"
#include "BoundingBox.h"
class Object
{
public:
	Mesh mesh;
	glm::vec3 currentPos;
	glm::vec3 speed;
	glm::vec3 prevPos;
	glm::vec3 size;
	glm::vec3 velocity;
	float degree;
	glm::vec3 axis;
	glm::vec3 amount;

	bool hasGravity;
	float mass;

	Object();
	Object(Mesh& mesh, glm::vec3 startPos, glm::vec3 size, ShaderClass& shaderProgram);
	Object(Mesh& mesh, glm::vec3 startPos, glm::vec3 size, ShaderClass& shaderProgram, glm::vec3 speed, bool hasGravity, float mass);

	void setPos(glm::vec3 newPos);
	void scale(glm::vec3 size);
	void rotate(float degree, glm::vec3 axis);

	void move(glm::vec3 amount);
	virtual void updateMovement(); //so physics class can overide

	void Draw(ShaderClass& shaderProgram);
	void Update(ShaderClass& shaderProgram);


public:
	BoundingBox AABB;
	glm::mat4 model;

};

