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

#include "Shaders/ShaderClass.h"
#include "Shaders/VAO.h"
#include "Shaders/VBO.h"
#include "Shaders/EBO.h"
#include "Camera.h"
#include "Cube.h"
#include "Plane.h"
#include "stb_image.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

float speed = 0.1f;
float MovementX{ 0 };
float MovementZ{ 0 };

// Window dimensions
const unsigned int width = 800;
const unsigned int height = 600;




int main()
{
	// Initialize GLFW
	glfwInit();
	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create a GLFWwindow object of 800 by 800 pixels, naming it "OpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGLProject", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	glViewport(0, 0, width, height);

	// Generates Shader object using shaders defiend in "shader.vs" and "shader.fs"

	

	ShaderClass shaderprogram("default.vert", "default.frag");

	shaderprogram.Activate();
	
	Cube cube;
	Plane plane;

	glUniform1i(glGetUniformLocation(shaderprogram.shaderID, "ourTexture"), 0);
	glUniform1i(glGetUniformLocation(shaderprogram.shaderID, "specularTexture"), 1);

	plane.model = scale(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f));
	plane.model = translate(plane.model, glm::vec3(0.0f, -0.051, 0.0f));

	Camera camera(width, height, glm::vec3(0.0f, 10.0f, 0.0f));

	
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		

		shaderprogram.Activate();

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		camera.Inputs(window);
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderprogram, "camMatrix");

		cube.model = translate(glm::mat4(1.0f), glm::vec3(MovementX, 0.0f, MovementZ));
		glUniformMatrix4fv(glGetUniformLocation(shaderprogram.shaderID, "model"), 1, GL_FALSE, &cube.model[0][0]);
		cube.Render();
		
		//plane.model = glm::mat4(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(shaderprogram.shaderID, "model"), 1, GL_FALSE, &plane.model[0][0]);
		plane.Render();


		processInput(window);
				
		glfwPollEvents();
		glfwSwapBuffers(window);


	}


	//del program
	shaderprogram.Delete();

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//Cube movement
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) //left
	{
		MovementX += speed;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) //right
	{
		MovementX -= speed;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) //back
	{
		MovementZ -= speed;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) //forward
	{
		MovementZ += speed;
	}

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}