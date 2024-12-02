#include "Terrain.h"
#include "delaunator.hpp"

Terrain::Terrain()
{
	//ReadFile("HøydeData/Fil.txt");
	ReadFile("HøydeData/Fil3.txt");
	MakeTriangles(this->vertices);
	
}

void Terrain::MakeTriangles(std::vector<Vertex> vertices)
{

	for (Vertex v : vertices) 
	{
		points.push_back((double)v.pos.x);
		points.push_back((double)v.pos.z);
	}

	delaunator::Delaunator d(points);

	for (int i = 0; i < d.triangles.size(); i += 3) 
	{
		indices.push_back(d.triangles[i]);
		indices.push_back(d.triangles[i+1]);
		indices.push_back(d.triangles[i+2]);
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
