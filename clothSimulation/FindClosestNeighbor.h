#include <vector>
#include <math.h>
#include <glm/glm.hpp>
#include <algorithm>
# include <iostream>

using namespace std;

int FindClosestNeighbor(vector<glm::vec3> massPositions, double mousePosX, double mousePosY,
				const int clippingPlaneNear, const float maxSpringLength);

struct CloseMasses{
	float orthogonalDistW;
	float parallelDistV;
	int number;
};