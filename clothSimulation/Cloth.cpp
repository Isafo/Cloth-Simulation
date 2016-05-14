#include "Cloth.h"
#include "linAlg.h"

#include <algorithm>

Cloth::Cloth(float _x, float _y, float _z, int nvertsH, int nvertsV)
{
	transform = glm::mat4(1.0f);
	transform[3][0] = _x;
	transform[3][1] = _y;
	transform[3][2] = _z;

	nparticlesH = nvertsH;
	nparticlesV = nvertsV;

	nverts = nvertsH * nvertsV;
	ntris = ((nvertsH - 1) * (nvertsV - 1)) * 2;

	createCloth();
}


Cloth::~Cloth(void)
{
	unmapBuffers();
	//delete vertexBufferPointer;

}

void Cloth::createCloth()
{
	Particle tempP;
	
	int row = 0;
	
	for (int i = 0; i < nparticlesH*nparticlesV; i++) 
	{
		//check if it is the first particle on a new row
		if (i != 0 && (i % nparticlesH) == 0) 
		{
			row += 1;
		}

		if (i == nparticlesH * (nparticlesV - 1) -1 || i == nparticlesH * (nparticlesV) - 1)
			tempP.t_static = true;
		else
			tempP.t_static = false;

		tempP.pos[0] = springRestLenght * (i - nparticlesH*row); // X-coordinate
		tempP.pos[1] = 0;										 // y-coordinate
		tempP.pos[2] = springRestLenght * row;					 // z-coordinate

		tempP.norm[0] = 0.0f;
		tempP.norm[1] = 0.0f;
		tempP.norm[2] = 0.0f;

		tempP.vel[0] = 0.0f;
		tempP.vel[1] = 0.0f;
		tempP.vel[2] = 0.0f;

		tempP.tex[0] = tempP.pos[0] / (springRestLenght * nparticlesH);
		tempP.tex[1] = tempP.pos[2] / (springRestLenght * nparticlesV);
		vA[currentArray].push_back(tempP);
		vA[currentArray + 1].push_back(tempP);
	}
	
	triangle tempT;

	for (int column = 0; column < nparticlesV - 1; column++)
	{
		for (int row = 0; row < nparticlesH - 1; row++)
		{
			int index = row + column * nparticlesH;
			tempT.index[0] = index;
			tempT.index[1] = index + nparticlesH;
			tempT.index[2] = index + 1;

			iA.push_back(tempT);

			tempT.index[0] = index + 1;
			tempT.index[1] = index + nparticlesH;
			tempT.index[2] = index + nparticlesH + 1;

			iA.push_back(tempT);
		}
	}

	// Generate one vertex array object (VAO) and bind it
	glGenVertexArrays(1, &(vao));
	glBindVertexArray(vao);

	// Generate two buffer IDs
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &indexbuffer);

	// Activate the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Present our vertex coordinates to OpenGL
	// Present our vertex coordinates to OpenGL
	glBufferStorage(GL_ARRAY_BUFFER, nverts * sizeof(Particle),
					&vA[0][0], GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
	// Specify how many attribute arrays we have in our VAO
	glEnableVertexAttribArray(0); // Vertex coordinates
	glEnableVertexAttribArray(1); // Normals
	glEnableVertexAttribArray(2); // Texture coords

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		sizeof(Particle), (void*)0);						// xyz coordinates

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		sizeof(Particle), (void*)(3 * sizeof(GLfloat)));	// normals

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
		sizeof(Particle), (void*)(6 * sizeof(GLfloat)));	// texture coords

	// Activate the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	// Present our vertex indices to OpenGL
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		3 * ntris * sizeof(GLuint), &iA[0], GL_DYNAMIC_DRAW);

	// Deactivate (unbind) the VAO and the buffers again.
	// Do NOT unbind the index buffer while the VAO is still bound.
	// The index buffer is an essential part of the VAO state.
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	mapBuffers();

}

void Cloth::mapBuffers() {
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	vertexBufferPointer = (Particle*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(Particle) * nverts,
		GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
}

void Cloth::unmapBuffers() {
	glUnmapBuffer(GL_ARRAY_BUFFER);

	// Specify how many attribute arrays we have in our VAO
	glEnableVertexAttribArray(0); // Vertex coordinates
	glEnableVertexAttribArray(1); // Normals
	glEnableVertexAttribArray(2); // Tex

	// Specify how OpenGL should interpret the vertex buffer data:
	// Attributes 0, 1, 2 (must match the lines above and the layout in the shader)
	// Number of dimensions (3 means vec3 in the shader, 2 means vec2)
	// Type GL_FLOAT
	// Not normalized (GL_FALSE)
	// Stride 8 (interleaved array with 8 floats per vertex)
	// Array buffer offset 0, 3, 6 (offset into first vertex)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		sizeof(Particle), (void*)0); // xyz coordinates
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		sizeof(Particle), (void*)(3 * sizeof(GLfloat))); // normals
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
		sizeof(Particle), (void*)(6 * sizeof(GLfloat))); // tex

	// Deactivate (unbind) the VAO and the buffers again.
	// Do NOT unbind the buffers while the VAO is still bound.
	// The index buffer is an essential part of the VAO state.
	//glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

// norm = sqrt(x^2+y^2+x^2) 
float norm(glm::vec3 vec) {
	return  sqrt(pow(vec.x, 2.0) + pow(vec.y, 2.0) + pow(vec.z, 2.0));
}

// TODO: rewrite this to not use glm and remove unnesseccary copies /  check what is wrong with the rewritten commented kode in update
void Euler(std::vector<glm::vec3> &particle, std::vector<glm::vec3> &particle_old, std::vector<glm::vec3> &velocity, std::vector<glm::vec3> &velocity_old) {

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

	static const float kSt = 120;
	static const float kSh = 90;
	static const float kB = 80;
	static const float oaSt = 0.1;
	static const float oaSh = sqrt(2 * pow(oaSt, 2.0));
	static const float oaB = 2 * oaSt;
	static const float cSt = 0.2;
	static const float cSh = 0.9;
	static const float cB = 0.6;

	// spring constants
	const float springRestLenght = 0.1;
	const float timestep = 0.003f;
	const float particleMass = 0.018f;
	const float k = 10000.0f;						  // spring konstant
	const float c = 5.0f;							  // damper constant
	const glm::vec3 g = glm::vec3(0.f, -18.82f, 0.f); // gravity

	std::vector<glm::vec3> particlesNextPos;

	int nrOfParticlesHorizontally = 16;
	int nrOfParticlesVertically = 16;

	int staticPoint1 = 0;
	int staticPoint2 = nrOfParticlesHorizontally-1;

	for (int j = 0; j < nrOfParticlesHorizontally*nrOfParticlesVertically; j++) {

		//if we can have a bend spring upwards
		if (j + 1 > 2 * nrOfParticlesHorizontally)
		{
			k2Up = ((particle_old[j - 2 * nrOfParticlesHorizontally] - particle_old[j])*((norm(particle_old[j - 2 * nrOfParticlesHorizontally] - particle_old[j]) - oaB) / norm(particle_old[j - 2 * nrOfParticlesHorizontally] - particle_old[j])));
			c2Up = velocity_old[j - 2 * nrOfParticlesHorizontally] - velocity_old[j];

			//then we can elso have a streatch spring upwards
			kUp = ((particle_old[j - nrOfParticlesHorizontally] - particle_old[j])*((norm(particle_old[j - nrOfParticlesHorizontally] - particle_old[j]) - oaSt) / norm(particle_old[j - nrOfParticlesHorizontally] - particle_old[j])));
			cUp = velocity_old[j - nrOfParticlesHorizontally] - velocity_old[j];
		}
		else {
			//if we can have a streatch spring upwards
			if (j + 1 > nrOfParticlesHorizontally)
			{
				kUp = ((particle_old[j - nrOfParticlesHorizontally] - particle_old[j])*((norm(particle_old[j - nrOfParticlesHorizontally] - particle_old[j]) - oaSt) / norm(particle_old[j - nrOfParticlesHorizontally] - particle_old[j])));
				cUp = velocity_old[j - nrOfParticlesHorizontally] - velocity_old[j];
			}
			else {
				kUp = glm::vec3(0.f, 0.f, 0.f);
				cUp = glm::vec3(0.f, 0.f, 0.f);
			}

			k2Up = glm::vec3(0.f, 0.f, 0.f);
			c2Up = glm::vec3(0.f, 0.f, 0.f);
		}

		//if we can have a bend spring to the right
		if ((j + 1) % nrOfParticlesHorizontally > 0 && (j + 1) % nrOfParticlesHorizontally < nrOfParticlesHorizontally - 1) {
			k2Right = ((particle_old[j + 2] - particle_old[j])*((norm(particle_old[j + 2] - particle_old[j]) - oaB) / norm(particle_old[j + 2] - particle_old[j])));
			c2Right = velocity_old[j + 2] - velocity_old[j];

			//then we can elso have a streatch spring to the right
			kRight = ((particle_old[j + 1] - particle_old[j])*((norm(particle_old[j + 1] - particle_old[j]) - oaSt) / norm(particle_old[j + 1] - particle_old[j])));
			cRight = velocity_old[j + 1] - velocity_old[j];
		}
		else {
			//if we can have a streatch spring to the right
			if ((j + 1) % nrOfParticlesHorizontally != 0) {
				kRight = ((particle_old[j + 1] - particle_old[j])*((norm(particle_old[j + 1] - particle_old[j]) - oaSt) / norm(particle_old[j + 1] - particle_old[j])));
				cRight = velocity_old[j + 1] - velocity_old[j];
			}
			else {
				kRight = glm::vec3(0.f, 0.f, 0.f);
				cRight = glm::vec3(0.f, 0.f, 0.f);
			}
			k2Right = glm::vec3(0.f, 0.f, 0.f);
			c2Right = glm::vec3(0.f, 0.f, 0.f);
		}

		//if we can have a bend spring downwards
		if (j + 1 <= nrOfParticlesHorizontally*(nrOfParticlesVertically - 2)) {
			k2Down = ((particle_old[j + 2 * nrOfParticlesHorizontally] - particle_old[j])*((norm(particle_old[j + 2 * nrOfParticlesHorizontally] - particle_old[j]) - oaB) / norm(particle_old[j + 2 * nrOfParticlesHorizontally] - particle_old[j])));
			c2Down = velocity_old[j + 2 * nrOfParticlesHorizontally] - velocity_old[j];

			//then we can elso have a streatch spring downwards
			kDown = ((particle_old[j + nrOfParticlesHorizontally] - particle_old[j])*((norm(particle_old[j + nrOfParticlesHorizontally] - particle_old[j]) - oaSt) / norm(particle_old[j + nrOfParticlesHorizontally] - particle_old[j])));
			cDown = velocity_old[j + nrOfParticlesHorizontally] - velocity_old[j];
		}
		else {
			//if we can have a streatch spring downwards
			if (j + 1 <= nrOfParticlesHorizontally*(nrOfParticlesVertically - 1)) {
				kDown = ((particle_old[j + nrOfParticlesHorizontally] - particle_old[j])*((norm(particle_old[j + nrOfParticlesHorizontally] - particle_old[j]) - oaSt) / norm(particle_old[j + nrOfParticlesHorizontally] - particle_old[j])));
				cDown = velocity_old[j + nrOfParticlesHorizontally] - velocity_old[j];
			}
			else {
				kDown = glm::vec3(0.f, 0.f, 0.f);
				cDown = glm::vec3(0.f, 0.f, 0.f);
			}
			k2Down = glm::vec3(0.f, 0.f, 0.f);
			c2Down = glm::vec3(0.f, 0.f, 0.f);
		}

		//if we can have a bend spring to the left
		if ((j + 1) % nrOfParticlesHorizontally > 2) {
			k2Left = ((particle_old[j - 2] - particle_old[j])*((norm(particle_old[j - 2] - particle_old[j]) - oaB) / norm(particle_old[j - 2] - particle_old[j])));
			c2Left = velocity_old[j - 2] - velocity_old[j];

			//then we can elso have a streatch spring to the left
			kLeft = ((particle_old[j - 1] - particle_old[j])*((norm(particle_old[j - 1] - particle_old[j]) - oaSt) / norm(particle_old[j - 1] - particle_old[j])));
			cLeft = velocity_old[j - 1] - velocity_old[j];
		}
		else {
			//if we can have a streatch spring to the left
			if ((j + 1) % nrOfParticlesHorizontally != 1) {
				kLeft = ((particle_old[j - 1] - particle_old[j])*((norm(particle_old[j - 1] - particle_old[j]) - oaSt) / norm(particle_old[j - 1] - particle_old[j])));
				cLeft = velocity_old[j - 1] - velocity_old[j];
			}
			else {
				kLeft = glm::vec3(0.f, 0.f, 0.f);
				cLeft = glm::vec3(0.f, 0.f, 0.f);
			}
			k2Left = glm::vec3(0.f, 0.f, 0.f);
			c2Left = glm::vec3(0.f, 0.f, 0.f);
		}

		//if we can have shear spring to the right and Upwards
		if ((j + 1) % nrOfParticlesHorizontally > 0 && j + 1 > nrOfParticlesHorizontally) {
			kUpRight = ((particle_old[j - nrOfParticlesHorizontally + 1] - particle_old[j])*((norm(particle_old[j - nrOfParticlesHorizontally + 1] - particle_old[j]) - oaSh) / norm(particle_old[j - nrOfParticlesHorizontally + 1] - particle_old[j])));
			cUpRight = velocity_old[j - nrOfParticlesHorizontally + 1] - velocity_old[j];
		}
		else {
			kUpRight = glm::vec3(0.f, 0.f, 0.f);
			cUpRight = glm::vec3(0.f, 0.f, 0.f);
		}

		//if we can have shear spring to the right and downwards
		if ((j + 1) % nrOfParticlesHorizontally > 0 && j + 1 < nrOfParticlesHorizontally*(nrOfParticlesVertically - 1)) {
			kDownRight = ((particle_old[j + nrOfParticlesHorizontally + 1] - particle_old[j])*((norm(particle_old[j + nrOfParticlesHorizontally + 1] - particle_old[j]) - oaSh) / norm(particle_old[j + nrOfParticlesHorizontally + 1] - particle_old[j])));
			cDownRight = velocity_old[j + nrOfParticlesHorizontally + 1] - velocity_old[j];
		}
		else {
			kDownRight = glm::vec3(0.f, 0.f, 0.f);
			cDownRight = glm::vec3(0.f, 0.f, 0.f);
		}

		//if we can have shear spring to the left and downwards
		if ((j + 1) % nrOfParticlesHorizontally != 1 && j + 1 < nrOfParticlesHorizontally*(nrOfParticlesVertically - 1)) {
			kDownLeft = ((particle_old[j + nrOfParticlesHorizontally - 1] - particle_old[j])*((norm(particle_old[j + nrOfParticlesHorizontally - 1] - particle_old[j]) - oaSh) / norm(particle_old[j + nrOfParticlesHorizontally - 1] - particle_old[j])));
			cDownLeft = velocity_old[j + nrOfParticlesHorizontally - 1] - velocity_old[j];
		}
		else {
			kDownLeft = glm::vec3(0.f, 0.f, 0.f);
			cDownLeft = glm::vec3(0.f, 0.f, 0.f);
		}

		//if we can have shear spring to the left and Upwards
		if ((j + 1) % nrOfParticlesHorizontally != 1 && j + 1 > nrOfParticlesHorizontally) {
			kUpLeft = ((particle_old[j - nrOfParticlesHorizontally - 1] - particle_old[j])*((norm(particle_old[j - nrOfParticlesHorizontally - 1] - particle_old[j]) - oaSh) / norm(particle_old[j - nrOfParticlesHorizontally - 1] - particle_old[j])));
			cUpLeft = velocity_old[j - nrOfParticlesHorizontally - 1] - velocity_old[j];
		}
		else {
			kUpLeft = glm::vec3(0.f, 0.f, 0.f);
			cUpLeft = glm::vec3(0.f, 0.f, 0.f);
		}

		if (j == staticPoint1 || j == staticPoint2) {// kollar ifall pukten är statisk
			particlesNextPos.push_back(particle[j]);
			velocity[j] = velocity_old[j];
		}
		else {

			//calculate the new velosity
			velocity[j] = velocity_old[j] + (timestep / particleMass)*(particleMass*g + kSt*(kUp + kLeft + kRight + kDown) + kSh*(kUpLeft + kUpRight + kDownLeft + kDownRight) + kB*(k2Up + k2Right + k2Down + k2Left) + cSt*(cUp + cLeft + cRight + cDown) + cSh*(cUpLeft + cUpRight + cDownLeft + cDownRight) + cB*(c2Up + c2Right + c2Down + c2Left));

			//calculate the new possition
			particlesNextPos.push_back(particle[j] + timestep*velocity[j]);
		}
	}

	//update to new values
	velocity_old = velocity;
	particle_old = particle;
	particle = particlesNextPos;
}

void Cloth::update() {
	
	//glm::vec3 kUp;
	//glm::vec3 cUp;
	//glm::vec3 kLeft;
	//glm::vec3 cLeft;
	//glm::vec3 kRight;
	//glm::vec3 cRight;
	//glm::vec3 kDown;
	//glm::vec3 cDown;
	//glm::vec3 k2Down;
	//glm::vec3 c2Down;
	//glm::vec3 kUpLeft;
	//glm::vec3 cUpLeft;
	//glm::vec3 kUpRight;
	//glm::vec3 cUpRight;
	//glm::vec3 kDownLeft;
	//glm::vec3 cDownLeft;
	//glm::vec3 kDownRight;
	//glm::vec3 cDownRight;
	//glm::vec3 k2Up;
	//glm::vec3 c2Up;
	//glm::vec3 k2Right;
	//glm::vec3 c2Right;
	//glm::vec3 k2Left;
	//glm::vec3 c2Left;


	//static const float kSt = 80;
	//static const float kSh = 60;
	//static const float kB = 20;
	//static const float oaSt = springRestLenght;
	//static const float oaSh = sqrt(2 * pow(oaSt, 2.0));
	//static const float oaB = 2 * oaSt;
	//static const float cSt = 1.2;
	//static const float cSh = 0.9;
	//static const float cB = 0.6;

	//int staticPoint1 = nparticlesH*nparticlesV - 1;
	//int staticPoint2 = nparticlesH*(nparticlesV - 1);

	//int oldIndex = (currentArray + 1) % 2;

	//float vec[3];

	//for (int j = 0; j < nparticlesH*nparticlesV; j++) {

	//	// dont make any calculations for static particles
	//	if (vA[currentArray][j].t_static)
	//		continue;

	//	//if we can have a bend spring upwards
	//	if (j + 1 > 2 * nparticlesH)
	//	{

	//		calcSpringForce(j, j - 2 * nparticlesH, k2Up, c2Up, oaB, oldIndex);

	//		//k2Up = ((particle_old[j - 2 * nparticlesH] - particle_old[j])*((norm(particle_old[j - 2 * nparticlesH] - particle_old[j]) - oaB) / norm(particle_old[j - 2 * nparticlesH] - particle_old[j])));
	//		//c2Up = velocity_old[j - 2 * nparticlesH] - velocity_old[j];

	//		//then we can elso have a streatch spring upwards
	//		calcSpringForce(j, j - nparticlesH, kUp, cUp, oaSt, oldIndex);
	//		
	//		//kUp = ((particle_old[j - nparticlesH] - particle_old[j])*((norm(particle_old[j - nparticlesH] - particle_old[j]) - oaSt) / norm(particle_old[j - nparticlesH] - particle_old[j])));
	//		//cUp = velocity_old[j - nparticlesH] - velocity_old[j];
	//	}
	//	else {
	//		//if we can have a streatch spring upwards
	//		if (j + 1 > nparticlesH)
	//		{
	//			calcSpringForce(j, j - nparticlesH, kUp, cUp, oaSt, oldIndex);
	//			//kUp = ((particle_old[j - nparticlesH] - particle_old[j])*((norm(particle_old[j - nparticlesH] - particle_old[j]) - oaSt) / norm(particle_old[j - nparticlesH] - particle_old[j])));
	//			//cUp = velocity_old[j - nparticlesH] - velocity_old[j];
	//		}
	//		else {
	//			kUp = glm::vec3(0.f, 0.f, 0.f);
	//			cUp = glm::vec3(0.f, 0.f, 0.f);
	//		}

	//		k2Up = glm::vec3(0.f, 0.f, 0.f);
	//		c2Up = glm::vec3(0.f, 0.f, 0.f);
	//	}

	//	//if we can have a bend spring to the right
	//	if ((j + 1) % nparticlesH > 0 && (j + 1) % nparticlesH < nparticlesH - 1) {
	//		
	//		calcSpringForce(j, j + 2, k2Right, c2Right, oaB, oldIndex);

	//		//k2Right = ((particle_old[j + 2] - particle_old[j])*((norm(particle_old[j + 2] - particle_old[j]) - oaB) / norm(particle_old[j + 2] - particle_old[j])));
	//		//c2Right = velocity_old[j + 2] - velocity_old[j];

	//		//then we can elso have a streatch spring to the right
	//		calcSpringForce(j, j + 1, kRight, cRight, oaSt, oldIndex);

	//		//kRight = ((particle_old[j + 1] - particle_old[j])*((norm(particle_old[j + 1] - particle_old[j]) - oaSt) / norm(particle_old[j + 1] - particle_old[j])));
	//		//cRight = velocity_old[j + 1] - velocity_old[j];
	//	}
	//	else {
	//		//if we can have a streatch spring to the right
	//		if ((j + 1) % nparticlesH != 0) {

	//			calcSpringForce(j, j + 1, kRight, cRight, oaSt, oldIndex);

	//			//kRight = ((particle_old[j + 1] - particle_old[j])*((norm(particle_old[j + 1] - particle_old[j]) - oaSt) / norm(particle_old[j + 1] - particle_old[j])));
	//			//cRight = velocity_old[j + 1] - velocity_old[j];
	//		}
	//		else {
	//			kRight = glm::vec3(0.f, 0.f, 0.f);
	//			cRight = glm::vec3(0.f, 0.f, 0.f);
	//		}
	//		k2Right = glm::vec3(0.f, 0.f, 0.f);
	//		c2Right = glm::vec3(0.f, 0.f, 0.f);
	//	}

	//	//if we can have a bend spring downwards
	//	if (j + 1 <= nparticlesH*(nparticlesV - 2)) {

	//		calcSpringForce(j, j + 2 * nparticlesH, k2Down, c2Down, oaB, oldIndex);

	//		//k2Down = ((particle_old[j + 2 * nparticlesH] - particle_old[j])*((norm(particle_old[j + 2 * nparticlesH] - particle_old[j]) - oaB) / norm(particle_old[j + 2 * nparticlesH] - particle_old[j])));
	//		//c2Down = velocity_old[j + 2 * nparticlesH] - velocity_old[j];

	//		//then we can elso have a streatch spring downwards
	//		calcSpringForce(j, j + nparticlesH, kDown, cDown, oaSt, oldIndex);

	//		//kDown = ((particle_old[j + nparticlesH] - particle_old[j])*((norm(particle_old[j + nparticlesH] - particle_old[j]) - oaSt) / norm(particle_old[j + nparticlesH] - particle_old[j])));
	//		//cDown = velocity_old[j + nparticlesH] - velocity_old[j];
	//	}
	//	else {
	//		//if we can have a streatch spring downwards
	//		if (j + 1 <= nparticlesH*(nparticlesV - 1)) {
	//			calcSpringForce(j, j + nparticlesH, kDown, cDown, oaSt, oldIndex);

	//			//kDown = ((particle_old[j + nparticlesH] - particle_old[j])*((norm(particle_old[j + nparticlesH] - particle_old[j]) - oaSt) / norm(particle_old[j + nparticlesH] - particle_old[j])));
	//			//cDown = velocity_old[j + nparticlesH] - velocity_old[j];
	//		}
	//		else {
	//			kDown = glm::vec3(0.f, 0.f, 0.f);
	//			cDown = glm::vec3(0.f, 0.f, 0.f);
	//		}
	//		k2Down = glm::vec3(0.f, 0.f, 0.f);
	//		c2Down = glm::vec3(0.f, 0.f, 0.f);
	//	}

	//	//if we can have a bend spring to the left
	//	if ((j + 1) % nparticlesH > 2) {
	//		calcSpringForce(j, j - 2, k2Left, c2Left, oaB, oldIndex);
	//		
	//		//k2Left = ((particle_old[j - 2] - particle_old[j])*((norm(particle_old[j - 2] - particle_old[j]) - oaB) / norm(particle_old[j - 2] - particle_old[j])));
	//		//c2Left = velocity_old[j - 2] - velocity_old[j];

	//		//then we can elso have a streatch spring to the left
	//		calcSpringForce(j, j - 1, kLeft, cLeft, oaSt, oldIndex);

	//		//kLeft = ((particle_old[j - 1] - particle_old[j])*((norm(particle_old[j - 1] - particle_old[j]) - oaSt) / norm(particle_old[j - 1] - particle_old[j])));
	//		//cLeft = velocity_old[j - 1] - velocity_old[j];
	//	}
	//	else {
	//		//if we can have a streatch spring to the left
	//		if ((j + 1) % nparticlesH != 1) {
	//			calcSpringForce(j, j - 1, kLeft, cLeft, oaSt, oldIndex);

	//			//kLeft = ((particle_old[j - 1] - particle_old[j])*((norm(particle_old[j - 1] - particle_old[j]) - oaSt) / norm(particle_old[j - 1] - particle_old[j])));
	//			//cLeft = velocity_old[j - 1] - velocity_old[j];
	//		}
	//		else {
	//			kLeft = glm::vec3(0.f, 0.f, 0.f);
	//			cLeft = glm::vec3(0.f, 0.f, 0.f);
	//		}
	//		k2Left = glm::vec3(0.f, 0.f, 0.f);
	//		c2Left = glm::vec3(0.f, 0.f, 0.f);
	//	}

	//	//if we can have shear spring to the right and Upwards
	//	if ((j + 1) % nparticlesH > 0 && j + 1 > nparticlesH) {
	//		calcSpringForce(j, j - nparticlesH + 1, kUpRight, cUpRight, oaSh, oldIndex);
	//		
	//		//kUpRight = ((particle_old[j - nparticlesH + 1] - particle_old[j])*((norm(particle_old[j - nparticlesH + 1] - particle_old[j]) - oaSh) / norm(particle_old[j - nparticlesH + 1] - particle_old[j])));
	//		//cUpRight = velocity_old[j - nparticlesH + 1] - velocity_old[j];
	//	}
	//	else {
	//		kUpRight = glm::vec3(0.f, 0.f, 0.f);
	//		cUpRight = glm::vec3(0.f, 0.f, 0.f);
	//	}

	//	//if we can have shear spring to the right and downwards
	//	if ((j + 1) % nparticlesH > 0 && j + 1 < nparticlesH*(nparticlesV - 1)) {
	//		calcSpringForce(j, j + nparticlesH + 1, kDownRight, cDownRight, oaSh, oldIndex);
	//		
	//		//kDownRight = ((particle_old[j + nparticlesH + 1] - particle_old[j])*((norm(particle_old[j + nparticlesH + 1] - particle_old[j]) - oaSh) / norm(particle_old[j + nparticlesH + 1] - particle_old[j])));
	//		//cDownRight = velocity_old[j + nparticlesH + 1] - velocity_old[j];
	//	}
	//	else {
	//		kDownRight = glm::vec3(0.f, 0.f, 0.f);
	//		cDownRight = glm::vec3(0.f, 0.f, 0.f);
	//	}

	//	//if we can have shear spring to the left and downwards
	//	if ((j + 1) % nparticlesH != 1 && j + 1 < nparticlesH*(nparticlesV - 1)) {
	//		calcSpringForce(j, j + nparticlesH - 1, kDownLeft, cDownLeft, oaSh, oldIndex);

	//		//kDownLeft = ((particle_old[j + nparticlesH - 1] - particle_old[j])*((norm(particle_old[j + nparticlesH - 1] - particle_old[j]) - oaSh) / norm(particle_old[j + nparticlesH - 1] - particle_old[j])));
	//		//cDownLeft = velocity_old[j + nparticlesH - 1] - velocity_old[j];
	//	}
	//	else {
	//		kDownLeft = glm::vec3(0.f, 0.f, 0.f);
	//		cDownLeft = glm::vec3(0.f, 0.f, 0.f);
	//	}

	//	//if we can have shear spring to the left and Upwards
	//	if ((j + 1) % nparticlesH != 1 && j + 1 > nparticlesH) {
	//		calcSpringForce(j, j - nparticlesH - 1, kUpLeft, cUpLeft, oaSh, oldIndex);

	//		//kUpLeft = ((particle_old[j - nparticlesH - 1] - particle_old[j])*((norm(particle_old[j - nparticlesH - 1] - particle_old[j]) - oaSh) / norm(particle_old[j - nparticlesH - 1] - particle_old[j])));
	//		//cUpLeft = velocity_old[j - nparticlesH - 1] - velocity_old[j];
	//	}
	//	else {
	//		kUpLeft = glm::vec3(0.f, 0.f, 0.f);
	//		cUpLeft = glm::vec3(0.f, 0.f, 0.f);
	//	}

	//	//calculate the new velosity
	//	glm::vec3 old_vel = glm::vec3(vA[oldIndex][j].vel[0], vA[oldIndex][j].vel[1], vA[oldIndex][j].vel[2]);

	//	glm::vec3 new_vel = old_vel + (timestep / particleMass)*(particleMass*g) + kSt  * (kUp + kLeft + kRight + kDown) +
	//						kSh*(kUpLeft + kUpRight + kDownLeft + kDownRight) + kB*(k2Up + k2Right + k2Down + k2Left) + cSt*(cUp + cLeft + cRight + cDown)
	//						+ cSh*(cUpLeft + cUpRight + cDownLeft + cDownRight) + cB*(c2Up + c2Right + c2Down + c2Left));

	//	vA[currentArray][j].vel[0] = new_vel.x;
	//	vA[currentArray][j].vel[1] = new_vel.y;
	//	vA[currentArray][j].vel[2] = new_vel.z;

	//	//calculate the new possition
	//	glm::vec3 pPos = glm::vec3(vA[currentArray][j].pos[0], vA[currentArray][j].pos[1], vA[currentArray][j].pos[2]);
	//	
	//	glm::vec3 newP = pPos + timestep*new_vel;
	//	
	//	vA[currentArray][j].pos[0] = newP.x;
	//	vA[currentArray][j].pos[1] = newP.y;
	//	vA[currentArray][j].pos[2] = newP.z;
	//}


	//// update buffer data
	//for (auto i = 0; i < nverts; ++i) {
	//	vertexBufferPointer[i].pos[0] = vA[currentArray][i].pos[0];
	//	vertexBufferPointer[i].pos[1] = vA[currentArray][i].pos[1];
	//	vertexBufferPointer[i].pos[2] = vA[currentArray][i].pos[2];

	//	//vertexBufferPointer[i].norm[0] = vA[currentArray][i].norm[0];
	//	//vertexBufferPointer[i].norm[1] = vA[currentArray][i].norm[1];
	//	//vertexBufferPointer[i].norm[2] = vA[currentArray][i].norm[2];
	//}

	//// change curr index
	//currentArray = (currentArray + 1) % 2;

	std::vector<glm::vec3> pos, oPos, vel, oldvel;

	for (int i = 0; i < nverts; i++) {
		oPos.push_back(glm::vec3(vA[(currentArray + 1) % 2][i].pos[0], vA[(currentArray + 1) % 2][i].pos[1], vA[(currentArray + 1) % 2][i].pos[2]));
		pos.push_back(glm::vec3(vA[currentArray][i].pos[0], vA[currentArray][i].pos[1], vA[currentArray][i].pos[2]));
		oldvel.push_back(glm::vec3(vA[(currentArray + 1) % 2][i].vel[0], vA[(currentArray + 1) % 2][i].vel[1], vA[(currentArray + 1) % 2][i].vel[2]));
		vel.push_back(glm::vec3(vA[currentArray][i].vel[0], vA[currentArray][i].vel[1], vA[currentArray][i].vel[2]));
	}

	Euler(pos, oPos, vel, oldvel);


	for (int i = 0; i < nverts; i++) {
		vA[(currentArray + 1) % 2][i].pos[0] = oPos[i].x;
		vA[(currentArray + 1) % 2][i].pos[1] = oPos[i].y;
		vA[(currentArray + 1) % 2][i].pos[2] = oPos[i].z;

		vA[currentArray][i].pos[0] = pos[i].x;
		vA[currentArray][i].pos[1] = pos[i].y;
		vA[currentArray][i].pos[2] = pos[i].z;

		vA[(currentArray + 1) % 2][i].vel[0] = oldvel[i].x;
		vA[(currentArray + 1) % 2][i].vel[1] = oldvel[i].y;
		vA[(currentArray + 1) % 2][i].vel[2] = oldvel[i].z;

		vA[currentArray][i].vel[0] = vel[i].x;
		vA[currentArray][i].vel[1] = vel[i].y;
		vA[currentArray][i].vel[2] = vel[i].z;

		vertexBufferPointer[i].pos[0] = vA[currentArray][i].pos[0];
		vertexBufferPointer[i].pos[1] = vA[currentArray][i].pos[1];
		vertexBufferPointer[i].pos[2] = vA[currentArray][i].pos[2];

		vertexBufferPointer[i].norm[0] = vA[currentArray][i].norm[0];
		vertexBufferPointer[i].norm[1] = vA[currentArray][i].norm[1];
		vertexBufferPointer[i].norm[2] = vA[currentArray][i].norm[2];

	}

}


void Cloth::calcSpringForce(const int& pA, const int& pB, glm::vec3& _k, glm::vec3& _c, const int& springCoeff, const int& aIndex ) {

	float vec[3];
	linAlg::calculateVec(vA[aIndex][pB].pos, vA[aIndex][pA].pos, vec);
	float length = linAlg::vecLength(vec);
	float scale = (length - springCoeff) / length;
	_k = glm::vec3(vec[0] * scale, vec[1] * scale, vec[2] * scale);

	linAlg::calculateVec(vA[aIndex][pB].vel, vA[aIndex][pA].vel, vec);
	_c = glm::vec3(vec[0], vec[1], vec[2]);
}

std::vector<glm::vec3> Cloth::placeZeros() {
	std::vector<glm::vec3> zeroVector;
	for (int i = 0; i < nparticlesV*nparticlesH; i++) {
		zeroVector.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	}
	return zeroVector;
}

void Cloth::render()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 3 * ntris, GL_UNSIGNED_INT, (void*)0);
	// (mode, vertex count, type, element array buffer offset)
	glBindVertexArray(0);
}
