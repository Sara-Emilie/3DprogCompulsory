#include <fstream>
#include <iostream>
#include <vector>
#include "sstream"
#include <string>
#include "Shaders/VBO.h"

#include <random>

#pragma once

using namespace std;

class Terrain
{

public:
	Terrain();

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<double> points;

	struct Triangle
	{
		Vertex v1, v2, v3;
		glm::vec3 normal;
	};

	std::vector<Triangle> triangles;
	int skip = 0;
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

			if (skip == 3000)
			{
				//vertex format:
				/*glm::vec3 pos;
				glm::vec3 normal;
				glm::vec2 tex;
				glm::vec3 col;*/

				//Random number to make terrain visable
				std::random_device rd;
				std::mt19937 generator(rd());
				std::uniform_real_distribution<double> distribution(0.1, 0.3);
				double random_number = distribution(generator);
				double random_number2 = distribution(generator);
				//double random_number3 = distribution(generator);

				glm::vec3 col(0.1f, 0.1f + random_number, 0.1f + random_number2);
				col = glm::normalize(col);

				float partDerivertX = -y;
				float partDerivertY = -x;

				glm::vec3 u(1, 0, partDerivertX);
				glm::vec3 v(0, 1, partDerivertY);

				glm::vec3 normal = glm::normalize(glm::cross(u, v));

				vertices.emplace_back(Vertex{
					glm::vec3(x - xoffset, z - zoffset, y - yoffset),
					normal,
					glm::vec2(1.0),
					col
					})
					;
				skip = 0;
			}
			else { skip++; }
			
		}
	}

	void MakeTriangles(std::vector<Vertex> vertices);

	std::vector<Vertex>& getvert();
	std::vector<GLuint>& getindi();

};

