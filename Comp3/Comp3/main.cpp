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


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);



float speed = 0.5f;
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

	
	///////////////////// Terrain ////////////
	Terrain terrain;
	Mesh terrainMesh(terrain.getvert());
	Object terrainObject(terrainMesh, glm::vec3(1.0), glm::vec3(1.0), shaderprogram);

	///////////////////// Walls and floor /////////////

	Shape cube(Shape::CUBE);

	//Cube 1: The Floor 
	Mesh cube1Mesh(cube.getvert(), cube.getindi());
	Object cube1Object(cube1Mesh, glm::vec3(0.0, -50, 0.0), glm::vec3(5, 0.1, 5), shaderprogram);

	//Cube 2: Wall along X-axis (right side)
	Mesh cube2Mesh(cube.getvert(), cube.getindi());
	Object cube2Object(cube2Mesh, glm::vec3(50.0, 0, 0.0), glm::vec3(0.1, 5, 5), shaderprogram);

	//Cube 3: Wall along X-axis (left side)
	Mesh cube3Mesh(cube.getvert(), cube.getindi());
	Object cube3Object(cube3Mesh, glm::vec3(-50.0, 0, 0.0), glm::vec3(0.1, 5, 5), shaderprogram);

	//Cube 4: Wall along Z-axis (back side)
	Mesh cube4Mesh(cube.getvert(), cube.getindi());
	Object cube4Object(cube4Mesh, glm::vec3(0.0, 0, 50.0), glm::vec3(5, 5, 0.1), shaderprogram);

	//Cube 5: Wall along Z-axis (front side)
	Mesh cube5Mesh(cube.getvert(), cube.getindi());
	Object cube5Object(cube5Mesh, glm::vec3(0.0, 0, -50.0), glm::vec3(5, 5, 0.1), shaderprogram);
					
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

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastframe;
		lastframe = currentFrame;
	

		//BSS.drawSurface(shaderprogram, BSS);
		//BSSMESH.Draw(shaderprogram);

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

		terrainObject.DrawArray(shaderprogram, isRunning);


		//cube1Object.Draw(shaderprogram, isRunning);
		//cube2Object.Draw(shaderprogram, isRunning);
		//cube3Object.Draw(shaderprogram, isRunning);
		//cube4Object.Draw(shaderprogram, isRunning);
		//cube5Object.Draw(shaderprogram, isRunning);
		//
	
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