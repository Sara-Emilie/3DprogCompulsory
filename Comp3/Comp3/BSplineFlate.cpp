#include "BSplineFlate.h"
#include <stdexcept>



glm::vec3 BSplineFlate::evaluateBiquadratic(int my_u, int my_v, glm::vec3 bu, glm::vec3 bv)
{
    glm::vec3 r{ 0.0f, 0.0f, 0.0f }; // Initialize result
    // Iterate through the control points and basis functions
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 3; i++) {
            // Check for out-of-range access in the control points array
           /* if (my_u + i < 0 || my_u + i >= 4 || my_v + j < 0 || my_v + j >= 3) {
                throw std::out_of_range("Index out of range in evaluateBiquadratic");
            }*/
            // B-spline surface equation: sum of basis functions times control points
            r += bu[i] * bv[j] * c[my_u + i][my_v + j];
        }
    }
    return r;
}

void BSplineFlate::makeBiquadraticSurface()
{
    float h = 0.1f; // Parameter spacing for u and v
    int nu = 4;     // Number of intervals in u-direction
    int nv = 3;     // Number of intervals in v-direction

    // Clear any existing vertex data
    mVertices.clear();
    mIndices.clear();

    // Loop over the grid in parameter space (u, v)
    for (int i = 0; i < nv; i++)
    {
        for (int j = 0; j < nu; j++)
        {
            // Parameter values (u, v)
            float u = j * h; // u parameter
            float v = i * h; // v parameter

            // Find the knot intervals for u and v
            int my_u = findKnotInterval(mu, d_u, n_u, u);
            int my_v = findKnotInterval(mv, d_v, n_v, v);

            // Ensure that the knot intervals found are valid
            if (my_u < 0 || my_u >= n_u || my_v < 0 || my_v >= n_v) {
                throw std::out_of_range("Knot interval out of range in makeBiquadraticSurface");
            }

            // Evaluate the B-spline surface point at (u, v)
            auto koeff_par = B2(u, v, my_u, my_v);
            glm::vec3 position = evaluateBiquadratic(my_u, my_v, koeff_par.first, koeff_par.second);

            // Compute partial derivatives for the surface to get tangents
            auto dKoeffPar = B2(u, v, my_u, my_v);
            glm::vec3 du = evaluateBiquadratic(my_u, my_v, dKoeffPar.first, koeff_par.second);
            glm::vec3 dv = evaluateBiquadratic(my_u, my_v, koeff_par.first, dKoeffPar.second);
            glm::vec3 normal = glm::normalize(glm::cross(du, dv)); // Normal from cross product of tangents

            // Texture coordinates (u and v normalized between 0 and 1)
            glm::vec2 texCoord(u / (nu * h), v / (nv * h));

            // Color (can be based on position or fixed color)
            glm::vec3 color(1.0f, 0.5f, 0.5f); // Example: light red

            // Push vertex data to the mVertices vector
            mVertices.push_back(Vertex{ position, normal, texCoord, color });
        }
    }

    // Now generate the indices for the surface
    generateIndices(nu, nv);
}

void BSplineFlate::generateIndices(int nu, int nv)
{
    for (int i = 0; i < nv - 1; i++) // Loop through rows
    {
        for (int j = 0; j < nu - 1; j++) // Loop through columns
        {
            // Calculate the indices of the four corner points of the current grid cell
            int idx0 = i * nu + j;        // Bottom-left corner
            int idx1 = idx0 + 1;          // Bottom-right corner
            int idx2 = (i + 1) * nu + j;  // Top-left corner
            int idx3 = idx2 + 1;          // Top-right corner

            // Ensure calculated indices are valid
            if (idx0 < 0 || idx0 >= mVertices.size() ||
                idx1 < 0 || idx1 >= mVertices.size() ||
                idx2 < 0 || idx2 >= mVertices.size() ||
                idx3 < 0 || idx3 >= mVertices.size()) {
                throw std::out_of_range("Index out of range in generateIndices");
            }

            // First triangle (bottom-left, top-left, bottom-right)
            mIndices.push_back(idx0);
            mIndices.push_back(idx2);
            mIndices.push_back(idx1);

            // Second triangle (bottom-right, top-left, top-right)
            mIndices.push_back(idx1);
            mIndices.push_back(idx2);
            mIndices.push_back(idx3);
        }
    }
}

int BSplineFlate::findKnotInterval(const std::vector<float>& knotVector, int degree, int numControlPoints, float t)
{
    // Knot vector length should be (numControlPoints + degree + 1)
    int n = numControlPoints + degree;

    // Handle special case when t is exactly at the end of the knot vector (clamped knot)
    if (t == knotVector[n])
        return n - 1;

    // Find the knot interval where t lies: knotVector[k] <= t < knotVector[k+1]
    for (int k = degree; k <= n; k++) {
        if (t >= knotVector[k] && t < knotVector[k + 1]) {
            return k;
        }
    }

    // If no valid interval is found, return an error or a default value.
    return -1; // This shouldn't happen in theory unless there's an issue with the knot vector
}

BSplineFlate::BSplineFlate()
{
    mu = { 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 2.0f, 2.0f };
    mv = { 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 2.0f, 2.0f };

    // Example of initializing control points
    c[0][0] = glm::vec3(0.0f, 0.0f, 0.0f); // Control Point 1
    c[0][1] = glm::vec3(1.0f, 0.0f, 0.0f); // Control Point 2
    c[0][2] = glm::vec3(0.5f, 1.0f, 0.0f); // Control Point 3
    c[1][0] = glm::vec3(0.0f, 1.0f, 0.0f); // Control Point 4
    c[1][1] = glm::vec3(1.0f, 1.0f, 0.0f); // Control Point 5
    c[1][2] = glm::vec3(0.5f, 2.0f, 0.0f); // Control Point 6
    // Add more control points as needed...


 
    makeBiquadraticSurface();
}

std::vector<Vertex>& BSplineFlate::getvert()
{
    return mVertices;
}

std::vector<unsigned int>& BSplineFlate::getindi()
{
    return mIndices;
}

std::pair<glm::vec3, glm::vec3> BSplineFlate::B2(float tu, float tv, int my_u, int my_v)
{
    // Knot vectors for u and v
    //std::vector<float> mu = { 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 2.0f, 2.0f };
    //std::vector<float> mv = { 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 2.0f, 2.0f };

    glm::vec3 dBu{ 0.0f, 0.0f, 0.0f }; // Derivative of Bu with respect to u
    glm::vec3 dBv{ 0.0f, 0.0f, 0.0f }; // Derivative of Bv with respect to v

    // Check bounds before accessing NpDerivative
    if (my_u < 0 || my_u + 2 >= mu.size() || my_v < 0 || my_v + 2 >= mv.size()) {
        throw std::out_of_range("Index out of range in B2 function");
    }

    // Loop to compute the derivatives for Bu and Bv
    for (int i = 0; i < 3; i++) {
        // Compute the derivative of the B-spline basis functions
        dBu[i] = NpDerivative(2, my_u + i, tu, mu); // Assuming degree 2 for Bu
        dBv[i] = NpDerivative(2, my_v + i, tv, mv); // Assuming degree 2 for Bv
    }

    return std::make_pair(dBu, dBv);
}

float BSplineFlate::NpDerivative(int degree, int i, float t, const std::vector<float>& knotVector)
{
    // Base case: The derivative of a degree-0 basis function is 0
    if (degree == 0) {
        return 0.0f;
    }

    // Ensure that we are within the bounds of the knot vector
    if (i < 0 || i >= knotVector.size() - degree) {
        throw std::out_of_range("Index i is out of bounds in NpDerivative");
    }

    float left = 0.0f;
    float right = 0.0f;

    // Check if the division is valid for the left term
    if (i + degree < knotVector.size() && knotVector[i + degree] != knotVector[i]) {
        left = degree / (knotVector[i + degree] - knotVector[i]) * NpDerivative(degree - 1, i, t, knotVector);
    }

    // Check if the division is valid for the right term
    if (i + degree + 1 < knotVector.size() && knotVector[i + degree + 1] != knotVector[i + 1]) {
        right = degree / (knotVector[i + degree + 1] - knotVector[i + 1]) * NpDerivative(degree - 1, i + 1, t, knotVector);
    }

    return left - right;
}


