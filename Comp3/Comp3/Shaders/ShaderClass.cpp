#include "ShaderClass.h"
#include <string>


// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (!in.is_open()) {
		std::cerr << "Error opening file: " << filename << std::endl;
		return "";
	}

	try {
		std::ostringstream contents;
		contents << in.rdbuf();
		in.close();
		return contents.str();
	}
	catch (const std::exception& e) {
		std::cerr << "Exception while reading file: " << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Unknown exception occurred while reading file." << std::endl;
	}
	return "";


	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);

}

ShaderClass::ShaderClass()
{
	shaderID = 0;
	
}

// Constructor that build the Shader Program from 2 different shaders
ShaderClass::ShaderClass(const char* vertexFile, const char* fragmentFile)
{
	// Read vertexFile and fragmentFile and store the strings
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Convert the shader source strings into character arrays
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);

	// Create Shader Program Object and get its reference
	shaderID = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(shaderID, vertexShader);
	glAttachShader(shaderID, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(shaderID);

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

// Activates the Shader Program
void ShaderClass::Activate()
{
	glUseProgram(shaderID);
}

// Deletes the Shader Program
void ShaderClass::Delete()
{
	glDeleteProgram(shaderID);
}


bool ShaderClass::SendUniformData(const std::string& uniformName, GLint data)
{
	GLint ID = glGetUniformLocation(shaderID, uniformName.c_str());

	if (ID == -1)
	{
		std::cout << "Shader variable " << uniformName << " not found or not used." << std::endl;
		return false;
	}

	glUniform1i(ID, data);
	return true;
}

bool ShaderClass::SendUniformData(const std::string& uniformName, GLuint data)
{
	GLint ID = glGetUniformLocation(shaderID, uniformName.c_str());

	if (ID == -1)
	{
		std::cout << "Shader variable " << uniformName << " not found or not used." << std::endl;
		return false;
	}

	glUniform1ui(ID, data);
	return true;
}

bool ShaderClass::SendUniformData(const std::string& uniformName, GLfloat data)
{
	GLint ID = glGetUniformLocation(shaderID, uniformName.c_str());

	if (ID == -1)
	{
		std::cout << "Shader variable " << uniformName << " not found or not used." << std::endl;
		return false;
	}

	glUniform1f(ID, data);
	return true;
}

bool ShaderClass::SendUniformData(const std::string& uniformName, GLfloat x, GLfloat y)
{
	GLint ID = glGetUniformLocation(shaderID, uniformName.c_str());

	if (ID == -1)
	{
		std::cout << "Shader variable " << uniformName << " not found or not used." << std::endl;
		return false;
	}

	glUniform2f(ID, x, y);
	return true;
}

bool ShaderClass::SendUniformData(const std::string& uniformName, GLfloat x, GLfloat y, GLfloat z)
{
	GLint ID = glGetUniformLocation(shaderID, uniformName.c_str());

	if (ID == -1)
	{
		std::cout << "Shader variable " << uniformName << " not found or not used." << std::endl;
		return false;
	}

	glUniform3f(ID, x, y, z);
	return true;
}

bool ShaderClass::SendUniformData(const std::string& uniformName, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	GLint ID = glGetUniformLocation(shaderID, uniformName.c_str());

	if (ID == -1)
	{
		std::cout << "Shader variable " << uniformName << " not found or not used." << std::endl;
		return false;
	}

	glUniform4f(ID, x, y, z, w);
	return true;
}

bool ShaderClass::SendUniformData(const std::string& uniformName, const glm::mat4& data)
{
	GLint ID = glGetUniformLocation(shaderID, uniformName.c_str());

	if (ID == -1)
	{
		std::cout << "Shader variable " << uniformName << " not found or not used." << std::endl;
		return false;
	}

	glUniformMatrix4fv(ID, 1, GL_FALSE, &data[0][0]);
	return true;
}

//
//
//// Reads a text file and outputs a string with everything in the text file
//std::string get_file_contents(const char* filename)
//{
//	std::ifstream in(filename, std::ios::binary);
//	if (!in.is_open()) {
//		std::cerr << "Error opening file: " << filename << std::endl;
//		return "";
//	}
//
//	try {
//		std::ostringstream contents;
//		contents << in.rdbuf();
//		in.close();
//		return contents.str();
//	}
//	catch (const std::exception& e) {
//		std::cerr << "Exception while reading file: " << e.what() << std::endl;
//	}
//	catch (...) {
//		std::cerr << "Unknown exception occurred while reading file." << std::endl;
//	}
//	return "";
//
//
//	if (in)
//	{
//		std::string contents;
//		in.seekg(0, std::ios::end);
//		contents.resize(in.tellg());
//		in.seekg(0, std::ios::beg);
//		in.read(&contents[0], contents.size());
//		in.close();
//		return(contents);
//	}
//	throw(errno);
//
//}

//// Constructor that build the Shader Program from 2 different shaders
//Shader::Shader(const char* vertexFile, const char* fragmentFile)
//{
//	// Read vertexFile and fragmentFile and store the strings
//	std::string vertexCode = get_file_contents(vertexFile);
//	std::string fragmentCode = get_file_contents(fragmentFile);
//
//	// Convert the shader source strings into character arrays
//	const char* vertexSource = vertexCode.c_str();
//	const char* fragmentSource = fragmentCode.c_str();
//
//	// Create Vertex Shader Object and get its reference
//	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//	// Attach Vertex Shader source to the Vertex Shader Object
//	glShaderSource(vertexShader, 1, &vertexSource, NULL);
//	// Compile the Vertex Shader into machine code
//	glCompileShader(vertexShader);
//
//	// Create Fragment Shader Object and get its reference
//	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//	// Attach Fragment Shader source to the Fragment Shader Object
//	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
//	// Compile the Vertex Shader into machine code
//	glCompileShader(fragmentShader);
//
//	// Create Shader Program Object and get its reference
//	ID = glCreateProgram();
//	// Attach the Vertex and Fragment Shaders to the Shader Program
//	glAttachShader(ID, vertexShader);
//	glAttachShader(ID, fragmentShader);
//	// Wrap-up/Link all the shaders together into the Shader Program
//	glLinkProgram(ID);
//
//	// Delete the now useless Vertex and Fragment Shader objects
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//
//}
//
//// Activates the Shader Program
//void Shader::Activate()
//{
//	glUseProgram(ID);
//}
//
//// Deletes the Shader Program
//void Shader::Delete()
//{
//	glDeleteProgram(ID);
//}
