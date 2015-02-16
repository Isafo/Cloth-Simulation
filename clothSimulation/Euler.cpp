#include "Euler.h"


/*
** Calculate the cloths position in the next frame using Eulermethod. Input is the cloths position in the current frame
*/
vector<glm::vec3> Euler(vector<glm::vec3> particlesCurPos){

	vector<glm::vec3> particlesNextPos = particlesCurPos;


	return particlesNextPos;
}

/*
** Place the particles in a coordinate system, cloth in its initial state, the first particle in origo.
*/
void placeParticles(const int nrOfParticlesHorizontally, const int nrOfParticlesVertically, const float springRestLenght, vector<glm::vec3> &particles) {

	float row = 0;
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


/*
** MakeTriangles                                  
** Takes in a vector of Coordinates for each mass 
** in the cloth and stores them in a new vector   
** in the order they should be used as triangle   
** corners when drawing the cloth.                
*/
vector<glm::vec3> MakeTriangles(vector<glm::vec3> C, int width, int height)
{
	vector<glm::vec3> order;

	//width-1 because we dont need to look at the 
	//last row of masses of the cloth
	//and not the last mass of the second last row either
	for (int j = 0; j < (width - 1)*height - 1; j++)
	{
		//if we have reached the rigth side of the cloth 
		//we skip to the next mass
		if (j%width != 0)
		{
			order.push_back(C[j]);
			order.push_back(C[width + j]);
			order.push_back(C[j + 1]);
			order.push_back(C[j + 1]);
			order.push_back(C[width + j]);
			order.push_back(C[width + j + 1]);
		}
	}

	return order;
}




