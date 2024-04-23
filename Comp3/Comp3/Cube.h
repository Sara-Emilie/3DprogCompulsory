#pragma once

#include <glm/glm.hpp>
#include "Buffer.h"
#include "Shaders/ShaderClass.h"


class Cube
{

public:

	Cube();
	~Cube();

	void Update();
	void Render();

	glm::vec3 GetPosition() { return m_position; }

	glm::mat4 m_model;
	glm::vec3 m_position;


private:

	Buffer m_buffer;

	
};