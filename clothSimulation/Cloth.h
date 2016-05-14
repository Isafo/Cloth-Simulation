#pragma once
#include "gObject.h"
#include <vector>

class Cloth : public gObject {
public:
	Cloth()
	{
		vao = 0;
		vertexbuffer = 0;
		indexbuffer = 0;
		nverts = 0;
		ntris = 0;
	};

	Cloth(float _x, float _y, float _z, int nvertsH, int nvertsV);
	~Cloth(void);

	void createCloth();

	float* getPosition() { return position; }
	void setPosition(float* p) { position[0] = p[0]; position[1] = p[1]; position[2] = p[2]; }

	float* getOrientation() { return orientation; }
	void setOrientation(float* o) { std::copy(o, o + 16, orientation); }

	void update();



	void render();
	//glm::vec3 getDim() { return dim; }

private:
	struct Particle
	{
		GLfloat pos[3];
		GLfloat norm[3];
		GLfloat tex[2];
		GLfloat vel[3];
		bool t_static = false;
	};

	std::vector<Particle> vA[2];
	std::vector<triangle> iA;

	int currentArray = 0;

	GLuint vao;          // Vertex array object, the main handle for geometry
	int nverts; // Number of vertices in the vertex array
	int ntris;  // Number of triangles in the index array (may be zero)
	GLuint vertexbuffer; // Buffer ID to bind to GL_ARRAY_BUFFER
	GLuint indexbuffer;  // Buffer ID to bind to GL_ELEMENT_ARRAY_BUFFER
	//glm::vec3 dim;

	float position[3];
	float orientation[16];

	int nparticlesH;
	int nparticlesV;

	// spring constants
	const float springRestLenght = 0.1;
	const float timestep = 0.0015f;
	const float particleMass = 0.018f;
	const float k = 1000.0f;						 // spring konstant
	const float c = 5.0f;							 // damper constant
	const glm::vec3 g = glm::vec3(0.f, -9.82f, 0.f); // gravity


	std::vector<glm::vec3> placeZeros();
	void calcSpringForce(const int& pA, const int& pB, glm::vec3& _k, glm::vec3& _c, const int& springCoeff, const int& aIndex);

	void mapBuffers();
	void unmapBuffers();

	Particle* vertexBufferPointer;
};