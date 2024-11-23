#include <fstream>
#include <iostream>
#include <vector>
#include "sstream"
#include <string>
#include "Shaders/VBO.h"

#pragma once

using namespace std;

class Terrain
{

public:
	Terrain();

	std::vector<Vertex> vertices;
	float xoffset, yoffset, zoffset;

	void ReadFile(const std::string& path)
	{
		std::ifstream file(path);
		if (!file.is_open())
		{
			std::cout << "Failed to load the file: " << path << std::endl;
			return;
		}


		std::string line;


		if (getline(file, line))
		{
			istringstream iss(line);
			float numberofpoints;

			if (!(iss >> numberofpoints)) 
			{
				std::cout << "Could not get number of points" << endl;
			}

		}

		if (getline(file, line))
		{
			istringstream iss(line);

			if (!(iss >> xoffset >> yoffset >> zoffset))
			{
				std::cout << "Could not load first point" << endl;
			}
		}

		while (getline(file, line))
		{
			istringstream iss(line);
			float x, y, z;

			if (!(iss >> x >> y >> z))
			{
				std::cout << "Lost one point" << endl;
				continue;
			}
			

			vertices.emplace_back(Vertex{
				glm::vec3(x - xoffset, z - zoffset, y - yoffset),
				glm::vec3(1.0),
				glm::vec2(1.0),
				glm::vec3(1.0)
				})
				;

		}

	}

	std::vector<Vertex>& getvert();

};

