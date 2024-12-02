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

	///////////////////// Walls and floor /////////////

	Shape cube(Shape::CUBE);

	//Cube 1
	Mesh cube1Mesh(cube.getvert(), cube.getindi());
	Object* cube1Object = new Object(cube1Mesh, glm::vec3(1.0, 0.0, 1.0), glm::vec3(5, 5, 5), shaderprogram);

	//Cube 1
	Mesh cube2Mesh(cube.getvert(), cube.getindi());
	Object cube2Object(cube1Mesh, glm::vec3(9.0, 1.0, 1.0), glm::vec3(5, 5, 5), shaderprogram);

	//////////////////// Balls //////////////////////

	Shape Sphere(Shape::SUBDIVIDED_OCTAHEDRON);
	Mesh BallMesh(Sphere.getvert(), Sphere.getindi());
	int index = 0;
	float size = 0.1;

	Object* Ball[16] = {};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			Ball[index] = new Object(BallMesh, glm::vec3((float)i * 10, 0.5, (float)j *10), glm::vec3(size, size, size), shaderprogram, glm::vec3(0, 0.0, 0.0), true, i * 0.2f);
			Ball[index]->velocity.z = 0.05 * (rand() * 0.0005);
			Ball[index]->velocity.x = 0.03 * (rand() * 0.0005);
			Ball[index]->velocity.y = 0.04;
			index++;
		}
	}





	//*********************************************************************************//
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

		


			glm::vec3 barycentric = bary.calculateBarysentricCoordinates(p1, p2, p3, cube1Object->model[3]);

			//std::cout << barycentric.x << " " << barycentric.y << " " << barycentric.z << std::endl;
		
			if (barycentric.x >= 0 && barycentric.y >= 0 && barycentric.z >= 0 && barycentric.x <= 1 && barycentric.y <= 1 &&  barycentric.z <= 1)
			{
				/*std::cout << barycentric.x << " " << barycentric.y << " " << barycentric.z << std::endl;
				std::cout << "p1: " << p1.x << " " << p1.y << " " << p1.z << std::endl;
				std::cout << "p2: " << p2.x << " " << p2.y << " " << p2.z << std::endl;
				std::cout << "p3: " << p3.x << " " << p3.y << " " << p3.z << std::endl;*/

				float u = barycentric.x;
				float P = p1.y;
				float v = barycentric.y;
				float Q = p2.y;
				float w = barycentric.z;
				float R = p3.y;

				//cube1Object->model[3].y = (u * P + v * Q + w * R) + 0.05/2;
				//std::cout << "hi" << std::endl;

				float newY = (u * P + v * Q + w * R) + 5 / 2;
				cube1Object->currentPos.y = newY;

			}
		}


		// Punktsky
		//terrainObject.DrawArray(shaderprogram, isRunning);

		//Triangel terrain
		terrainObject.Draw(shaderprogram, isRunning);


		// Cube movement based on key input
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			cube1Object->currentPos.x += 10.f * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			cube1Object->currentPos.x -= 10.f * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			cube1Object->currentPos.z -= 10.f * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			cube1Object->currentPos.z += 10.f * deltaTime;
		}

		//Update cube's position matrix
		cube1Object->Update(shaderprogram);

		//Cubes
		cube1Object->Draw(shaderprogram, isRunning);

		Collision collide;
		
		//for (int i = 0; i < 16; i++) 
		//{
		//	Ball[i]->Draw(shaderprogram, isRunning);
		//
		//	//Collide with walls
		//	collide.CollideWithWall(Ball[i]->AABB.Extent, Ball[i]->AABB.Position, cube1Object.AABB.Extent, cube1Object.AABB.Position,Ball[i]->velocity);
		//	collide.CollideWithWall(Ball[i]->AABB.Extent, Ball[i]->AABB.Position, cube2Object.AABB.Extent, cube2Object.AABB.Position, Ball[i]->velocity);
		//	collide.CollideWithWall(Ball[i]->AABB.Extent, Ball[i]->AABB.Position, cube3Object.AABB.Extent, cube3Object.AABB.Position, Ball[i]->velocity);
		//	collide.CollideWithWall(Ball[i]->AABB.Extent, Ball[i]->AABB.Position, cube4Object.AABB.Extent, cube4Object.AABB.Position, Ball[i]->velocity);
		//	collide.CollideWithWall(Ball[i]->AABB.Extent, Ball[i]->AABB.Position, cube5Object.AABB.Extent, cube5Object.AABB.Position, Ball[i]->velocity);
		//	
		//	//Collide with other balls
		//	for (int j = i + 1; j < 16; j++) 
		//	{
		//		collide.CollideWithBall(
		//			Ball[i]->AABB.Extent,
		//			Ball[i]->AABB.Position,
		//			Ball[j]->AABB.Extent,
		//			Ball[j]->AABB.Position,
		//			Ball[i]->velocity,
		//			Ball[i]->radius
		//		);

		//	
		//		collide.CollideWithBall(
		//			Ball[j]->AABB.Extent,
		//			Ball[j]->AABB.Position,
		//			Ball[i]->AABB.Extent,
		//			Ball[i]->AABB.Position,
		//			Ball[j]->velocity,
		//			Ball[j]->radius
		//		);
		//	}

		//}
		
		
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