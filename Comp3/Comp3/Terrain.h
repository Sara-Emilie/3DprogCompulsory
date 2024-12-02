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
	std::vector<GLuint> indices;
	std::vector<double> points;

	float xoffset, yoffset, zoffset;
	int highest{ 0 };

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
			
			if (y > highest) 
			{
				highest = y;
			}
			/*glm::vec3 pos;
			glm::vec3 normal;
			glm::vec2 tex;
			glm::vec3 col;*/

		/*	glm::vec3 col(0.1 * y, 0.2 * y, 0.1 * y);
			col = glm::normalize(col);*/
			float normalizedHeight = y / 6936000.0f; // Normalize height (0 to 1)

			// Map the green channel to normalizedHeight
			glm::vec3 col(0.1f, 0.1f + 0.9f * normalizedHeight, 0.1f); // Base green increases with height

			col = glm::normalize(col);


			vertices.emplace_back(Vertex{
				glm::vec3(x - xoffset, z - zoffset, y - yoffset),
				glm::vec3(1.0),
				glm::vec2(1.0),
				col
				})
				;

		}

		std::cout << highest << std::endl;
	}

	void MakeTriangles(std::vector<Vertex> vertices);

	std::vector<Vertex>& getvert();
	std::vector<GLuint>& getindi();

};

