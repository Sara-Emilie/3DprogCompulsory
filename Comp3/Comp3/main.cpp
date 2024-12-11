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

#include "Shaders/ShaderClass.h"
#include "Camera.h"
#include "stb_image.h"

#include "Particles/Emitter.h"
#include <random>
#include "Systems.h"
#include "ComponentManager.h"
#include "Components.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);



float speed = 0.5f;
bool isWireframe = false;
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

	//	// -- //

	Shape Sphere(Shape::SUBDIVIDED_OCTAHEDRON);
	Mesh BallMesh(Sphere.getvert(), Sphere.getindi());

	Shape cube(Shape::CUBE);
	Mesh cubeMesh(cube.getvert(), cube.getindi());
	

	std::vector<Entity> entities;

	ComponentManager<PositionComponent>* positionManager = new ComponentManager<PositionComponent>();
	ComponentManager<VelocityComponent>* velocityManager = new ComponentManager<VelocityComponent>();
	ComponentManager<RadiusComponent>* radiusManager = new ComponentManager<RadiusComponent>();
	ComponentManager<MeshComponent>* meshManager = new ComponentManager<MeshComponent>();


	for (int i = 0; i < 2; i++) 
	{
		Entity entity{ i };
		entities.push_back(entity);


		MeshComponent meshComp{ BallMesh, BallMesh };

		PositionComponent posComp
		{
			{i , i * 10 }, // x
			{1 , 1}, // y
			{i , i * 10}  // z
		};
		VelocityComponent velComp
		{
			{0.1f * i, 0}, // vx
			{0.05f * i, -1 }, // vy
			{0.02f * i, 0 }  // vz
		};

		RadiusComponent radiusComp{ {1.0f} }; // radius

		positionManager->AddComponent(entity.ID, posComp);
		velocityManager->AddComponent(entity.ID, velComp);
		radiusManager->AddComponent(entity.ID, radiusComp);
		meshManager->AddComponent(entity.ID, meshComp);
	}

	MovementSystem movementSystem(positionManager, velocityManager);
	//CollisionSystem collisionSystem(positionManager, velocityManager, radiusManager);
	RenderingSystem renderingSystem(positionManager, meshManager);


	///////////// Particles/////////////////////
	
	Emitter particleEmitter;
	while (!glfwWindowShouldClose(window))
	{
		
		processInput(window);

		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastframe;
		lastframe = currentFrame;

		shaderprogram.Activate();
		glClearColor(0.7f, 0.7f, 0.97f, 1.0f);
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
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderprogram, "camMatrix");

		renderingSystem.Render(entities, shaderprogram);
		movementSystem.Move(entities, deltaTime);

		//Particles
		particleEmitter.UpdateParticles(deltaTime);
		particleEmitter.DrawParticles(shaderprogram, camera);


		if (particleEmitter.Alive < particleEmitter.Max)
		{
			double lower = -15;
			double upper = 15;
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(lower, upper);
			std::vector<double> random_numbers;

			for (int i = 0; i < 3; ++i)
			{
				random_numbers.push_back(dis(gen));
			}

			particleEmitter.CreateParticles(BallMesh, glm::vec3(5 + random_numbers[1] * 10, 5 + random_numbers[0] * 10, 1000), glm::vec3(1 + random_numbers[1], 1 + random_numbers[2], -100));
			particleEmitter.CreateParticles(BallMesh, glm::vec3(5 + random_numbers[0] * 10, 5 + random_numbers[1] * 10, 1000), glm::vec3(1 + random_numbers[1], 1 + random_numbers[2], -100));
		}



		processInput(window);

		glfwPollEvents();
		glfwSwapBuffers(window);


	}


	//del program
	delete positionManager;
	delete velocityManager;
	delete radiusManager;


	shaderprogram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

 //-- //

//	glm::vec3 amount1 = glm::vec3(0.00, 0.0, 0.00);
//	glm::vec3 amount2= glm::vec3(0.02, 0, 0.02);
//
//
//	///////////////////// Walls and floor /////////////
//	Shape cube(Shape::CUBE);
//	Mesh cubeMesh(cube.getvert(), cube.getindi());
//
//	//Cube 1: The Floor 
//	Object cube1Object(cubeMesh, glm::vec3(0.0, -50, 0.0), glm::vec3(5, 0.1, 5), shaderprogram);
//
//	//Cube 2: Wall along X-axis (right side)
//	Object cube2Object(cubeMesh, glm::vec3(50.0, 0, 0.0), glm::vec3(0.1, 5, 5), shaderprogram);
//
//	//Cube 3: Wall along X-axis (left side)
//	Object cube3Object(cubeMesh, glm::vec3(-50.0, 0, 0.0), glm::vec3(0.1, 5, 5), shaderprogram);
//
//	//Cube 4: Wall along Z-axis (back side)
//	Object cube4Object(cubeMesh, glm::vec3(0.0, 0, 50.0), glm::vec3(5, 5, 0.1), shaderprogram);
//
//	//Cube 5: Wall along Z-axis (front side)
//	Object cube5Object(cubeMesh, glm::vec3(0.0, 0, -50.0), glm::vec3(5, 5, 0.1), shaderprogram);
//					
//	//////////////////// Balls //////////////////////
//
//	Shape Sphere(Shape::SUBDIVIDED_OCTAHEDRON);
//	Mesh BallMesh(Sphere.getvert(), Sphere.getindi());
//	int index = 0;
//	float size = 0.1;
//
//	Object* Ball[16] = {};
//	for (int i = 0; i < 4; i++) {
//		for (int j = 0; j < 4; j++) {
//			Ball[index] = new Object(BallMesh, glm::vec3((float)i * 10, 0.5, (float)j *10), glm::vec3(size, size, size), shaderprogram, glm::vec3(0, 0.0, 0.0), true, i * 0.2f);
//			Ball[index]->velocity.z = 0.05 * (rand() * 0.0005);
//			Ball[index]->velocity.x = 0.03 * (rand() * 0.0005);
//			Ball[index]->velocity.y = 0.04;
//			index++;
//		}
//	}
//
//
//	///////////// Particles/////////////////////
//	Emitter particleEmitter;
//	while (!glfwWindowShouldClose(window))
//	{
//		processInput(window);
//		
//		float currentFrame = glfwGetTime();
//		float deltaTime = currentFrame - lastframe;
//		lastframe = currentFrame;
//
//		shaderprogram.Activate();
//		glClearColor(0.7f, 0.7f, 0.97f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		//wireframe
//		if (isWireframe) 
//		{
//			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//		}
//		else 
//		{
//			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//		}
//
//
//		//Camera
//		camera.Inputs(window);
//		camera.Matrix(45.0f, 0.1f, 100.0f, shaderprogram, "camMatrix");
//
//
//		//Cubes
//		cube1Object.Draw(shaderprogram, isRunning);
//		cube2Object.Draw(shaderprogram, isRunning);
//		cube3Object.Draw(shaderprogram, isRunning);
//		cube4Object.Draw(shaderprogram, isRunning);
//		cube5Object.Draw(shaderprogram, isRunning);
//		
//		
//		//Collision
//		Collision collide;
//		
//		for (int i = 0; i < 16; i++) 
//		{
//			Ball[i]->Draw(shaderprogram, isRunning);
//		
//			//Collide with walls
//			collide.CollideWithWall(Ball[i]->AABB.Extent, Ball[i]->AABB.Position, cube1Object.AABB.Extent, cube1Object.AABB.Position,Ball[i]->velocity);
//			collide.CollideWithWall(Ball[i]->AABB.Extent, Ball[i]->AABB.Position, cube2Object.AABB.Extent, cube2Object.AABB.Position, Ball[i]->velocity);
//			collide.CollideWithWall(Ball[i]->AABB.Extent, Ball[i]->AABB.Position, cube3Object.AABB.Extent, cube3Object.AABB.Position, Ball[i]->velocity);
//			collide.CollideWithWall(Ball[i]->AABB.Extent, Ball[i]->AABB.Position, cube4Object.AABB.Extent, cube4Object.AABB.Position, Ball[i]->velocity);
//			collide.CollideWithWall(Ball[i]->AABB.Extent, Ball[i]->AABB.Position, cube5Object.AABB.Extent, cube5Object.AABB.Position, Ball[i]->velocity);
//			
//			//Collide with other balls
//			for (int j = i + 1; j < 16; j++) 
//			{
//				collide.CollideWithBall(
//					Ball[i]->AABB.Extent,
//					Ball[i]->AABB.Position,
//					Ball[j]->AABB.Extent,
//					Ball[j]->AABB.Position,
//					Ball[i]->velocity,
//					Ball[i]->radius
//				);
//				collide.CollideWithBall(
//					Ball[j]->AABB.Extent,
//					Ball[j]->AABB.Position,
//					Ball[i]->AABB.Extent,
//					Ball[i]->AABB.Position,
//					Ball[j]->velocity,
//					Ball[j]->radius
//				);
//			}
//
//		}
//		
//
//		//Particles
//		particleEmitter.UpdateParticles(deltaTime);
//		particleEmitter.DrawParticles(shaderprogram, camera);
//
//
//		if (particleEmitter.Alive < particleEmitter.Max)
//		{
//				double lower = -15;
//				double upper = 15;
//				std::random_device rd;
//				std::mt19937 gen(rd());
//				std::uniform_real_distribution<> dis(lower, upper);
//				std::vector<double> random_numbers;
//
//				for (int i = 0; i < 3; ++i) 
//				{
//					random_numbers.push_back(dis(gen));
//				}
//
//			particleEmitter.CreateParticles(BallMesh, glm::vec3(5 + random_numbers[1] * 10, 5 + random_numbers[0] * 10, 1000), glm::vec3(1 + random_numbers[1], 1 + random_numbers[2], -100));
//			particleEmitter.CreateParticles(BallMesh, glm::vec3(5 + random_numbers[0] * 10, 5 + random_numbers[1] * 10, 1000), glm::vec3(1 + random_numbers[1], 1 + random_numbers[2], -100));
//		}
//
//
//		
//		processInput(window);
//				
//		glfwPollEvents();
//		glfwSwapBuffers(window);
//
//
//	}
//
//
//	//del program
//	shaderprogram.Delete();
//	glfwDestroyWindow(window);
//	glfwTerminate();
//	
//	return 0;
//}


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