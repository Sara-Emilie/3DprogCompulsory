#include "Object.h"
#include "BoundingBox.h"
#include "glm/glm.hpp"

Object::Object() 
{
	
}

Object::Object(Mesh& mesh, glm::vec3 startPos, glm::vec3 size, ShaderClass& shaderProgram, glm::vec3 speed, bool hasGravity, float mass) //with physics
{
	this->mesh = mesh;
	this->currentPos = startPos;
	this->size = size;

	this->velocity = glm::vec3(0, 0, 0);
	this->prevPos = this->currentPos;

	this->mesh.VAO.Bind();

	this->hasGravity = true;
	this->mass = mass;
	this->radius = size.x / 2;

	model = glm::mat4(1.f);
	model = glm::scale(this->size);
	model = glm::translate(model, currentPos);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));

	AABB.Position = model[3];
	AABB.Extent = size;
}

Object::Object(Mesh& mesh, glm::vec3 startPos, glm::vec3 size, ShaderClass& shaderProgram)
{
	this->mesh = mesh;
	this->currentPos = startPos;
	this->size = size;

	this->velocity = glm::vec3(0, 0, 0);
	this->prevPos = this->currentPos;

	this->mesh.VAO.Bind();

	this->hasGravity = false;
	this->mass = 1;

	model = glm::mat4(1.f);
	model = glm::scale(this->size);
	model = glm::translate(model, currentPos);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));

	AABB.Position = model[3];
	AABB.Extent = size;
}

void Object::setPos(glm::vec3 newPos)
{
	this->currentPos = newPos;
	//TODO add prev pos here?
}

void Object::scale(glm::vec3 size)
{
	this->size = size;
}

void Object::rotate(float degree, glm::vec3 axis)
{
	this->degree = degree;
	this->axis = axis;
}

void Object::move(glm::vec3 amount)
{
	this->currentPos += amount;

}

void Object::updateMovement()
{
	if (hasGravity && this->velocity.y > -0.4) 
	{
		this->velocity += glm::vec3(0, -0.004, 0);
	}

	this->prevPos = this->currentPos;
	move(this->velocity);
}

void Object::Update(ShaderClass& shaderProgram)
{
	this->mesh.VAO.Bind();
	model = glm::mat4(1.f);
	model = glm::scale(this->size);
	model = glm::translate(model, currentPos);
	//model = glm::rotate(model, glm::radians(this->degree), this->axis); //TODO FIX
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));

	AABB.Position = model[3];
	AABB.Extent = size;
}


void Object::Draw(ShaderClass& shaderProgram, bool isRunning)
{
	if (isRunning) {
		updateMovement();
	}
	
	Update(shaderProgram);
	this->mesh.Draw(shaderProgram);
}
