#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include <glm/glm.hpp>

std::string get_file_contents(const char* filename);

class ShaderClass
{
public:
	// Reference ID of the Shader Program
	GLuint shaderID;

	GLuint GetShaderID() { return shaderID; };
	// Constructor that build the Shader Program from 2 different shaders
	ShaderClass();
	ShaderClass(const char* vertexFile, const char* fragmentFile);

	// Activates the Shader Program
	void Activate();
	// Deletes the Shader Program
	void Delete();

	bool SendUniformData(const std::string& uniformName, GLint data);
	bool SendUniformData(const std::string& uniformName, GLuint data);
	bool SendUniformData(const std::string& uniformName, GLfloat data);

	bool SendUniformData(const std::string& uniformName, GLfloat x, GLfloat y);
	bool SendUniformData(const std::string& uniformName, GLfloat x, GLfloat y, GLfloat z);
	bool SendUniformData(const std::string& uniformName, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

	bool SendUniformData(const std::string& uniformName, const glm::mat4& data);


};
#endif