#pragma once
#include "vector"
#include "Shaders/VBO.h"
class Shape
{
public:
	enum Type {
		CUBE,
		OCTAHEDRON,
		SUBDIVIDED_OCTAHEDRON
	};


	std::vector<Vertex> vert;
	std::vector<GLuint> indi;

	Shape(Type shape);

	std::vector<Vertex>& getvert();
	std::vector<GLuint>& getindi();

	void SubDivideOcta(int n);
	void SubDivide(const glm::vec3 a, const glm::vec3 b, const glm::vec3 c, int n);
	void makeTriangle(const glm::vec3 a, const glm::vec3 b, const glm::vec3 c);

};

