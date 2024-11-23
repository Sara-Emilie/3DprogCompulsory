#include "Terrain.h"

Terrain::Terrain()
{
	//ReadFile("HøydeData/Fil.txt");
	ReadFile("HøydeData/Fil2.txt");
}

std::vector<Vertex>& Terrain::getvert()
{
	return this->vertices;
}
