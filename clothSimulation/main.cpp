#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // incude frustum
#include <glm/gtc/type_ptr.hpp> 

#include "Particles.h"
#include "Shader.h"


using namespace glm;

//-----------------------
// function declarations 
//-----------------------
static void key_callback (GLFWwindow* window, int key, int scancode, int action, int mods);
static void error_callback (int error, const char* description);
void drawTriangles(vector<glm::vec3> particles, Shader phongShader);
void calculateVerticeNormal(vector <vec3> drawOrder, vector <vec3> surfaceNormals, vector <vec3> &verticeNormals);

//-----------------------
// variable declarations 
//-----------------------

GLuint vbo_triangle, vbo_triangle_colors, vbo_vertices_normals; // Vertex Buffer Objects, for storing vertices directly in the graphics card
GLint attribute_coord3d, attribute_v_color, attribute_vertices_normals;
GLint uniform_mv, uniform_p;


struct attributes {
	GLfloat coord3d[3];
	GLfloat v_color[3];
};


int main(void) {

	//-----------------------
	// variable declarations 
	//-----------------------

	/*GLfloat P[16] = { frustum[0][0], frustum[0][1], frustum[0][2], frustum[0][3]
	, frustum[1][0], frustum[1][1], frustum[1][2], frustum[1][3]
	, frustum[2][0], frustum[2][1], frustum[2][2], frustum[2][3]
	, frustum[3][0], frustum[3][1], frustum[3][2], frustum[3][3] };*/

	vector<glm::vec3> particles;
	vector<glm::vec3> particle_old;
	vector<glm::vec3> velocity;
	vector<glm::vec3> velocity_old;

	GLint attribute_coord3d;

	Shader phongShader;


	glfwSetErrorCallback(error_callback);

	// Initialise GLFW
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // window not resizable

	// Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Cloth simulation", NULL, NULL);


	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);		// set swapinterval to 1 to avoid screen tearing
	
	// Initialize GLEW 
	glewExperimental = GL_TRUE; 

	if (glewInit() != GLEW_OK)
		exit(EXIT_FAILURE);

	glfwSetKeyCallback(window, key_callback); //set key callback for window

	// place the cloth in its initial state
	placeParticles(particles); 
	particle_old = particles;
	velocity = placeZeros();
	velocity_old = velocity;


	//create shader
	phongShader.createShader("Shaders/vertexShader.glsl", "Shaders/fragmentShader.glsl");

	glEnable(GL_DEPTH_TEST);

	attribute_coord3d = glGetAttribLocation(phongShader.programID, "coord3d");
	if (attribute_coord3d == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", "coord3d");
		return 0;
	}

	attribute_v_color = glGetAttribLocation(phongShader.programID, "v_color");
	if (attribute_v_color == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", "v_color");
		return 0;
	}

	uniform_mv = glGetUniformLocation(phongShader.programID, "mv");
	if (uniform_mv == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", "mv");
		return 0;
	}

	uniform_p = glGetUniformLocation(phongShader.programID, "p");
	if (uniform_p == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", "p");
		return 0;
	}

	attribute_vertices_normals = glGetAttribLocation(phongShader.programID, "normals");
	if (attribute_vertices_normals == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", "normals");
		return 0;
	}

	// run untill window should close
	while (!glfwWindowShouldClose(window)) {

		int width, height;

		//set up viewport
		glfwGetFramebufferSize(window, &width, &height);
		float ratio = width / (float)height;
		glViewport(0, 0, width, height);

		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//draw here
		drawTriangles(particles, phongShader);
		for (int skipp = 0; skipp < 7; skipp++){
			Euler(particles, particle_old, velocity, velocity_old); // calculate the cloths next position
		}

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}

static void key_callback (GLFWwindow* window, int key, int scancode, int action, int mods) {
	// Close window if ESC is pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

static void error_callback (int error, const char* description) {
	fputs(description, stderr);
}

// Function for drawing the cloth
void drawTriangles(vector<glm::vec3> particles, Shader phongShader) {
	
	GLuint ibo_cloth_elements;
	GLuint vbo_cloth_vertices, vbo_cloth_colors;

	glm::mat4 frustum = glm::frustum(0, 2, -2, 0, -2 , 1); // left, right, bottom, top, near, far
	mat4 view = glm::lookAt(vec3(0.3f, 0.0f, -2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)); // get the view matrix

	vector<glm::vec3> drawOrder = MakeTriangles(particles); // orders input so that the normal points in the correct direction

	const int vectorSize = ((nrOfParticlesHorizontally -1 )*(nrOfParticlesVertically - 1) )*6*3;
	GLfloat clothVertices[vectorSize]; // contains the cloth vertices coordiates
	GLushort clothElements[vectorSize]; // contains the order the particles should be drawn in
	GLfloat clothColors[vectorSize]; // contains the vertices colors
	GLfloat normals[vectorSize/3];

	// insert the coordinates, elements and colors in arrays.
	// i is used for taking 3 particles each loop, 3 particles -> i = i + 3
	// while j is used to save the 3 particles x, y and z coordinates and color in the arrays, 9 coordinates -> j = j + 9
	// loop continues untill there is less than 3 particles left in drawOrder
	int row = 0;
	vec3 P1 = vec3(0.0f);
	vec3 P2 = vec3(0.0f);
	vec3 P3 = vec3(0.0f);
	vec3 V1 = vec3(0.0f);
	vec3 V2 = vec3(0.0f);
	vector <vec3> surfaceNormals;
	vector <vec3> verticeNormals;

	for (int i = 0, j = 0; i < drawOrder.size(); i = i + 3, j = j + 9) {

		if (i % 2 == 0) {
			row++;
		}

		// insert the first particles coordinates
		clothVertices[j] = (drawOrder[i].x);
		clothVertices[j + 1] = (drawOrder[i].y);
		clothVertices[j + 2] = (drawOrder[i].z);

		// set draw order, the input is orderd
		clothElements[i] = i;

		if (row % 2 == 0) {
			// the first triangles colors
			clothColors[j] = 0.5f;
			clothColors[j + 1] = 0.0f;
			clothColors[j + 2] = 1.0f;
		}
		else {
			// the first triangles colors
			clothColors[j] = 1.0f;
			clothColors[j + 1] = 0.0f;
			clothColors[j + 2] = 1.0f;
		}

		if (i + 1 < drawOrder.size()) {

			// insert the second particles coordinates
			clothVertices[j + 3] = (drawOrder[i + 1].x);
			clothVertices[j + 4] = (drawOrder[i + 1].y);
			clothVertices[j + 5] = (drawOrder[i + 1].z);

			clothElements[i + 1] = (i + 1);


			if (row % 2 == 0) {
				// the second triangles colors
				clothColors[j + 3] = 0.5f;
				clothColors[j + 4] = 0.0f;
				clothColors[j + 5] = 1.0f;
			}
			else {
				// the second triangles colors
				clothColors[j + 3] = 1.0f;
				clothColors[j + 4] = 1.0f;
				clothColors[j + 5] = 1.0f;
			}
		}

		if (i + 2 < drawOrder.size()) {
			// insert the third particles coordinates
			clothVertices[j + 6] = (drawOrder[i + 2].x);
			clothVertices[j + 7] = (drawOrder[i + 2].y);
			clothVertices[j + 8] = (drawOrder[i + 2].z);

			clothElements[i + 2] = (i + 2);

			if (row % 2 == 0) {
				// the third triangles colors
				clothColors[j + 6] = 0.5f;
				clothColors[j + 7] = 0.0f;
				clothColors[j + 8] = 1.0f;
			}
			else {
				// the third triangles colors
				clothColors[j + 6] = 0.5f;
				clothColors[j + 7] = 1.0f;
				clothColors[j + 8] = 1.0f;

			}

			//calculate the surface normal
			P1 = vec3(drawOrder[i].x, drawOrder[i].y, drawOrder[i].z);
			P2 = vec3(drawOrder[i + 1].x, drawOrder[i + 1].y, drawOrder[i + 1].z);
			P3 = vec3(drawOrder[i + 2].x, drawOrder[i + 2].y, drawOrder[i + 2].z);

			V1 = P2 - P1;
			V2 = P3 - P1;

			surfaceNormals.push_back(cross(V2, V1)); // calculate surface normal
		}
	}

	calculateVerticeNormal(drawOrder, surfaceNormals, verticeNormals);

	for (int i = 0; i < verticeNormals.size(); i++){
		normals[i] = verticeNormals[i].x;
		normals[i] = verticeNormals[i].y;
		normals[i] = verticeNormals[i].z;
	}

	// generate and bind buffer for clothVertices
	glGenBuffers(1, &vbo_cloth_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cloth_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(clothVertices), clothVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	// generate and bind buffer for clothElements
	glGenBuffers(1, &ibo_cloth_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cloth_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(clothElements), clothElements, GL_STATIC_DRAW);

	// generate and bind buffer for clothColors
	glGenBuffers(1, &vbo_cloth_colors);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cloth_colors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(clothColors), clothColors, GL_STATIC_DRAW);

	// generate and bind buffer for verticeNormals
	glGenBuffers(1, &vbo_vertices_normals);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

	glUseProgram(phongShader.programID);

	// calculate the global transform matrix
	glUniformMatrix4fv(uniform_p, 1, GL_FALSE, glm::value_ptr(frustum));

	glm::mat4 mv = view;
	glUniformMatrix4fv(uniform_mv, 1, GL_FALSE, glm::value_ptr(mv));

	//bind cloth coordinates
	glEnableVertexAttribArray(attribute_coord3d);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cloth_vertices);
	glVertexAttribPointer(attribute_coord3d, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// bind Colors
	glEnableVertexAttribArray(attribute_v_color);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cloth_colors);
	glVertexAttribPointer(attribute_v_color, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// bind normals
	glEnableVertexAttribArray(attribute_vertices_normals);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices_normals);
	glVertexAttribPointer(attribute_vertices_normals, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// bind buffer and draw
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cloth_elements);
	int size; 
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size); // use glGetBufferParameteriv to get the buffer size
	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

	//clean up
	glDisableVertexAttribArray(attribute_coord3d);
	glDisableVertexAttribArray(attribute_v_color);
	glDeleteBuffers(1, &ibo_cloth_elements);

}

//calculate the vertices normals from the surface normals
void calculateVerticeNormal(vector <vec3> drawOrder, vector <vec3> surfaceNormals, vector <vec3> &verticeNormals) {
	vec3 tempNormal;
	int row = 1;
	bool firstParticle = true;
	for (int i = 0; i < drawOrder.size() / 3; i++) { // TODO: i måste följa antalet partiklar inte antalet surfacenormals!!
		if (row == 1 || row == nrOfParticlesVertically) { // the first row of particles
			if (i != 0 && i % (nrOfParticlesHorizontally * row) == 0 && row == 0) { // last particle on the first row
				tempNormal = vec3(surfaceNormals[(i - 1) * 2].x + surfaceNormals[(i - 1) * 2 - 1].x,
					surfaceNormals[(i - 1) * 2].y + surfaceNormals[(i - 1) * 2 - 1].y,
					surfaceNormals[(i - 1) * 2].z + surfaceNormals[(i - 1) * 2 - 1].z);
				row++;
				firstParticle = true;
			}
			else if (firstParticle == true) { // first particle on the first row
				tempNormal = vec3(surfaceNormals[i].x,
					surfaceNormals[i].y,
					surfaceNormals[i].z);
				firstParticle = false;

			}
			else{ //middle particle on first row 
				//tempNormal = vec3(surfaceNormals[(i - 1) * 2].x + surfaceNormals[(i - 1) * 2 - 1].x + surfaceNormals[i * 2 - 1].x,
				//				  surfaceNormals[(i - 1) * 2].y + surfaceNormals[(i - 1) * 2 - 1].y + surfaceNormals[i * 2 - 1].y,
				//				  surfaceNormals[(i - 1) * 2].z + surfaceNormals[(i - 1) * 2 - 1].z + surfaceNormals[i * 2 - 1].z);
			}
		}
		else if (row == nrOfParticlesVertically) { // last row of particles

			if (row == nrOfParticlesVertically && i / (nrOfParticlesHorizontally * row) == 1) { // last particle in the cloth is only connected by 1 triangle
				tempNormal = vec3(surfaceNormals[(i - nrOfParticlesHorizontally - 1) * 2].x,
					surfaceNormals[(i - nrOfParticlesHorizontally - 1) * 2].y,
					surfaceNormals[(i - nrOfParticlesHorizontally - 1) * 2].z);
				firstParticle = true;
			}
			else if (firstParticle == true) { // first particle
				tempNormal = vec3(surfaceNormals[(i - nrOfParticlesHorizontally) * 2].x + surfaceNormals[(i - nrOfParticlesHorizontally) * 2 - 1].x,
					surfaceNormals[(i - nrOfParticlesHorizontally) * 2].y + surfaceNormals[(i - nrOfParticlesHorizontally) * 2 - 1].y,
					surfaceNormals[(i - nrOfParticlesHorizontally) * 2].z + surfaceNormals[(i - nrOfParticlesHorizontally) * 2 - 1].z);
				firstParticle = false;
			}
			else { // middle particle
				tempNormal = vec3(surfaceNormals[(i - nrOfParticlesHorizontally) * 2].x + surfaceNormals[(i - nrOfParticlesHorizontally) * 2 - 1].x
					+ surfaceNormals[(i - nrOfParticlesHorizontally - 1) * 2].x,
					surfaceNormals[(i - nrOfParticlesHorizontally) * 2].y + surfaceNormals[(i - nrOfParticlesHorizontally) * 2 - 1].y
					+ surfaceNormals[(i - nrOfParticlesHorizontally - 1) * 2].y,
					surfaceNormals[(i - nrOfParticlesHorizontally) * 2].z + surfaceNormals[(i - nrOfParticlesHorizontally) * 2 - 1].z
					+ surfaceNormals[(i - nrOfParticlesHorizontally - 1) * 2].z);
			}

		}
		else { // a middle row
			if (firstParticle == true) { // first particle on a middle row, affected by 3 triangles, two from the row above and the first one on its row
				tempNormal = vec3(surfaceNormals[(i - nrOfParticlesHorizontally) * 2].x + surfaceNormals[(i - nrOfParticlesHorizontally) * 2 - 1].x + surfaceNormals[i * 2 - 1].x,
					surfaceNormals[(i - nrOfParticlesHorizontally) * 2].y + surfaceNormals[(i - nrOfParticlesHorizontally) * 2 - 1].y + surfaceNormals[i * 2 - 1].y,
					surfaceNormals[(i - nrOfParticlesHorizontally) * 2].z + surfaceNormals[(i - nrOfParticlesHorizontally) * 2 - 1].z + surfaceNormals[i * 2 - 1].z);
				firstParticle = false;
			}
			else if (i / (nrOfParticlesHorizontally * row) == 1) { // last particle on a middle row
				tempNormal = vec3(surfaceNormals[(i - nrOfParticlesHorizontally - 1) * 2].x, //+ surfaceNormals[(i - 1) * 2].x + surfaceNormals[(i - 1) * 2 - 1].x,
					surfaceNormals[(i - nrOfParticlesHorizontally - 1) * 2].y, //+ surfaceNormals[(i - 1) * 2].y + surfaceNormals[(i - 1) * 2 - 1].y,
					surfaceNormals[(i - nrOfParticlesHorizontally - 1) * 2].z); // + surfaceNormals[(i - 1) * 2].z + surfaceNormals[(i - 1) * 2 - 1].z);
				firstParticle = true;

			} else { // middle particle middle row
				tempNormal = vec3(surfaceNormals[i].x + surfaceNormals[(i - nrOfParticlesHorizontally - 1) * 2].x + surfaceNormals[(i - nrOfParticlesHorizontally) * 2].x
					+ surfaceNormals[(i - nrOfParticlesHorizontally) * 2 - 1].x + surfaceNormals[(i - 1) * 2].x + surfaceNormals[(i - 1) * 2 - 1].x
					+ surfaceNormals[i * 2 - 1].x,
					surfaceNormals[i].y + surfaceNormals[(i - nrOfParticlesHorizontally - 1) * 2].y + surfaceNormals[(i - nrOfParticlesHorizontally) * 2].y
					+ surfaceNormals[(i - nrOfParticlesHorizontally) * 2 - 1].y + surfaceNormals[(i - 1) * 2].y + surfaceNormals[(i - 1) * 2 - 1].y
					+ surfaceNormals[i * 2 - 1].y,
					surfaceNormals[i].z + surfaceNormals[(i - nrOfParticlesHorizontally - 1) * 2].z + surfaceNormals[(i - nrOfParticlesHorizontally) * 2].z
					+ surfaceNormals[(i - nrOfParticlesHorizontally) * 2 - 1].z + surfaceNormals[(i - 1) * 2].z + surfaceNormals[(i - 1) * 2 - 1].z
					+ surfaceNormals[i * 2 - 1].z);
			}
		}

		tempNormal = normalize(tempNormal);

		verticeNormals.push_back(tempNormal);
	}
}