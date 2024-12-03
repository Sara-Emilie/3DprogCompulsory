#include <iostream>
#include <fstream>
#include <iomanip>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <cstddef>
#include <string>

#include <Eigen/Dense>

#include "Shape.h"
#include "Mesh.h"
#include "Object.h"
#include "Collision.h"

#include "BSplineFlate.h"
#include "terrain.h"

#include "Shaders/ShaderClass.h"
#include "Camera.h"
#include "stb_image.h"

#include "BarysentricCalc.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);



float speed = 0.5f;
float speed2 = 100.f;
bool isWireframe = true;
bool isRunning = false;

// Window dimensions
const unsigned int width = 1200;
const unsigned int height = 800;



int main()
{
	// Initialize GLFW & glad & window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGLProject", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, width, height);


	// Generates Shader object
	ShaderClass shaderprogram("default.vert", "default.frag");
	shaderprogram.Activate();
	

	//TODO fix texture
	glUniform1i(glGetUniformLocation(shaderprogram.shaderID, "ourTexture"), 0);
	glUniform1i(glGetUniformLocation(shaderprogram.shaderID, "specularTexture"), 1);


	//Camera
	Camera camera(width, height, glm::vec3(0.0f, 10.0f, 0.0f));

	
	glEnable(GL_DEPTH_TEST);
	float lastframe = glfwGetTime();

	glm::vec3 amount1 = glm::vec3(0.00, 0.0, 0.00);
	glm::vec3 amount2= glm::vec3(0.02, 0, 0.02);

	BarysentricCalc bary;

	///////////////////// Terrain ////////////
	Terrain terrain;
	//Mesh terrainMesh(terrain.getvert());
	Mesh terrainMesh(terrain.getvert(), terrain.getindi());
	Object terrainObject(terrainMesh, glm::vec3(1.0), glm::vec3(1.0), shaderprogram);

	///////////////////// Spheres /////////////

	Shape Sphere(Shape::SUBDIVIDED_OCTAHEDRON);
	Mesh SphereMesh(Sphere.getvert(), Sphere.getindi());

	vector< Object*> Spheres;

	for (int i = 1; i < 10; i++) 
	{
		Spheres.push_back(new Object(SphereMesh, glm::vec3(100.0 + i * 4, 1.0, 10.0 + i), glm::vec3(6.f), shaderprogram, glm::vec3(0,0,0), 1));
	}
	

	//**********************************      While-loop           ***********************************************//
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastframe;
		lastframe = currentFrame;
	
		shaderprogram.Activate();
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//wireframe
		if (isWireframe) 
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else 
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		//Camera
		camera.Inputs(window);
		camera.Matrix(45.0f, 0.1f, 1000.0f, shaderprogram, "camMatrix");

		
		for (int i = 0; i < terrain.triangles.size(); i++)
		{
			Vertex vp1 = terrain.triangles[i].v1;
			Vertex vp2 = terrain.triangles[i].v2;
			Vertex vp3 = terrain.triangles[i].v3;

			glm::vec3 p1 = glm::vec3(vp1.pos.x, vp1.pos.y, vp1.pos.z);
			glm::vec3 p2 = glm::vec3(vp2.pos.x, vp2.pos.y, vp2.pos.z);
			glm::vec3 p3 = glm::vec3(vp3.pos.x, vp3.pos.y, vp3.pos.z);

		
				//calc normal
			
			glm::vec3 u = vp2.pos - vp1.pos;
			glm::vec3 v = vp3.pos - vp1.pos;

			glm::vec3 normal = glm::normalize(glm::cross(u, v));

				
			for (Object* balls : Spheres)
			{

				glm::vec3 barycentric = bary.calculateBarysentricCoordinates(p1, p2, p3, balls->model[3]);

				if (barycentric.x >= 0 && barycentric.y >= 0 && barycentric.z >= 0 && barycentric.x <= 1 && barycentric.y <= 1 && barycentric.z <= 1)
				{
					float u = barycentric.x;
					float P = p1.y;
					float v = barycentric.y;
					float Q = p2.y;
					float w = barycentric.z;
					float R = p3.y;

					float newY = (u * P + v * Q + w * R) + 6 / 2;
					balls->currentPos.y = newY;

					float g = 9.81;
					glm::vec3 a = glm::vec3(g * normal.x * normal.y, g * normal.y * normal.y - 1, g * normal.z * normal.y);
					glm::vec3 newVelocity = glm::vec3(balls->velocity + glm::vec3(a.x / 2 * deltaTime * deltaTime, a.y / 2 * deltaTime * deltaTime, a.z / 2 * deltaTime * deltaTime));
					balls->velocity = newVelocity;
					balls->currentPos = glm::vec3(balls->currentPos + newVelocity);

					////rotate
					balls->degree += glm::length(balls->velocity) / deltaTime * deltaTime;
					
					if (newVelocity.y != 0) 
					{
						balls->axis = glm::cross(normal, balls->velocity);
						balls->axis = glm::normalize(balls->axis);
					}
				}
			}
		}

		Collision collide;




		// Punktsky
		//terrainObject.DrawArray(shaderprogram, isRunning);

		//Triangel terrain
		terrainObject.Draw(shaderprogram, isRunning);


		// Cube movement based on key input
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			Spheres[0]->currentPos.x += 10.f * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			Spheres[0]->currentPos.x -= 10.f * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			Spheres[0]->currentPos.z -= 10.f * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			Spheres[0]->currentPos.z += 10.f * deltaTime;
		}

		//Interaktivitet
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
			Spheres[0]->currentPos = camera.Position;
		}


		//Cubes
		//Spheres[0]->Draw(shaderprogram, isRunning);
		
	/*	for (Object* balls : Spheres) 
		{
			balls->Draw(shaderprogram, isRunning);
		}*/


		// Kollisjon fungerer men uten friksjon eller luftmotstand vil velocity bli alt før høg over tid
		for (int i = 0; i < Spheres.size(); i++)
		{
			Object* balls = Spheres[i];
			balls->Draw(shaderprogram, isRunning);


			// Collide with other balls
			for (int j = i + 1; j < Spheres.size(); j++)
			{
				Object* otherBall = Spheres[j];

				collide.CollideWithBall(
					balls->AABB.Extent,
					balls->AABB.Position,
					otherBall->AABB.Extent,
					otherBall->AABB.Position,
					balls->velocity,
					balls->radius
				);

				collide.CollideWithBall(
					otherBall->AABB.Extent,
					otherBall->AABB.Position,
					balls->AABB.Extent,
					balls->AABB.Position,
					otherBall->velocity,
					otherBall->radius
				);
			}
		}

		processInput(window);
				
		glfwPollEvents();
		glfwSwapBuffers(window);


	}


	//del program
	shaderprogram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		isWireframe = true;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		isWireframe = false;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		isRunning = true;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}