#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // incude frustum
#include <glm/gtc/type_ptr.hpp> 

#include "Particles.h"
#include "Shader.h"
#include "FindClosestNeighbor.h"

using namespace glm;

//-----------------------
// function declarations 
//-----------------------
static void key_callback (GLFWwindow* window, int key, int scancode, int action, int mods);
static void error_callback (int error, const char* description);
void drawTriangles(vector<glm::vec3> particles, Shader phongShader);
void checkMouseButtons(GLFWwindow* window, int height, int width, vector<glm::vec3> &particles, bool &particleInStatic, vector<int> &staticParticles, bool &pressed, int &selectedParticlePos);
void convertMouseCordToOpenGLCord(double &mousePosX, double &mousePosY, int width, int height);
vec3 newPos(float oldZ, float mousePosX, float mousePosY, vec3 cameraPos, int clipingPlaneNear);

//-----------------------
// variable declarations 
//-----------------------

GLuint vbo_triangle, vbo_triangle_colors; // Vertex Buffer Objects, for storing vertices directly in the graphics card
GLint attribute_coord3d, attribute_v_color;
GLint uniform_mvp;
const vec3 cameraPosition = vec3(0.0f, 0.0f, -3.0f);
const vec3 viewDirection = vec3(0.0f, 0.0f, 0.0f);
const vec3 up = vec3(0.0f, -1.0f, 0.0f);
const int clipingPlaneNear = -2;
const float maxSpringLenght = springRestLenght * 25;


struct attributes {
	GLfloat coord3d[3];
	GLfloat v_color[3];
};


int main(void) {

	//-----------------------
	// variable declarations 
	//-----------------------

	// bools for checking what simulation to run
	bool clothHanging = true;
	bool pressed = false; //checks for the first call of mouse being pressed down
	bool particleInStatic = false; // bool for checking if particle has been pushed back in vector
	int selectedParticlePos = -1;

	vector<glm::vec3> particles;
	vector<glm::vec3> particle_old;
	vector<glm::vec3> velocity;
	vector<glm::vec3> velocity_old;
	vector<int> staticParticles; // list of all particles that are static, ignored by Euler

	GLint attribute_coord3d;

	Shader phongShader;

	glfwSetErrorCallback(error_callback);

	// Initialise GLFW
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // window not resizable

	// Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow(800, 800, "Cloth simulation", NULL, NULL);


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

	// initialise static particles if there are any for the specific simulation
	if (clothHanging == true){
		initializeStaticParticles(staticParticles);
	}

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

	uniform_mvp = glGetUniformLocation(phongShader.programID, "mvp");
	if (uniform_mvp == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", "mvp");
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

		checkMouseButtons(window, width, height, particles, particleInStatic, staticParticles, pressed, selectedParticlePos); // check if a mouse is pressed

		//draw here
		drawTriangles(particles, phongShader);
		for (int skipp = 0; skipp < 12; skipp++){// to enhance preformanse since movment in one timestep is so smale that we dont need to draw every timestep.
			Euler(particles, particle_old, velocity, velocity_old, staticParticles); // calculate the cloths next position
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

	glm::mat4 frustum = glm::frustum(-1, 1, -1, 1, clipingPlaneNear, 1); // left, right, bottom, top, near, far
	mat4 view = glm::lookAt(cameraPosition, viewDirection, up); // get the view matrix

	vector<glm::vec3> drawOrder = MakeTriangles(particles); // orders input so that the normal points in the correct direction

	const int vectorSize = ((nrOfParticlesHorizontally -1 )*(nrOfParticlesVertically - 1) )*6*3;
	GLfloat clothVertices[vectorSize]; // contains the cloth vertices coordiates
	GLushort clothElements[vectorSize]; // contains the order the particles should be drawn in
	GLfloat clothColors[vectorSize]; // contains the vertices colors

	// insert the coordinates, elements and colors in arrays.
	// i is used for taking 3 particles each loop, 3 particles -> i = i + 3
	// while j is used to save the 3 particles x, y and z coordinates and color in the arrays, 9 coordinates -> j = j + 9
	// loop continues untill there is less than 3 particles left in drawOrder
	int row = 0;
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
		}
	}

	// generate and bind buffer for clothVertices
	glGenBuffers(1, &vbo_cloth_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cloth_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(clothVertices), clothVertices, GL_STATIC_DRAW);

	// generate and bind buffer for clothElements
	glGenBuffers(1, &ibo_cloth_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cloth_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(clothElements), clothElements, GL_STATIC_DRAW);

	// generate and bind buffer for clothColors
	glGenBuffers(1, &vbo_cloth_colors);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cloth_colors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(clothColors), clothColors, GL_STATIC_DRAW);

	glUseProgram(phongShader.programID);

	// calculate the global transform matrix
	glm::mat4 mvp = frustum * view;
	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

	//bind cloth coordinates
	glEnableVertexAttribArray(attribute_coord3d);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cloth_vertices);
	glVertexAttribPointer(attribute_coord3d, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// bind Colors
	glEnableVertexAttribArray(attribute_v_color);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cloth_colors);
	glVertexAttribPointer(attribute_v_color, 3, GL_FLOAT, GL_FALSE, 0, 0);

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

//	Check if either left or right mousebuttons are pressed down
//	If left mouse button is pressed down set the closest particle to follow the mouse, make it static for euler.
//	If right mouse button is pressed down destroy the springs at the cursor location.
void checkMouseButtons(GLFWwindow* window, int height, int width, vector<glm::vec3> &particles, bool &particleInStatic, vector<int> &staticParticles, bool &pressed, int &selectedParticlePos){
	double cursorPosX, cursorPosY; // position of the cursor

	// Check Mousebutton status
	int stateRightButton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	int stateLeftButton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

	// If mouse is pressed get cursor position and set
	// the closest particles position to the cursor position to be able to
	// pull the cloth
	if (stateLeftButton == GLFW_PRESS){
		glfwGetCursorPos(window, &cursorPosX, &cursorPosY);
		glfwGetCursorPos(window, &cursorPosX, &cursorPosY);

		// Convert to openGL coordinates
		convertMouseCordToOpenGLCord(cursorPosX, cursorPosY, width, height);
		
		if (!pressed){
			// fixa variabler istället för siffror i vec
			selectedParticlePos = FindClosestNeighbor(particles, cursorPosX, cursorPosY,
														cameraPosition,	clipingPlaneNear, maxSpringLenght);			
			pressed = true;
		}

		if (selectedParticlePos != -1){ // was a particle found?
			// Set the closest particles position to the mousePos and make it static if it isnt allready
			particles[selectedParticlePos] = newPos(particles[selectedParticlePos].z, -cursorPosX, cursorPosY, cameraPosition, clipingPlaneNear);
			
			if (particleInStatic == false) { // the particle is not static
				staticParticles.push_back(selectedParticlePos);
				particleInStatic = true;
			}
		}
	} else if (stateLeftButton == GLFW_RELEASE && particleInStatic == true){ // Set the particle that was being pulled to non static if there has has been an addition
		staticParticles.pop_back();
		selectedParticlePos = -1;
		particleInStatic = false;
		pressed = false;
	} else if (stateRightButton == GLFW_PRESS) { // cut the cloth
		glfwGetCursorPos(window, &cursorPosX, &cursorPosY);
		glfwGetCursorPos(window, &cursorPosX, &cursorPosY);

		// Convert to openGL coordinates
		convertMouseCordToOpenGLCord(cursorPosX, cursorPosY, width, height);

		// Destroy the springs at the cursor location
	}
}


// Check what halfplane/quatrant the cursor is in and calculate its position in OpenGL coordinates.
void convertMouseCordToOpenGLCord(double &mousePosX, double &mousePosY, int width, int height){
	
	
	if (mousePosX < width / 2) {				//check if cursor is in left half plane if so take the negative mousePos add (width / 2) to make origo in center of window, then normalize and make it negative
		mousePosX = (-2) * (- mousePosX + (width / 2)) / (width / 2);
	} else if (mousePosX == width / 2) { 		// If mouse position equals width / 2 then it is in the center
		mousePosX = 0;
	} else {									// else the cursor is in the right half plane, subtract (width / 2) to make origo in center of window the normalize
		mousePosX = 2*(mousePosX - (width / 2)) / (width / 2);
	}

	// Same as for X but add/subtract (height / 2) and make upper half positive and lower negative
	if (mousePosY < height / 2) {
		mousePosY = 2* (-mousePosY + (height / 2)) / (height / 2);
	} else if (mousePosY == height / 2) {
		mousePosY = 0;
	} else {
		mousePosY = (-2) * (mousePosY - (height / 2)) / (height / 2);
	}
}

vec3 newPos(float oldZ, float mousePosX, float mousePosY, vec3 cameraPos, int clipingPlaneNear){
	//create a 3D position for the mouse
	//The mouse coordinates are already scaled to match openGL
	glm::vec3 mousePosition = glm::vec3(mousePosX, mousePosY, clipingPlaneNear);

	//create a ray from the camera to the mouse
	glm::vec3 rayN = glm::vec3(0, 0, 1);//mousePosition - cameraPos;

	//calculate new x and y positions for the mass. z is unchanged.
	//c could help to make the cloth follow the mouse even if the camera is moved, doesn't work rigth now
	float newX = mousePosX + rayN.x, newY = mousePosY + rayN.y;

	//cout << "c " << c << endl;
	//cout << "X " << newX << " Y " << newY << " Z " << oldZ << endl;
	
	return vec3(newX, newY, oldZ);
}