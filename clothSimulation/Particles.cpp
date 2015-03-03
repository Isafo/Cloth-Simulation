#include "Particles.h"

// Calculate the cloths position in the next frame using Eulermethod. Input is the cloths position in the current frame

void Euler(vector<glm::vec3> &particle, vector<glm::vec3> &particle_old, vector<glm::vec3> &velocity, vector<glm::vec3> &velocity_old, vector<int> staticParticles){
	
	vector<glm::vec3> particlesNextPos;
	particlesNextPos = placeZeros();

	std::vector<int>::iterator it;

	glm::vec3 kUpp;
	glm::vec3 cUpp;
	glm::vec3 kVanster;
	glm::vec3 cVanster;
	/*glm::vec3 kRigth;
	glm::vec3 cRigth;
	glm::vec3 kDown;
	glm::vec3 cDown;*/

	glm::vec3 kHoger;
	glm::vec3 cHoger;
	glm::vec3 kNed;
	glm::vec3 cNed;
	glm::vec3 k2Ned;
	glm::vec3 c2Ned;
	glm::vec3 kUppVanster;
	glm::vec3 cUppVanster;
	glm::vec3 kUppHoger;
	glm::vec3 cUppHoger;
	glm::vec3 kNedVanster;
	glm::vec3 cNedVanster;
	glm::vec3 kNedHoger;
	glm::vec3 cNedHoger;
	glm::vec3 k2Upp;
	glm::vec3 c2Upp;
	glm::vec3 k2Hoger;
	glm::vec3 c2Hoger;
	glm::vec3 k2Vanster;
	glm::vec3 c2Vanster;

	int staticPoint1 = nrOfParticlesHorizontally*nrOfParticlesVertically - 1;
	int staticPoint2 = nrOfParticlesHorizontally*(nrOfParticlesVertically - 1);

	for (int j = 0; j < nrOfParticlesHorizontally*nrOfParticlesVertically; j++){

		//if we can have a bend spring upwards
		if (j +1 > 2 * nrOfParticlesHorizontally)
		{
			k2Upp = ((particle_old[j - 2 * nrOfParticlesHorizontally] - particle_old[j])*((norm(particle_old[j - 2 * nrOfParticlesHorizontally] - particle_old[j]) - oaB) / norm(particle_old[j - 2 * nrOfParticlesHorizontally] - particle_old[j])));
			c2Upp = velocity_old[j - 2 * nrOfParticlesHorizontally] - velocity_old[j];

			//then we can elso have a streatch spring upwards
			kUpp = ((particle_old[j - nrOfParticlesHorizontally] - particle_old[j])*((norm(particle_old[j - nrOfParticlesHorizontally] - particle_old[j]) - oaSt) / norm(particle_old[j - nrOfParticlesHorizontally] - particle_old[j])));
			cUpp = velocity_old[j - nrOfParticlesHorizontally] - velocity_old[j];
		}
		else{
			//if we can have a streatch spring upwards
			if (j+1 > nrOfParticlesHorizontally)
			{
				kUpp = ((particle_old[j - nrOfParticlesHorizontally] - particle_old[j])*((norm(particle_old[j - nrOfParticlesHorizontally] - particle_old[j]) - oaSt) / norm(particle_old[j - nrOfParticlesHorizontally] - particle_old[j])));
				cUpp = velocity_old[j - nrOfParticlesHorizontally] - velocity_old[j];
			}
			else{
				kUpp = glm::vec3(0.f, 0.f, 0.f);
				cUpp = glm::vec3(0.f, 0.f, 0.f);
			}

			k2Upp = glm::vec3(0.f, 0.f, 0.f);
			c2Upp = glm::vec3(0.f, 0.f, 0.f);
		}

		//if we can have a bend spring to the right
		if ((j+1) % nrOfParticlesHorizontally > 0 && (j+1)% nrOfParticlesHorizontally < nrOfParticlesHorizontally - 1){
			k2Hoger = ((particle_old[j + 2] - particle_old[j])*((norm(particle_old[j + 2] - particle_old[j]) - oaB) / norm(particle_old[j + 2] - particle_old[j])));
			c2Hoger = velocity_old[j + 2] - velocity_old[j];

			//then we can elso have a streatch spring to the right
			kHoger = ((particle_old[j + 1] - particle_old[j])*((norm(particle_old[j + 1] - particle_old[j]) - oaSt) / norm(particle_old[j + 1] - particle_old[j])));
			cHoger = velocity_old[j + 1] - velocity_old[j];
		}
		else{
			//if we can have a streatch spring to the right
			if ((j+1) % nrOfParticlesHorizontally != 0){
				kHoger = ((particle_old[j + 1] - particle_old[j])*((norm(particle_old[j + 1] - particle_old[j]) - oaSt) / norm(particle_old[j + 1] - particle_old[j])));
				cHoger = velocity_old[j + 1] - velocity_old[j];
			}
			else{
				kHoger = glm::vec3(0.f, 0.f, 0.f);
				cHoger = glm::vec3(0.f, 0.f, 0.f);
			}
			k2Hoger = glm::vec3(0.f, 0.f, 0.f);
			c2Hoger = glm::vec3(0.f, 0.f, 0.f);
		}

		//if we can have a bend spring downwards
		if (j+1 <= nrOfParticlesHorizontally*(nrOfParticlesVertically - 2)){
			k2Ned = ((particle_old[j + 2 * nrOfParticlesHorizontally] - particle_old[j])*((norm(particle_old[j + 2 * nrOfParticlesHorizontally] - particle_old[j]) - oaB) / norm(particle_old[ j + 2 * nrOfParticlesHorizontally] - particle_old[j])));
			c2Ned = velocity_old[j + 2 * nrOfParticlesHorizontally] - velocity_old[j];

			//then we can elso have a streatch spring downwards
			kNed = ((particle_old[ j + nrOfParticlesHorizontally] - particle_old[j])*((norm(particle_old[j + nrOfParticlesHorizontally] - particle_old[j]) - oaSt) / norm(particle_old[j + nrOfParticlesHorizontally] - particle_old[j])));
			cNed = velocity_old[j + nrOfParticlesHorizontally] - velocity_old[j];
		}
		else{
			//if we can have a streatch spring downwards
			if (j+1 <= nrOfParticlesHorizontally*(nrOfParticlesVertically - 1)){
				kNed = ((particle_old[j + nrOfParticlesHorizontally] - particle_old[j])*((norm(particle_old[j + nrOfParticlesHorizontally] - particle_old[j]) - oaSt) / norm(particle_old[j + nrOfParticlesHorizontally] - particle_old[j])));
				cNed = velocity_old[j + nrOfParticlesHorizontally] - velocity_old[j];
			}
			else{
				kNed = glm::vec3(0.f, 0.f, 0.f);
				cNed = glm::vec3(0.f, 0.f, 0.f);
			}
			k2Ned = glm::vec3(0.f, 0.f, 0.f);
			c2Ned = glm::vec3(0.f, 0.f, 0.f);
		}

		//if we can have a bend spring to the left
		if ((j+1) % nrOfParticlesHorizontally > 2){
			k2Vanster = ((particle_old[j - 2] - particle_old[j])*((norm(particle_old[j - 2] - particle_old[j]) - oaB) / norm(particle_old[j - 2] - particle_old[j])));
			c2Vanster = velocity_old[j - 2] - velocity_old[j];

			//then we can elso have a streatch spring to the left
			kVanster = ((particle_old[j - 1] - particle_old[j])*((norm(particle_old[j - 1] - particle_old[j]) - oaSt) / norm(particle_old[j - 1] - particle_old[j])));
			cVanster = velocity_old[j - 1] - velocity_old[j];
		}
		else{
			//if we can have a streatch spring to the left
			if ((j+1) % nrOfParticlesHorizontally != 1){
				kVanster = ((particle_old[j - 1] - particle_old[j])*((norm(particle_old[j - 1] - particle_old[j]) - oaSt) / norm(particle_old[j - 1] - particle_old[j])));
				cVanster = velocity_old[j - 1] - velocity_old[j];
			}
			else{
				kVanster = glm::vec3(0.f, 0.f, 0.f);
				cVanster = glm::vec3(0.f, 0.f, 0.f);
			}
			k2Vanster = glm::vec3(0.f, 0.f, 0.f);
			c2Vanster = glm::vec3(0.f, 0.f, 0.f);
		}
		
		//if we can have shear spring to the right and uppwards
		if ((j+1) % nrOfParticlesHorizontally > 0 && j+1 > nrOfParticlesHorizontally){
			kUppHoger = ((particle_old[j - nrOfParticlesHorizontally + 1] - particle_old[j])*((norm(particle_old[j - nrOfParticlesHorizontally + 1] - particle_old[j]) - oaSh) / norm(particle_old[j - nrOfParticlesHorizontally + 1] - particle_old[j])));
			cUppHoger = velocity_old[ j - nrOfParticlesHorizontally + 1] - velocity_old[j];
		}
		else{
			kUppHoger = glm::vec3(0.f, 0.f, 0.f);
			cUppHoger = glm::vec3(0.f, 0.f, 0.f);
		}

		//if we can have shear spring to the right and downwards
		if ((j+1) % nrOfParticlesHorizontally > 0 && j+1 < nrOfParticlesHorizontally*(nrOfParticlesVertically - 1)){
			kNedHoger = ((particle_old[j + nrOfParticlesHorizontally + 1] - particle_old[j])*((norm(particle_old[j + nrOfParticlesHorizontally + 1] - particle_old[j]) - oaSh) / norm(particle_old[j + nrOfParticlesHorizontally + 1] - particle_old[j])));
			cNedHoger = velocity_old[j + nrOfParticlesHorizontally + 1] - velocity_old[j];
		}
		else{
			kNedHoger = glm::vec3(0.f, 0.f, 0.f);
			cNedHoger = glm::vec3(0.f, 0.f, 0.f);
		}

		//if we can have shear spring to the left and downwards
		if ((j+1) % nrOfParticlesHorizontally != 1 && j+1 < nrOfParticlesHorizontally*(nrOfParticlesVertically - 1)){
			kNedVanster = ((particle_old[j + nrOfParticlesHorizontally - 1] - particle_old[j])*((norm(particle_old[j + nrOfParticlesHorizontally - 1] - particle_old[j]) - oaSh) / norm(particle_old[j + nrOfParticlesHorizontally - 1] - particle_old[j])));
			cNedVanster = velocity_old[j + nrOfParticlesHorizontally - 1] - velocity_old[j];
		}
		else{
			kNedVanster = glm::vec3(0.f, 0.f, 0.f);
			cNedVanster = glm::vec3(0.f, 0.f, 0.f);
		}

		//if we can have shear spring to the left and uppwards
		if ((j+1) % nrOfParticlesHorizontally != 1 && j+1 > nrOfParticlesHorizontally){
			kUppVanster = ((particle_old[j - nrOfParticlesHorizontally - 1] - particle_old[j])*((norm(particle_old[j - nrOfParticlesHorizontally - 1] - particle_old[j]) - oaSh) / norm(particle_old[j - nrOfParticlesHorizontally - 1] - particle_old[j])));
			cUppVanster = velocity_old[j - nrOfParticlesHorizontally - 1] - velocity_old[j];
		}
		else{
			kUppVanster = glm::vec3(0.f, 0.f, 0.f);
			cUppVanster = glm::vec3(0.f, 0.f, 0.f);
		}
		

		it = find(staticParticles.begin(), staticParticles.end(), j);
		if (it !=staticParticles.end()){// kollar ifall pukten är statisk
			particlesNextPos[j] = particle[j];
			velocity[j] = velocity_old[j];
		} else{
			
			//calculate the new velosity
			velocity[j] = velocity_old[j] + (timestep / particleMass)*(particleMass*g + kSt*(kUpp + kVanster + kHoger + kNed) + kSh*(kUppVanster + kUppHoger + kNedVanster + kNedHoger) + kB*(k2Upp + k2Hoger + k2Ned + k2Vanster) + cSt*(cUpp + cVanster + cHoger + cNed) + cSh*(cUppVanster + cUppHoger + cNedVanster + cNedHoger) + cB*(c2Upp + c2Hoger + c2Ned + c2Vanster));

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