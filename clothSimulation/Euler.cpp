#include "Euler.h"


/*
** Calculate the cloths position in the next frame using Eulermethod. Input is the cloths position in the current frame
*/
vector<glm::vec3> Euler(vector<glm::vec3> particlesCurPos){

	vector<glm::vec3> particlesNextPos;


	return particlesNextPos;
}


/*
** Place the particles in a coordinate system, cloth in its initial state, the first particle in origo.
*/
void placeParticles(const int nrOfParticlesHorizontally, const int nrOfParticlesVertically, const float springRestLenght, vector<glm::vec3> &particles) {

	int row = 0;
	glm::vec3 tempVec = glm::vec3(0.f); // vec3 for holding the particles coordinates untill it can be push_backed to particles

	for (float i = 0; i < nrOfParticlesVertically*nrOfParticlesHorizontally; i++) {
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