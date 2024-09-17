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



};

