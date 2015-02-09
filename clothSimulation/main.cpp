#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace glm;

//-----------------------
// function declarations 
//-----------------------
static void key_callback (GLFWwindow* window, int key, int scancode, int action, int mods);
static void error_callback (int error, const char* description);
void drawTriangle (float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);

int main(void) {

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

	// run untill window should close
	while (!glfwWindowShouldClose(window)) {

		int width, height;

		//set up viewport
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		//draw here
		drawTriangle (-1.f, -1.f, 0.f, 1.f, 1.f, 0.f, -1.f, 1.f, 0.f);


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
** Function for drawing a triangle, input should be particle coordinates for particle 1 , 2 then 3 to make the normal point the correct direction
*/
void drawTriangle (float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
	glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(x1, y1, z1);
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(x2, y2, z2);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(x3, y3, z3);
	glEnd();
}