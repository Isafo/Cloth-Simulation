#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

#include "Euler.h"

using namespace glm;

//-----------------------
// function declarations 
//-----------------------
static void key_callback (GLFWwindow* window, int key, int scancode, int action, int mods);
static void error_callback (int error, const char* description);
void drawTriangles (vector<glm::vec3> particles);


const int nrOfParticlesHorizontally = 100;
const int nrOfParticlesVertically = 100;

int main(void) {

	//-----------------------
	// variable declarations 
	//-----------------------

	const float springRestLenght = 0.2f;
	vector<glm::vec3> particles;


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
	glfwSwapInterval(1); // set swapinterval to 1 to avoid screen tearing
	glewExperimental = GL_TRUE; // Initialize GLEW 

	if (glewInit() != GLEW_OK)
		exit(EXIT_FAILURE);

	glfwSetKeyCallback(window, key_callback); //set key callback for window

	
	placeParticles(nrOfParticlesHorizontally, nrOfParticlesVertically, springRestLenght, particles); // place the cloth in its initial state

	// run untill window should close
	while (!glfwWindowShouldClose(window)) {

		int width, height;

		//set up viewport
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		//draw here
		drawTriangles (particles);
		particles = Euler(particles); // calculate the cloths next position

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

/*
** Function for drawing a triangle between the neighbouring particles, input should be ordered so that the normal points in the correct direction
*/
void drawTriangles (vector<glm::vec3> particles) {
	vector<glm::vec3> drawOrder = MakeTriangles(particles, nrOfParticlesHorizontally, nrOfParticlesVertically);
	for (int i = 0; i + 2 < drawOrder.size(); i = i + 3) {
		//cout << i << " av " << drawOrder.size() << "; i + 2 = " << i + 2 << endl;
		//cout << "x = " << drawOrder[i].x << "; y = " << drawOrder[i].y << "; z = " << drawOrder[i].z << endl << endl;
		glBegin(GL_TRIANGLES);
			glColor3f(1.f, 0.f, 0.f);
			glVertex3f(drawOrder[i].x, drawOrder[i].y, drawOrder[i].z);
			glColor3f(0.f, 1.f, 0.f);
			glVertex3f(drawOrder[i + 1].x, drawOrder[i + 1].y, drawOrder[i + 1].z);
			glColor3f(0.f, 0.f, 1.f);
			glVertex3f(drawOrder[i + 2].x, drawOrder[i + 2].y, drawOrder[i + 2].z);
		glEnd();
	}
	//cout << "de va inte fel på loopen!";
}