#include <stdio.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <vector>
#include <math.h>

using namespace std;

//-----------------------
// function declarations 
//-----------------------
void placeParticles(const float nrOfParticlesHorizontally, const float nrOfParticlesVertically, const float springRestLenght, vector<glm::vec3> &coordinates);

//-----------------------
// variable declarations 
//-----------------------
vector<glm::vec3> particles;

int Euler(){





}


/*
** Place the particles in a coordinate system, cloth in its initial state, the first particle in origo.
*/
void placeParticles(const float nrOfParticlesHorizontally, const float nrOfParticlesVertically, const float springRestLenght, vector<glm::vec3> &particles) {

	int row = 0;
	glm::vec3 tempVec = glm::vec3(0.f); // vec3 for holding the particles coordinates untill it can be push_backed to particles

	for (int i = 0; i < nrOfParticlesVertically*nrOfParticlesHorizontally; i++) {
		tempVec.x = springRestLenght * i - nrOfParticlesHorizontally*row; // X-coordinate

		//check if it is the last particle on the row
		if (fmod(i, nrOfParticlesHorizontally) == 0) {
			tempVec.y = row; // Y-coordinate
			row += springRestLenght;
		}

		else
			tempVec.y = row; // Y-coordinate

		particles.push_back(tempVec);
	}
}