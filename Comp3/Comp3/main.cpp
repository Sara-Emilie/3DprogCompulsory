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

#include "Shaders/ShaderClass.h"
#include "Camera.h"
#include "stb_image.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);



float speed = 1.1f;


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
	
	//unsigned int texture;
	//unsigned int textureSpecular;

	////unsigned int texture;
	//glGenTextures(1, &texture);
	//glBindTexture(GL_TEXTURE_2D, texture);

	//// set the texture wrapping/filtering options (on the currently bound texture object)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//// load and generate the texture
	//int width, height, nrChannels;
	//unsigned char* data = stbi_load("Textures/dirt.jpg", &width, &height, &nrChannels, 0);
	//if (data)
	//{
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else
	//{
	//	std::cout << "Failed to load texture" << std::endl;
	//}
	//stbi_image_free(data);

	////unsigned int textureSpecular;
	//glGenTextures(1, &textureSpecular);
	//glBindTexture(GL_TEXTURE_2D, textureSpecular);

	//// set the texture wrapping/filtering options (on the currently bound texture object)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//// load and generate the texture
	//data = stbi_load("Textures/dirt_specular.jpg", &width, &height, &nrChannels, 0);
	//if (data)
	//{
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else
	//{
	//	std::cout << "Failed to load texture" << std::endl;
	//}
	//stbi_image_free(data);


	//
	glUniform1i(glGetUniformLocation(shaderprogram.shaderID, "ourTexture"), 0);
	glUniform1i(glGetUniformLocation(shaderprogram.shaderID, "specularTexture"), 1);


	//Camera
	Camera camera(width, height, glm::vec3(0.0f, 1.0f, 0.0f));

	
	glEnable(GL_DEPTH_TEST);
	float lastframe = glfwGetTime();

	Shape cube(Shape::CUBE);
	Mesh CubeMesh(cube.getvert(), cube.getindi());

	CubeMesh.VAO.Bind();
	glm::mat4 cubeModel = glm::mat4(1.f);
	//cubeModel = glm::scale(cubeModel, glm::vec3(0.05f));
	shaderprogram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderprogram.shaderID, "model"), 1, GL_FALSE, glm::value_ptr(cubeModel));

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastframe;
		lastframe = currentFrame;
	


		shaderprogram.Activate();
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


		//Camera
		camera.Inputs(window);
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderprogram, "camMatrix");


		CubeMesh.Draw(shaderprogram, camera);


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

}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}