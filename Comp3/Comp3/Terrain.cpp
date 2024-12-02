#include "Terrain.h"
#include "delaunator.hpp"

Terrain::Terrain()
{
	//ReadFile("HøydeData/Fil.txt");
	ReadFile("HøydeData/Fil4.txt");
	MakeTriangles(this->vertices);
	
}

void Terrain::MakeTriangles(std::vector<Vertex> vertices)
{

	for (Vertex v : vertices) 
	{
		points.push_back((double)v.pos.x);
		points.push_back((double)v.pos.z);
	}

	delaunator::Delaunator d(points); // https://github.com/delfrrr/delaunator-cpp

	for (int i = 0; i < d.triangles.size(); i += 3) 
	{
		indices.push_back(d.triangles[i]);
		indices.push_back(d.triangles[i+1]);
		indices.push_back(d.triangles[i+2]);

		size_t i1 = d.triangles[i];
		size_t i2 = d.triangles[i + 1];
		size_t i3 = d.triangles[i + 2];

		// Retrieve the vertices
		Vertex v1 = vertices[i1];
		Vertex v2 = vertices[i2];
		Vertex v3 = vertices[i3];

		// Create a Triangle struct and add it to the triangles vector
		Triangle t1 = { v1, v2, v3 };
		triangles.push_back(t1);



	}
}

std::vector<Vertex>& Terrain::getvert()
{
	return this->vertices;
}

std::vector<GLuint>& Terrain::getindi()
{
	return this->indices;
}
