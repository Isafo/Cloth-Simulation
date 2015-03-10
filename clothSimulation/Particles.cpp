#include "Particles.h"

// Calculate the cloths position in the next frame using Eulermethod. Input is the cloths position in the current frame

void Euler(vector<glm::vec3> &particle, vector<glm::vec3> &particle_old, vector<glm::vec3> &velocity, vector<glm::vec3> &velocity_old, vector<int> staticParticles){
	
	vector<glm::vec3> particlesNextPos;
	particlesNextPos = placeZeros();

	std::vector<int>::iterator it;

	glm::vec3 kUp;
	glm::vec3 cUp;
	glm::vec3 kLeft;
	glm::vec3 cLeft;
	glm::vec3 kRight;
	glm::vec3 cRight;
	glm::vec3 kDown;
	glm::vec3 cDown;
	glm::vec3 k2Down;
	glm::vec3 c2Down;
	glm::vec3 kUpLeft;
	glm::vec3 cUpLeft;
	glm::vec3 kUpRight;
	glm::vec3 cUpRight;
	glm::vec3 kDownLeft;
	glm::vec3 cDownLeft;
	glm::vec3 kDownRight;
	glm::vec3 cDownRight;
	glm::vec3 k2Up;
	glm::vec3 c2Up;
	glm::vec3 k2Right;
	glm::vec3 c2Right;
	glm::vec3 k2Left;
	glm::vec3 c2Left;

	int staticPoint1 = nrOfParticlesHorizontally*nrOfParticlesVertically - 1;
	int staticPoint2 = nrOfParticlesHorizontally*(nrOfParticlesVertically - 1);

	for (int j = 0; j < nrOfParticlesHorizontally*nrOfParticlesVertically; j++){

		//if we can have a bend spring upwards
		if (j +1 > 2 * nrOfParticlesHorizontally)
		{
			k2Up = ((particle_old[j - 2 * nrOfParticlesHorizontally] - particle_old[j])*((norm(particle_old[j - 2 * nrOfParticlesHorizontally] - particle_old[j]) - oaB) / norm(particle_old[j - 2 * nrOfParticlesHorizontally] - particle_old[j])));
			c2Up = velocity_old[j - 2 * nrOfParticlesHorizontally] - velocity_old[j];

			//then we can elso have a streatch spring upwards
			kUp = ((particle_old[j - nrOfParticlesHorizontally] - particle_old[j])*((norm(particle_old[j - nrOfParticlesHorizontally] - particle_old[j]) - oaSt) / norm(particle_old[j - nrOfParticlesHorizontally] - particle_old[j])));
			cUp = velocity_old[j - nrOfParticlesHorizontally] - velocity_old[j];
		}
		else{
			//if we can have a streatch spring upwards
			if (j+1 > nrOfParticlesHorizontally)
			{
				kUp = ((particle_old[j - nrOfParticlesHorizontally] - particle_old[j])*((norm(particle_old[j - nrOfParticlesHorizontally] - particle_old[j]) - oaSt) / norm(particle_old[j - nrOfParticlesHorizontally] - particle_old[j])));
				cUp = velocity_old[j - nrOfParticlesHorizontally] - velocity_old[j];
			}
			else{
				kUp = glm::vec3(0.f, 0.f, 0.f);
				cUp = glm::vec3(0.f, 0.f, 0.f);
			}

			k2Up = glm::vec3(0.f, 0.f, 0.f);
			c2Up = glm::vec3(0.f, 0.f, 0.f);
		}

		//if we can have a bend spring to the right
		if ((j+1) % nrOfParticlesHorizontally > 0 && (j+1)% nrOfParticlesHorizontally < nrOfParticlesHorizontally - 1){
			k2Right = ((particle_old[j + 2] - particle_old[j])*((norm(particle_old[j + 2] - particle_old[j]) - oaB) / norm(particle_old[j + 2] - particle_old[j])));
			c2Right = velocity_old[j + 2] - velocity_old[j];

			//then we can elso have a streatch spring to the right
			kRight = ((particle_old[j + 1] - particle_old[j])*((norm(particle_old[j + 1] - particle_old[j]) - oaSt) / norm(particle_old[j + 1] - particle_old[j])));
			cRight = velocity_old[j + 1] - velocity_old[j];
		}
		else{
			//if we can have a streatch spring to the right
			if ((j+1) % nrOfParticlesHorizontally != 0){
				kRight = ((particle_old[j + 1] - particle_old[j])*((norm(particle_old[j + 1] - particle_old[j]) - oaSt) / norm(particle_old[j + 1] - particle_old[j])));
				cRight = velocity_old[j + 1] - velocity_old[j];
			}
			else{
				kRight = glm::vec3(0.f, 0.f, 0.f);
				cRight = glm::vec3(0.f, 0.f, 0.f);
			}
			k2Right = glm::vec3(0.f, 0.f, 0.f);
			c2Right = glm::vec3(0.f, 0.f, 0.f);
		}

		//if we can have a bend spring downwards
		if (j+1 <= nrOfParticlesHorizontally*(nrOfParticlesVertically - 2)){
			k2Down = ((particle_old[j + 2 * nrOfParticlesHorizontally] - particle_old[j])*((norm(particle_old[j + 2 * nrOfParticlesHorizontally] - particle_old[j]) - oaB) / norm(particle_old[ j + 2 * nrOfParticlesHorizontally] - particle_old[j])));
			c2Down = velocity_old[j + 2 * nrOfParticlesHorizontally] - velocity_old[j];

			//then we can elso have a streatch spring downwards
			kDown = ((particle_old[ j + nrOfParticlesHorizontally] - particle_old[j])*((norm(particle_old[j + nrOfParticlesHorizontally] - particle_old[j]) - oaSt) / norm(particle_old[j + nrOfParticlesHorizontally] - particle_old[j])));
			cDown = velocity_old[j + nrOfParticlesHorizontally] - velocity_old[j];
		}
		else{
			//if we can have a streatch spring downwards
			if (j+1 <= nrOfParticlesHorizontally*(nrOfParticlesVertically - 1)){
				kDown = ((particle_old[j + nrOfParticlesHorizontally] - particle_old[j])*((norm(particle_old[j + nrOfParticlesHorizontally] - particle_old[j]) - oaSt) / norm(particle_old[j + nrOfParticlesHorizontally] - particle_old[j])));
				cDown = velocity_old[j + nrOfParticlesHorizontally] - velocity_old[j];
			}
			else{
				kDown = glm::vec3(0.f, 0.f, 0.f);
				cDown = glm::vec3(0.f, 0.f, 0.f);
			}
			k2Down = glm::vec3(0.f, 0.f, 0.f);
			c2Down = glm::vec3(0.f, 0.f, 0.f);
		}

		//if we can have a bend spring to the left
		if ((j+1) % nrOfParticlesHorizontally > 2){
			k2Left = ((particle_old[j - 2] - particle_old[j])*((norm(particle_old[j - 2] - particle_old[j]) - oaB) / norm(particle_old[j - 2] - particle_old[j])));
			c2Left = velocity_old[j - 2] - velocity_old[j];

			//then we can elso have a streatch spring to the left
			kLeft = ((particle_old[j - 1] - particle_old[j])*((norm(particle_old[j - 1] - particle_old[j]) - oaSt) / norm(particle_old[j - 1] - particle_old[j])));
			cLeft = velocity_old[j - 1] - velocity_old[j];
		}
		else{
			//if we can have a streatch spring to the left
			if ((j+1) % nrOfParticlesHorizontally != 1){
				kLeft = ((particle_old[j - 1] - particle_old[j])*((norm(particle_old[j - 1] - particle_old[j]) - oaSt) / norm(particle_old[j - 1] - particle_old[j])));
				cLeft = velocity_old[j - 1] - velocity_old[j];
			}
			else{
				kLeft = glm::vec3(0.f, 0.f, 0.f);
				cLeft = glm::vec3(0.f, 0.f, 0.f);
			}
			k2Left = glm::vec3(0.f, 0.f, 0.f);
			c2Left = glm::vec3(0.f, 0.f, 0.f);
		}
		
		//if we can have shear spring to the right and Upwards
		if ((j+1) % nrOfParticlesHorizontally > 0 && j+1 > nrOfParticlesHorizontally){
			kUpRight = ((particle_old[j - nrOfParticlesHorizontally + 1] - particle_old[j])*((norm(particle_old[j - nrOfParticlesHorizontally + 1] - particle_old[j]) - oaSh) / norm(particle_old[j - nrOfParticlesHorizontally + 1] - particle_old[j])));
			cUpRight = velocity_old[ j - nrOfParticlesHorizontally + 1] - velocity_old[j];
		}
		else{
			kUpRight = glm::vec3(0.f, 0.f, 0.f);
			cUpRight = glm::vec3(0.f, 0.f, 0.f);
		}

		//if we can have shear spring to the right and downwards
		if ((j+1) % nrOfParticlesHorizontally > 0 && j+1 < nrOfParticlesHorizontally*(nrOfParticlesVertically - 1)){
			kDownRight = ((particle_old[j + nrOfParticlesHorizontally + 1] - particle_old[j])*((norm(particle_old[j + nrOfParticlesHorizontally + 1] - particle_old[j]) - oaSh) / norm(particle_old[j + nrOfParticlesHorizontally + 1] - particle_old[j])));
			cDownRight = velocity_old[j + nrOfParticlesHorizontally + 1] - velocity_old[j];
		}
		else{
			kDownRight = glm::vec3(0.f, 0.f, 0.f);
			cDownRight = glm::vec3(0.f, 0.f, 0.f);
		}

		//if we can have shear spring to the left and downwards
		if ((j+1) % nrOfParticlesHorizontally != 1 && j+1 < nrOfParticlesHorizontally*(nrOfParticlesVertically - 1)){
			kDownLeft = ((particle_old[j + nrOfParticlesHorizontally - 1] - particle_old[j])*((norm(particle_old[j + nrOfParticlesHorizontally - 1] - particle_old[j]) - oaSh) / norm(particle_old[j + nrOfParticlesHorizontally - 1] - particle_old[j])));
			cDownLeft = velocity_old[j + nrOfParticlesHorizontally - 1] - velocity_old[j];
		}
		else{
			kDownLeft = glm::vec3(0.f, 0.f, 0.f);
			cDownLeft = glm::vec3(0.f, 0.f, 0.f);
		}

		//if we can have shear spring to the left and Upwards
		if ((j+1) % nrOfParticlesHorizontally != 1 && j+1 > nrOfParticlesHorizontally){
			kUpLeft = ((particle_old[j - nrOfParticlesHorizontally - 1] - particle_old[j])*((norm(particle_old[j - nrOfParticlesHorizontally - 1] - particle_old[j]) - oaSh) / norm(particle_old[j - nrOfParticlesHorizontally - 1] - particle_old[j])));
			cUpLeft = velocity_old[j - nrOfParticlesHorizontally - 1] - velocity_old[j];
		}
		else{
			kUpLeft = glm::vec3(0.f, 0.f, 0.f);
			cUpLeft = glm::vec3(0.f, 0.f, 0.f);
		}
		

		it = find(staticParticles.begin(), staticParticles.end(), j);
		if (it !=staticParticles.end()){// kollar ifall pukten är statisk
			particlesNextPos[j] = particle[j];
			velocity[j] = velocity_old[j];
		} else{
			
			//calculate the new velosity
			velocity[j] = velocity_old[j] + (timestep / particleMass)*(particleMass*g + kSt*(kUp +kLeft + kRight + kDown) + kSh*(kUpLeft + kUpRight + kDownLeft + kDownRight) + kB*(k2Up + k2Right + k2Down + k2Left) + cSt*(cUp + cLeft + cRight + cDown) + cSh*(cUpLeft + cUpRight + cDownLeft + cDownRight) + cB*(c2Up + c2Right + c2Down + c2Left));

			//calculate the new possition
			particlesNextPos[j] = particle[j] + timestep*velocity[j];
		}
	}
	
	//update to new values
	velocity_old = velocity;
	particle_old = particle;
	particle = particlesNextPos;
}

void initializeStaticParticles(vector<int> &staticParticles){
	int staticPoint1 = nrOfParticlesHorizontally*nrOfParticlesVertically - 1;
	int staticPoint2 = nrOfParticlesHorizontally*(nrOfParticlesVertically - 1);
	staticParticles.push_back(staticPoint1);
	staticParticles.push_back(staticPoint2);
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
		tempVec.y = 0; // y-coordinate
		tempVec.z =  springRestLenght * row; // z-coordinate

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

	//heigth-1 because we dont need to look at the 
	//last row of masses of the cloth
	//and not the last mass of the second last row either
	for (int j = 0; j < (nrOfParticlesHorizontally)*(nrOfParticlesVertically-1)-1; j++)
	{
		//if we have reached the right side of the cloth 
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