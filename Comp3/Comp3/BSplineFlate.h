#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "Shaders/VBO.h"
#include "Shaders/ShaderClass.h"

class BSplineFlate
{
public:

	void generateIndices(int nu, int nv);
	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;
	float NpDerivative(int degree, int i, float t, const std::vector<float>& knotVector);
	void drawSurface(ShaderClass& shaderProgram, BSplineFlate& spline);

	private:
	int n_u = 4; // antall kontrollpunkter for u
	int n_v = 3; // antall kontrollpunkter for v
	int d_u = 2;
	int d_v = 2;
	float hu = 0.1f , hv = 0.1f;
	std::vector<float> mu; //sjotvektor u
	std::vector<float> mv; // sjotvektor v

	std::vector<glm::vec3> mc; // kontrollpunktenelinjevis u=retning
	glm::vec3 c[4][3]; // eller i to dimansjonal matirse
	std::pair <glm::vec3 , glm::vec3> B2(float tu, float tv , int my_u, int my_v);
	// Hardkodet for grad d_u = d_v = 2
	glm::vec3 evaluateBiquadratic(int my_u, int my_v, glm::vec3 bu , glm::vec3 bv);
	void makeBiquadraticSurface();
	int findKnotInterval(const std::vector<float>& knotVector, int degree, int numControlPoints, float t);
	public:
		BSplineFlate();

		std::vector<Vertex>& getvert();
		std::vector<unsigned int>& getindi();

};

