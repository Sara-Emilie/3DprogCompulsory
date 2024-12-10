#pragma once

#include "../Mesh.h"
#include "glm/glm.hpp"

struct Emitter
{
	static const int Max = 5000;

	float PosX[Max];
	float PosZ[Max];
	float PosY[Max];

	float VX[Max]; //velocity
	float VZ[Max];
	float VY[Max];

	float LifeTime[Max];

	int Alive = 0;

	Mesh meshes[Max];

	void CreateParticles(Mesh& mesh, glm::vec3 pos, glm::vec3 v);
	void UpdateParticles(float deltatime);
	void DrawParticles(ShaderClass& shaderprogram, Camera& camera);

};
