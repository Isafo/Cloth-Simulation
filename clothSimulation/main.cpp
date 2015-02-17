#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Particles.h"

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

	GLfloat P[16] = { 2.42f, 0.0f, 0.0f, 0.0f
					, 0.0f, 2.42f, 0.0f, 0.0f
					, 0.0f, 0.0f, -1.0f, -1.0f
					, 0.0f, 0.0f, -0.2f, 0.0f };

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
	cout << " Size of particle  = " << particles.size() << endl;
	/*cout << " Size of particle_old  = " << particle_old.size() << endl;
	cout << " Size of velocity  = " << velocity.size() << endl;
	cout << " Size of velocity_old  = " << velocity_old.size() << endl;*/

	//create shader
	phongShader.createShader("Shaders/vertexShader.glsl", "Shaders/fragmentShader.glsl");

	GLint location_Mv = glGetUniformLocation(phongShader.programID, "Mv");
	GLint location_P = glGetUniformLocation(phongShader.programID, "P");
	if (location_P != -1) { // If the variable is not found , -1 is returned
		glUniformMatrix4fv(location_P, 1, GL_FALSE, P); // Copy the value
	}

	// run untill window should close
	while (!glfwWindowShouldClose(window)) {

		int width, height;

		/*cout << " Particle 5 x  = " << particles[5].x << " Particle 6 x  = " << particles[6].x << " Particle 7 x  = " << particles[7].x << endl;
		cout << " Particle 5 y  = " << particles[5].y << " Particle 6 y  = " << particles[6].y << " Particle 7 y  = " << particles[7].y << endl;
		cout << " Particle 5 x  = " << particles[5].z << " Particle 6 z  = " << particles[6].z << " Particle 7 z  = " << particles[7].z << endl<<endl;//*/

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