#include "Plane.h"
#include "stb_image.h"

Plane::Plane()
{
	GLfloat vertices[] =
	{

		-0.5, 0, 0.5,		0,1,0,	 0,1,
		0.5, -0.2, 0.5,		0,1,0,	 1,1,
		0.5, 0, -0.5,		0,1,0,	 1,0,
		-0.5, -0.2, -0.5,	0,1,0,	 0,0

	};

	GLuint indices[] =
	{
		0,1,2,
		0,2,3
	};

	Vertex v1 = { -0.5, 0, 0.5,			0,1,0,	 0,1 };
	Vertex v2 = { 0.5, -0.2, 0.5,		0,1,0,	 1,1 };
	Vertex v3 = { 0.5, 0, -0.5,			0,1,0,	 1,0 };
	Vertex v4 = { -0.5, -0.2, -0.5,		0,1,0,	 0,0 };

	Triangle t1 = { v1, v2, v3 };
	Triangle t2 = { v1, v3, v4 };

	triangles.push_back(t1);
	triangles.push_back(t2);


	
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	//unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load("Textures/dirt.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	//unsigned int textureSpecular;
	glGenTextures(1, &textureSpecular);
	glBindTexture(GL_TEXTURE_2D, textureSpecular);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	data = stbi_load("Textures/dirt_specular.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

Plane::~Plane()
{
}


void Plane::Render() 
{
	glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE1); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, textureSpecular);

	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 4);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


}