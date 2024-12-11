#include "Emitter.h"

void Emitter::CreateParticles(Mesh& mesh, glm::vec3 pos, glm::vec3 v)
{
	if (Alive < Max)
	{
		PosX[Alive] = pos.x;
		PosZ[Alive] = pos.z;
		PosY[Alive] = pos.y;

		VX[Alive] = v.x;
		VZ[Alive] = v.z;
		VY[Alive] = v.y;

		LifeTime[Alive] = 10;
		meshes[Alive] = mesh;

		Alive++;
	}
}

void Emitter::UpdateParticles(float deltatime)
{
	for (int i = 0; i < Alive; i++) 
	{
		PosX[i] += VX[i] * deltatime;
		PosZ[i] += VZ[i] * deltatime;
		PosY[i] += VY[i] * deltatime;
		LifeTime[i] -= deltatime;

		if (LifeTime[i] <= 0) 
		{
			Alive -= 1;
			if (i != Alive)
			{
				PosX[i] = PosX[Alive];
				PosZ[i] = PosZ[Alive];
				PosY[i] = PosY[Alive];

				VX[i] = VX[Alive];
				VZ[i] = VZ[Alive];
				VY[i] = VY[Alive];

				LifeTime[i] = LifeTime[Alive];
				meshes[i] = meshes[Alive];
			}
		}
	}
}

void Emitter::DrawParticles(ShaderClass& shaderprogram, Camera& camera)
{
	for (int i = 0; i < Alive; i++)
	{
		glm::mat4 model = glm::mat4(1.f);
		model = glm::scale(model,glm::vec3(0.04f));
		model = glm::translate(model, glm::vec3(PosX[i], PosZ[i], PosY[i]));
		model = glm::rotate(model, glm::radians(45.0f * LifeTime[i] * 2), glm::vec3(0, 0, 1));

		shaderprogram.Activate();
		glUniformMatrix4fv(glGetUniformLocation(shaderprogram.shaderID, "model"), 1, GL_FALSE, glm::value_ptr(model));

		meshes[i].Draw(shaderprogram);
	}
}

