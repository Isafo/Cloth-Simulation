#include "Particles.h"

// Calculate the cloths position in the next frame using Eulermethod. Input is the cloths position in the current frame

void Euler(vector<glm::vec3> &particle, vector<glm::vec3> &particle_old, vector<glm::vec3> &velocity, vector<glm::vec3> &velocity_old){
	
	vector<glm::vec3> particlesNextPos;
	particlesNextPos = placeZeros();

	glm::vec3 kUpp;
	glm::vec3 cUpp;
	glm::vec3 kVanster;
	glm::vec3 cVanster;
	glm::vec3 kRigth;
	glm::vec3 cRigth;
	glm::vec3 kDown;
	glm::vec3 cDown;
	

	int staticPoint1 = nrOfParticlesHorizontally*nrOfParticlesVertically - 1;
	int staticPoint2 = nrOfParticlesHorizontally*(nrOfParticlesVertically - 1);
	//int staticPoint1 = 0;
	//int staticPoint2 = nrOfParticlesHorizontally - 1;

	for (int j = 0; j < nrOfParticlesHorizontally*nrOfParticlesVertically; j++){
		// if we are on the top row of the fabric
		if (j <= nrOfParticlesHorizontally-1){
			kUpp = glm::vec3(0.f, 0.f, 0.f);
			cUpp = glm::vec3(0.f, 0.f, 0.f);
		} else{ // else add force from the spring and damper attatch to the paricle above
			kUpp = (particle_old[j - nrOfParticlesHorizontally] - particle_old[j])*((norm(particle_old[j - nrOfParticlesHorizontally] - particle_old[j]) - springRestLenght) / norm(particle_old[j - nrOfParticlesHorizontally] - particle_old[j]));
			cUpp = velocity_old[j - nrOfParticlesHorizontally] - velocity_old[j];
		}
		
		// if we are on the far left column of the fabric
		if (j % nrOfParticlesHorizontally == 0){
			kVanster = glm::vec3(0.f, 0.f, 0.f);
			cVanster = glm::vec3(0.f, 0.f, 0.f);
		} else{
			kVanster = (particle_old[j - 1] - particle_old[j])*((norm(particle_old[j - 1] - particle_old[j]) - springRestLenght) / norm(particle_old[j - 1] - particle_old[j]));
			cVanster = velocity_old[j - 1] - velocity_old[j];
		}

		// if we are on the far rigth column of the fabric
		if (j % nrOfParticlesHorizontally == nrOfParticlesHorizontally -1){
			kRigth = glm::vec3(0.f, 0.f, 0.0f);
			cRigth = glm::vec3(0.f, 0.f, 0.0f);
		} else{
			kRigth = (particle_old[j + 1] - particle_old[j])*((norm(particle_old[j + 1] - particle_old[j]) - springRestLenght) / norm(particle_old[j + 1] - particle_old[j]));
			cRigth = velocity_old[j + 1] - velocity_old[j];
		}

		// if we are on the bottom row of the fabric
		if (j >= nrOfParticlesHorizontally*(nrOfParticlesVertically - 1)){
			kDown = glm::vec3(0.f, 0.f, 0.0f);
			cDown = glm::vec3(0.f, 0.f, 0.0f);
		} else{
			kDown = (particle_old[j + nrOfParticlesHorizontally] - particle_old[j])*((norm(particle_old[j + nrOfParticlesHorizontally] - particle_old[j]) - springRestLenght) / norm(particle_old[j + nrOfParticlesHorizontally] - particle_old[j]));
			cDown = velocity_old[j + nrOfParticlesHorizontally] - velocity_old[j];
		}

		if (j == staticPoint1 || j == staticPoint2){//! detta borde vara en vecktor så att man kan fylla på med statiska punkter, även ta bort statiska ;)
			particlesNextPos[j] = particle[j];
			velocity[j] = velocity_old[j];
		} else{
			//calculate the new velosity
			velocity[j] = velocity_old[j] + (timestep / particleMass)*(-particleMass*g + k*(kUpp + kVanster + kRigth + kDown) + c*(cUpp + cVanster + cRigth + cDown));

			//calculate the new possition
			particlesNextPos[j] = particle[j] + timestep*velocity[j];
		}
		//cout << "particel " << j << ".x = " << particlesNextPos[j].x << " .y = " << particlesNextPos[j].y << " .z = " << particlesNextPos[j].z << endl;
		//cout << "velocity " << j << ".x = " << velocity[j].x << " .y = " << velocity[j].y << " .z = " << velocity[j].z << endl;
	}
	//cout << endl;
	
	//update to new values
	velocity_old = velocity;
	particle_old = particle;
	particle = particlesNextPos;
}


// Place a vector whith zeros, cloth velosity in its initial state.
vector<glm::vec3> placeZeros(){
	vector<glm::vec3> zeroVector;
	for (int i = 0; i < nrOfParticlesVertically*nrOfParticlesHorizontally; i++) {
		zeroVector.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	}
	return zeroVector;
}

// Place the particles in a coordinate system, cloth in its initial state, the first particle in origo.
void placeParticles(vector<glm::vec3> &particles) {

	float row = 0;
	glm::vec3 tempVec = glm::vec3(0.f); // vec3 for holding the particles coordinates untill it can be push_backed to particles

	for (int  i = 0; i < nrOfParticlesVertically*nrOfParticlesHorizontally; i++) {

		//check if it is the first particle on a new row
		if (i != 0 && i % nrOfParticlesHorizontally == 0) {
			row += 1;
		} 

		tempVec.x =  springRestLenght * (i - nrOfParticlesHorizontally*row); // X-coordinate
		tempVec.y= 0; // y-coordinate
		tempVec.z =  springRestLenght * row; // z-coordinate

		cout << "particel " << ".x = " << tempVec.x << " .y = " << tempVec.y << " .z = " << tempVec.z << endl;

		particles.push_back(tempVec);
	}
}
        
// Takes in a vector of Coordinates for each particle 
// in the cloth and stores them in a new vector   
// in the order they should be used as triangle   
// corners when drawing the cloth. To ensure the
// normal points in the correct direction
vector<glm::vec3> MakeTriangles(vector<glm::vec3> Coord)
{
	vector<glm::vec3> order;

	//width-1 because we dont need to look at the 
	//last row of masses of the cloth
	//and not the last mass of the second last row either
	for (int j = 0; j < (nrOfParticlesHorizontally - 1)*nrOfParticlesVertically - 1; j++)
	{
		//if we have reached the rigth side of the cloth 
		//we skip to the next mass
		if ((j+1)%nrOfParticlesHorizontally != 0)
		{
			order.push_back(Coord[j]);
			order.push_back(Coord[j + 1]);
			order.push_back(Coord[nrOfParticlesHorizontally + j]);
			order.push_back(Coord[j + 1]);
			order.push_back(Coord[nrOfParticlesHorizontally + j + 1]);
			order.push_back(Coord[nrOfParticlesHorizontally + j]);
		}
	}
	return order;
}

// norm = sqrt(x^2+y^2+x^2) 
float norm(glm::vec3 vec){
	return  sqrt(pow(vec.x, 2.0) + pow(vec.y, 2.0) + pow(vec.z, 2.0));
}