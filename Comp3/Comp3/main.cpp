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
#include "Camera.h"
#include "Cube.h"
#include "Plane.h"
#include "stb_image.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);



float speed = 1.1f;


// Window dimensions
const unsigned int width = 1200;
const unsigned int height = 800;

//used code for bezier curve from here:
//https://github.com/wolfcomp/OpenGLApp/blob/master/src/Math.h#L47-L69
template <typename T>
struct Bezier
{
	T p0, p1, p2, p3;

	Bezier() : p0(T()), p1(T()), p2(T()), p3(T())
	{
	}

	Bezier(T p0, T p1, T p2, T p3) : p0(p0), p1(p1), p2(p2), p3(p3)
	{
	}

	T operator()(const float t)
	{
		const auto u = 1 - t;
		const auto tp0 = powf(u, 3) * p0;
		const auto tp1 = 3 * powf(u, 2) * t * p1;
		const auto tp2 = 3 * u * powf(t, 2) * p2;
		const auto tp3 = powf(t, 3) * p3;
		return tp0 + tp1 + tp2 + tp3;
	}
};


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
	
	//Objects
	Cube cube;
	Cube NPC;
	Cube Object;
	Plane plane;

	glUniform1i(glGetUniformLocation(shaderprogram.shaderID, "ourTexture"), 0);
	glUniform1i(glGetUniformLocation(shaderprogram.shaderID, "specularTexture"), 1);

	//Model matrix
	cube.model = scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f));

	NPC.model = scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.05f));

	Object.model= translate(Object.model, glm::vec3(0.2f, 0.0f, 0.3f)); //trans, rot, scale
	Object.model = scale(Object.model, glm::vec3(0.05f, 0.05f, 0.05f));
	

	//Camera
	Camera camera(width, height, glm::vec3(0.0f, 10.0f, 0.0f));

	
	glEnable(GL_DEPTH_TEST);
	float lastframe = glfwGetTime();

	//Bezier curve
	glm::vec3 p00 = glm::vec3(-0.0f, 0.0f, 0.0f);
	glm::vec3 p01 = glm::vec3(-0.5f, 0.0f, 0.0f);
	glm::vec3 p02 = glm::vec3(1.0f, 0.0f, -0.5f);
	glm::vec3 p03 = glm::vec3(0.0f, 0.0f, -0.5f);
	auto bez = Bezier <glm::vec3>(p00, p01, p02, p03);
	float t = 0.0f;


	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastframe;
		lastframe = currentFrame;
		t = fmod(t + deltaTime, 1.0f);


		shaderprogram.Activate();
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//Camera
		camera.Inputs(window);
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderprogram, "camMatrix");


		//Cube
		for (int i = 0; i < plane.triangles.size(); i++)
		{
			Vertex vp1 = plane.triangles[i].v1;
			Vertex vp2 = plane.triangles[i].v2;
			Vertex vp3 = plane.triangles[i].v3;

			glm::vec3 p1 = glm::vec3(vp1.x, vp1.y, vp1.z);
			glm::vec3 p2 = glm::vec3(vp2.x, vp2.y, vp2.z);
			glm::vec3 p3 = glm::vec3(vp3.x, vp3.y, vp3.z);

		


			glm::vec3 barycentric = cube.calculateBarysentricCoordinates(p1, p2, p3, cube.model[3]);

			//std::cout << barycentric.x << " " << barycentric.y << " " << barycentric.z << std::endl;
		
			if (barycentric.x >= 0 && barycentric.y >= 0 && barycentric.z >= 0 && barycentric.x <= 1 && barycentric.y <= 1 &&  barycentric.z <= 1)
			{
				//std::cout << barycentric.x << " " << barycentric.y << " " << barycentric.z << std::endl;
				//std::cout << "p1: " << p1.x << " " << p1.y << " " << p1.z << std::endl;
				//std::cout << "p2: " << p2.x << " " << p2.y << " " << p2.z << std::endl;
				//std::cout << "p3: " << p3.x << " " << p3.y << " " << p3.z << std::endl;

				float u = barycentric.x;
				float P = p1.y;
				float v = barycentric.y;
				float Q = p2.y;
				float w = barycentric.z;
				float R = p3.y;

				cube.model[3].y = (u * P + v * Q + w * R) + 0.05/2;
				

			}
		}

		glUniformMatrix4fv(glGetUniformLocation(shaderprogram.shaderID, "model"), 1, GL_FALSE, &cube.model[0][0]);
		cube.Render();

		//Cube movement
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) //left
		{
			cube.model[3].x += speed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) //right
		{
			cube.model[3].x -= speed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) //back
		{
			cube.model[3].z -= speed * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) //forward
		{
			cube.model[3].z += speed * deltaTime;
		}



		//plane
		glUniformMatrix4fv(glGetUniformLocation(shaderprogram.shaderID, "model"), 1, GL_FALSE, &plane.model[0][0]);
		plane.Render();


		
		//NPC
		glUniformMatrix4fv(glGetUniformLocation(shaderprogram.shaderID, "model"), 1, GL_FALSE, &NPC.model[0][0]);
		NPC.Render();
		
		NPC.model[3] = glm::vec4(bez(t),1);

		for (int i = 0; i < plane.triangles.size(); i++)
		{
			Vertex vp1 = plane.triangles[i].v1;
			Vertex vp2 = plane.triangles[i].v2;
			Vertex vp3 = plane.triangles[i].v3;

			glm::vec3 p1 = glm::vec3(vp1.x, vp1.y, vp1.z);
			glm::vec3 p2 = glm::vec3(vp2.x, vp2.y, vp2.z);
			glm::vec3 p3 = glm::vec3(vp3.x, vp3.y, vp3.z);

			glm::vec3 barycentric = NPC.calculateBarysentricCoordinates(p2, p1, p3, NPC.model[3]);

			if (barycentric.x >= 0 && barycentric.y >= 0 && barycentric.z >= 0 && barycentric.x <= 1 && barycentric.y <= 1 && barycentric.z <= 1)
			{
				
				float u = barycentric.x;
				float P = p1.y;
				float v = barycentric.y;
				float Q = p2.y;
				float w = barycentric.z;
				float R = p3.y;

				NPC.model[3].y = (u * P + v * Q + w * R) + 0.05/2;


			}
		}



		//Object placed in world
		glUniformMatrix4fv(glGetUniformLocation(shaderprogram.shaderID, "model"), 1, GL_FALSE, &Object.model[0][0]);
		Object.Render();

		for (int i = 0; i < plane.triangles.size(); i++)
		{
			Vertex vp1 = plane.triangles[i].v1;
			Vertex vp2 = plane.triangles[i].v2;
			Vertex vp3 = plane.triangles[i].v3;

			glm::vec3 p1 = glm::vec3(vp1.x, vp1.y, vp1.z);
			glm::vec3 p2 = glm::vec3(vp2.x, vp2.y, vp2.z);
			glm::vec3 p3 = glm::vec3(vp3.x, vp3.y, vp3.z);

			glm::vec3 barycentric = Object.calculateBarysentricCoordinates(p2, p1, p3, Object.model[3]);

			if (barycentric.x >= 0 && barycentric.y >= 0 && barycentric.z >= 0 && barycentric.x <= 1 && barycentric.y <= 1 && barycentric.z <= 1)
			{

				float u = barycentric.x;
				float P = p1.y;
				float v = barycentric.y;
				float Q = p2.y;
				float w = barycentric.z;
				float R = p3.y;

				Object.model[3].y = (u * P + v * Q + w * R) + 0.05 / 2;


			}
		}



		//Collision detection between cube and NPC
		cube.Update();
		NPC.Update();
		
		if (cube.AABB.TestAABBAABB(NPC.AABB))
		{
			std::cout << "Collision" << std::endl;
		}
		else
		{
			std::cout << "No Collision" << std::endl;
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

}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}