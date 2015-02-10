#include <stdio.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <vector>
#include <math.h>

using namespace std;

//-----------------------
// function declarations 
//-----------------------

//Calculate the cloths position in the next frame using Eulermethod. Input is the cloths position in the current frame
vector<glm::vec3> Euler(vector<glm::vec3> particlesCurPos);

// Place the particles in a coordinate system, cloth in its initial state, the first particle in origo.
void placeParticles(const int nrOfParticlesHorizontally, const int nrOfParticlesVertically, const float springRestLenght, vector<glm::vec3> &coordinates);