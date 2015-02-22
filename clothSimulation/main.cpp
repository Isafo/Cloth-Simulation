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

//-----------------------
// variable declarations 
//-----------------------

GLuint vbo_triangle, vbo_triangle_colors; // Vertex Buffer Objects, for storing vertices directly in the graphics card
GLint attribute_coord3d, attribute_v_color;
GLint uniform_mvp;

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
		//P[0] = P[5] * height / width;
		float ratio = width / (float)height;
		glViewport(0, 0, width, height);

		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//draw here
		drawTriangles(particles, phongShader);
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


// Function for drawing a triangle between the neighbouring particles
void drawTriangles(vector<glm::vec3> particles, Shader phongShader) {
/*		
	vector<glm::vec3> drawOrder = MakeTriangles(particles); // orders input so that the normal points in the correct direction
	for (int i = 0; i + 2 < drawOrder.size(); i = i + 3) {
		//cout << i << " av " << drawOrder.size() << "; i + 2 = " << i + 2 << endl;
		//cout << "x = " << drawOrder[i].x << "; y = " << drawOrder[i].y << "; z = " << drawOrder[i].z << endl << endl;
		glLoadIdentity();
		glBegin(GL_TRIANGLES);
			glColor3f(1.f, 0.f, 0.f);
			glVertex3f(drawOrder[i].x, drawOrder[i].y, drawOrder[i].z);
			glColor3f(0.f, 1.f, 0.f);
			glVertex3f(drawOrder[i + 1].x, drawOrder[i + 1].y, drawOrder[i + 1].z);
			glColor3f(0.f, 0.f, 1.f);
			glVertex3f(drawOrder[i + 2].x, drawOrder[i + 2].y, drawOrder[i + 2].z);
		glEnd();
	}
*/
	glm::mat4 frustum = glm::frustum(1, -1, -1, 1, -5 , 5); // left, right, bottom, top, near, far

	mat4 view = glm::lookAt(vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 1.0f)); // get the view matrix

	double time = glfwGetTime();

	// rotate triangle
	float move = sinf(time / 1000.0 * (2 * 3.14) / 5); // -1<->+1 every 5 seconds
	float angle = time / 1000.0 * 45;  // 45° per second
	glm::vec3 axis_z(0, 0, 1);
	glm::mat4 m_transform = glm::translate(glm::mat4(1.0f), glm::vec3(move, 0.0, 0.0))
		* glm::rotate(glm::mat4(1.0f), angle, axis_z);

	struct attributes triangle_attributes[] = {
		{ { 0.0, 0.8, 0.0 }, { 1.0, 1.0, 0.0 } },
		{ { -0.8, -0.8, 0.0 }, { 0.0, 0.0, 1.0 } },
		{ { 0.8, -0.8, 0.0 }, { 1.0, 0.0, 0.0 } }
	};
	
	glGenBuffers(1, &vbo_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_attributes), triangle_attributes, GL_STATIC_DRAW);


	glUseProgram(phongShader.programID);

	// glm::mat4 mvp = frustum * view * m_transform; // TODO: frustum is messing things up
	glm::mat4 mvp = view * m_transform;

	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

	glEnableVertexAttribArray(attribute_coord3d);
	glEnableVertexAttribArray(attribute_v_color);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glVertexAttribPointer(attribute_coord3d, 3, GL_FLOAT, GL_FALSE,  sizeof(struct attributes), 0);								// next coord2d appears every 6 floats: sizeof(struct attributes)
	glVertexAttribPointer(attribute_v_color, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),  (GLvoid*)(2 * sizeof(GLfloat)));		// next color appears every 5 floats,  offset of first element: (GLvoid*)(2 * sizeof(GLfloat)) 
	
	// Push each element in buffer_vertices to the vertex shader
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//clean up
	glDisableVertexAttribArray(attribute_v_color);
	glDisableVertexAttribArray(attribute_coord3d);
	glDeleteBuffers(1, &vbo_triangle);
	glDeleteBuffers(1, &vbo_triangle_colors);
}