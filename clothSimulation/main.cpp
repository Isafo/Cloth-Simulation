#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // incude frustum

#include "Euler.h"

#include "shader.h"

using namespace glm;

//-----------------------
// function declarations 
//-----------------------
static void key_callback (GLFWwindow* window, int key, int scancode, int action, int mods);
static void error_callback (int error, const char* description);
void drawTriangles (vector<glm::vec3> particles);


int main(void) {

	//-----------------------
	// variable declarations 
	//-----------------------

	vector<glm::vec3> particles;
	vector<glm::vec3> particle_old;
	vector<glm::vec3> velocity;
	vector<glm::vec3> velocity_old;

	glm::mat4 frustum = glm::frustum(10, -10, -10, 4, 0, 10);

	GLfloat P[16] = { frustum[0][0], frustum[0][1], frustum[0][2], frustum[0][3]
					, frustum[1][0], frustum[1][1], frustum[1][2], frustum[1][3]
					, frustum[2][0], frustum[2][1], frustum[2][2], frustum[2][3]
					, frustum[3][0], frustum[3][1], frustum[3][2], frustum[3][3] };

	mat4 V = glm::lookAt(vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 1.0f)); // get the view matrix

	Shader phongShader;
	GLuint textureID;


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

	// place the cloth in its initial state
	placeParticles(particles); 
	particle_old = particles;
	velocity = placeZeros();
	velocity_old = velocity;

	//create shader
	phongShader.createShader("Shaders/vertexShader.glsl", "Shaders/fragmentShader.glsl");

	

	GLint location_Mv = glGetUniformLocation(phongShader.programID, "Mv");
	GLint location_P = glGetUniformLocation(phongShader.programID, "P");
	if (location_P != -1) { // If the variable is not found , -1 is returned
		glUniformMatrix4fv(location_P, 1, GL_FALSE, P); // Copy the value
	}
	GLint location_V = glGetUniformLocation(phongShader.programID, "V");


	// run untill window should close
	while (!glfwWindowShouldClose(window)) {

		int width, height;

		//set up viewport
		glfwGetFramebufferSize(window, &width, &height);
		P[0] = P[5] * height / width;
		glViewport(0, 0, width, height);

		//draw here
		drawTriangles (particles);
		particles = Euler(particles, particle_old, velocity, velocity_old); // calculate the cloths next position

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
	vector<glm::vec3> drawOrder = MakeTriangles(particles);
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