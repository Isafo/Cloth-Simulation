#include "Particles.h"

// Calculate the cloths position in the next frame using Eulermethod. Input is the cloths position in the current frame

vector<glm::vec3> Euler(vector<glm::vec3> particle, vector<glm::vec3> particle_old, vector<glm::vec3> velocity, vector<glm::vec3> velocity_old){
	
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

	//for (int time = 0; h*endTime; time++)
	//{
	for (int j = 0; j < nrOfParticlesHorizontally*nrOfParticlesVertically; j++){
		// if we are on the top row of the fabric
		if (j <= nrOfParticlesHorizontally){
			kUpp = glm::vec3(0.f, 0.f, 0.0f);
			cUpp = glm::vec3(0.f, 0.f, 0.0f);
		} else{ // else add force from the spring and damper attatch to the paricle above
			kUpp = (particle_old[j - nrOfParticlesHorizontally] - particle_old[j])*((norm(particle_old[j - nrOfParticlesHorizontally] - particle_old[j]) - springRestLenght) / norm(particle_old[j - nrOfParticlesHorizontally] - particle_old[j]));
			cUpp = velocity_old[j - nrOfParticlesHorizontally] - velocity_old[j];
		}
		
		// if we are on the far left column of the fabric
		if (j % nrOfParticlesHorizontally == 0){
			kVanster = glm::vec3(0.f, 0.f, 0.0f);
			cVanster = glm::vec3(0.f, 0.f, 0.0f);
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
	}
			/*cout << "kUpp.x   = " << kUpp.x << " kUpp.y = " << kUpp.x << " kUpp.z = " << kUpp.z << endl;
			cout << "cUpp.x   = " << cUpp.x << " cUpp.y = " << cUpp.x << " cUpp.z = " << cUpp.z << endl;
			cout << "kVanster.x   = " << kVanster.x << " kVanster.y = " << kVanster.x << " kVanster.z = " << kVanster.z << endl;
			cout << "cVanster.x   = " << cVanster.x << " cVanster.y = " << cVanster.x << " cVanster.z = " << cVanster.z << endl;
			cout << "kRigth.x   = " << kRigth.x << " kRigth.y = " << kRigth.x << " kRigth.z = " << kRigth.z << endl;
			cout << "cRigth.x   = " << cRigth.x << " cRigth.y = " << cRigth.x << " cRigth.z = " << cRigth.z << endl;
			cout << "kDown.x   = " << kDown.x << " kDown.y = " << kDown.x << " kDown.z = " << kDown.z << endl;
			cout << "cDown.x   = " << cDown.x << " cDown.y = " << cDown.x << " cDown.z = " << cDown.z << endl;
			cout << "normalize  = " << norm(particle_old[1 + nrOfParticlesHorizontally] - particle_old[1]) << endl << endl;*/
	//}
	return particlesNextPos;
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

	for (float i = 0; i < nrOfParticlesVertically*nrOfParticlesHorizontally; i++) {
		tempVec.x = springRestLenght * i - nrOfParticlesHorizontally*row; // X-coordinate

		//check if it is the last particle on the row
		if (fmod(i, nrOfParticlesHorizontally) == 0) {
			tempVec.y = row; // Y-coordinate
			row += springRestLenght;
		} else {
			tempVec.y = row; // Y-coordinate
		}
			
		particles.push_back(tempVec);
	}

}
        
// Takes in a vector of Coordinates for each mass 
// in the cloth and stores them in a new vector   
// in the order they should be used as triangle   
// corners when drawing the cloth.
vector<glm::vec3> MakeTriangles(vector<glm::vec3> C)
{
	vector<glm::vec3> order;

	//width-1 because we dont need to look at the 
	//last row of masses of the cloth
	//and not the last mass of the second last row either
	for (int j = 0; j < (nrOfParticlesHorizontally - 1)*nrOfParticlesVertically - 1; j++)
	{
		//if we have reached the rigth side of the cloth 
		//we skip to the next mass
		if (j%nrOfParticlesHorizontally != 0)
		{
			order.push_back(C[j]);
			order.push_back(C[nrOfParticlesHorizontally + j]);
			order.push_back(C[j + 1]);
			order.push_back(C[j + 1]);
			order.push_back(C[nrOfParticlesHorizontally + j]);
			order.push_back(C[nrOfParticlesHorizontally + j + 1]);
		}
	}
	/*
	cout << " order 5 x  = " << order[5].x << " order 6 x  = " << order[6].x << " order 7 x  = " << order[7].x << endl;
	cout << " order 5 y  = " << order[5].y << " order 6 y  = " << order[6].y << " order 7 y  = " << order[7].y << endl;
	cout << " order 5 x  = " << order[5].z << " order 6 z  = " << order[6].z << " order 7 z  = " << order[7].z << endl << endl; */

	return order;
}


// norm = sqrt(x^2+y^2+x^2) 
float norm(glm::vec3 vec){
	return  sqrt(pow(vec.x, 2.0) + pow(vec.y, 2.0) + pow(vec.x, 2.0));
}


