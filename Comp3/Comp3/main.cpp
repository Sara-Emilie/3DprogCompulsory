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


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);



float speed = 0.5f;
bool isWireframe = false;

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
	
	


	//
	glUniform1i(glGetUniformLocation(shaderprogram.shaderID, "ourTexture"), 0);
	glUniform1i(glGetUniformLocation(shaderprogram.shaderID, "specularTexture"), 1);


	//Camera
	Camera camera(width, height, glm::vec3(0.0f, 10.0f, 0.0f));

	
	glEnable(GL_DEPTH_TEST);
	float lastframe = glfwGetTime();
	glm::vec3 amount1 = glm::vec3(0.00, -0.02, 0.00);
	glm::vec3 amount2= glm::vec3(0.02, 0, 0.02);
	Shape Sphere1(Shape::SUBDIVIDED_OCTAHEDRON);
	Mesh Sphere1Mesh(Sphere1.getvert(), Sphere1.getindi());
	Object Sphere1Object(Sphere1Mesh, glm::vec3(1.0,4,1.0), glm::vec3(1.0), shaderprogram, amount1);

	Shape Sphere2(Shape::SUBDIVIDED_OCTAHEDRON);
	Mesh Sphere2Mesh(Sphere2.getvert(), Sphere2.getindi());
	Object Sphere2Object(Sphere2Mesh, glm::vec3(4.0, 4, 1.0), glm::vec3(1.0), shaderprogram, amount2);

// Cube 1: The Floor 
	Shape cube1(Shape::CUBE);
	Mesh cube1Mesh(cube1.getvert(), cube1.getindi());
	Object cube1Object(cube1Mesh, glm::vec3(0.0, -100, 0.0), glm::vec3(10, 0.1, 10), shaderprogram, glm::vec3(0));

	// Cube 2: Wall along X-axis (right side)
	Shape cube2(Shape::CUBE);
	Mesh cube2Mesh(cube2.getvert(), cube2.getindi());
	Object cube2Object(cube2Mesh, glm::vec3(100.0, 0, 0.0), glm::vec3(0.1, 10, 10), shaderprogram, glm::vec3(0));

	// Cube 3: Wall along X-axis (left side)
	Shape cube3(Shape::CUBE);
	Mesh cube3Mesh(cube3.getvert(), cube3.getindi());
	Object cube3Object(cube3Mesh, glm::vec3(-100.0, 0, 0.0), glm::vec3(0.1, 10, 10), shaderprogram, glm::vec3(0));

	// Cube 4: Wall along Z-axis (back side)
	Shape cube4(Shape::CUBE);
	Mesh cube4Mesh(cube4.getvert(), cube4.getindi());
	Object cube4Object(cube4Mesh, glm::vec3(0.0, 0, 100.0), glm::vec3(10, 10, 0.1), shaderprogram, glm::vec3(0));

	// Cube 5: Wall along Z-axis (front side)
	Shape cube5(Shape::CUBE);
	Mesh cube5Mesh(cube5.getvert(), cube5.getindi());
	Object cube5Object(cube5Mesh, glm::vec3(0.0, 0, -100.0), glm::vec3(10, 10, 0.1), shaderprogram, glm::vec3(0));

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
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderprogram, "camMatrix");


		Sphere1Object.Draw(shaderprogram);
		Sphere2Object.Draw(shaderprogram);

		cube1Object.Draw(shaderprogram);
		cube2Object.Draw(shaderprogram);
		cube3Object.Draw(shaderprogram);
		cube4Object.Draw(shaderprogram);
		cube5Object.Draw(shaderprogram);
		
		Sphere1Object.move(amount1);

		Collision collide;
		collide.CollideWithWall(Sphere1Object.AABB.Extent, Sphere1Object.AABB.Position, cube1Object.AABB.Extent, cube1Object.AABB.Position, amount1);
		
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
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}