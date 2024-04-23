#include "Buffer.h"
#include "Shaders/ShaderClass.h"

ShaderClass shader;

Buffer::Buffer()
{
	VAO = 0;
	EBO = 0;
	vertexVBO = 0;
	colorVBO = 0;
	textureVBO = 0;
	totalVertices = 0;
	hasEBO = false;
}

void Buffer::CreateBuffer(GLuint totalVertices, bool hasEBO)
{
	glGenBuffers(1, &vertexVBO);
	glGenBuffers(1, &colorVBO);
	glGenBuffers(1, &textureVBO);
	glGenVertexArrays(1, &VAO);

	if (hasEBO)
	{
		glGenBuffers(1, &EBO);
		hasEBO = hasEBO;
	}

	totalVertices = totalVertices;
}

void Buffer::FillEBO(const GLuint* data, GLsizeiptr bufferSize, FillType fill)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, data, static_cast<GLenum>(fill));
}

void Buffer::FillVBO(VBOType vboType, GLfloat* data, GLsizeiptr bufferSize, FillType fillType)
{
	glBindVertexArray(VAO);

		if (vboType == VBOType::VertexBuffer)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
		}

		else if(vboType == VBOType::ColorBuffer)
		{
			glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		}

		else
		{
			glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
		}

		glBufferData(GL_ARRAY_BUFFER, bufferSize, data, static_cast<GLenum>(fillType));

	glBindVertexArray(0);
}

void Buffer::LinkEBO()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBindVertexArray(0);
}

void Buffer::LinkVBO(const std::string& attribute, VBOType vboType, ComponentType componentType, DataType dataType)
{
	
	GLuint shaderProgramID = shader.GetShaderID();

	GLint ID = glGetAttribLocation(shaderProgramID, attribute.c_str());

	glBindVertexArray(VAO);

		if (vboType == VBOType::VertexBuffer)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
		}

		else if (vboType == VBOType::ColorBuffer)
		{
			glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		}

		else
		{
			glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
		}

		glVertexAttribPointer(ID, static_cast<GLint>(componentType), GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(ID);

	glBindVertexArray(0);

}

void Buffer::Render(DrawType drawType)
{
	glBindVertexArray(VAO);

	if (hasEBO)
	{
		glDrawElements(static_cast<GLenum>(drawType),
			totalVertices, GL_UNSIGNED_INT, nullptr);
	}

	else
	{
		glDrawArrays(static_cast<GLenum>(drawType), 0, totalVertices);
	}

	glBindVertexArray(0);
}

void Buffer::DestroyBuffer()
{
	glDeleteBuffers(1, &vertexVBO);
	glDeleteBuffers(1, &colorVBO);
	glDeleteBuffers(1, &textureVBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);
}