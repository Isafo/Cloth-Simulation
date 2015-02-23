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
GLuint ibo_cloth_elements;
GLuint vbo_cloth_vertices, vbo_cloth_colors;

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
		/*cout << " x: " << particles[1].x << " y: " << particles[1].y << " z: " << particles[1].z << endl;
		cout << " x: " << particles[2].x << " y: " << particles[2].y << " z: " << particles[2].z << endl;
		cout << " x: " << particles[3].x << " y: " << particles[3].y << " z: " << particles[3].z << endl << endl;// */
		
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
	
	glm::mat4 frustum = glm::frustum(-1, 1, -1, 1, -1 , 1); // left, right, bottom, top, near, far

	mat4 view = glm::lookAt(vec3(0.0f, 0.0f, -3.00001f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 1.0f)); // get the view matrix

	double time = glfwGetTime();

	vector<glm::vec3> drawOrder = MakeTriangles(particles); // orders input so that the normal points in the correct direction

	GLfloat clothVertices[1000]; // contains the cloth vertes coordiates
	GLushort clothElements[1000]; // contains the order the cloth elements should be drawn in
	GLfloat clothColors[1000]; // contains the vertices colors

	// insert the coordinates in the array and generate the draw order
	for (int i = 0, j = 0; i + 2 < drawOrder.size(); i = i + 3, j + 9) {
		clothVertices[j] = (drawOrder[i].x);
		clothVertices[j + 1] = (drawOrder[i].y);
		clothVertices[j + 2] = (drawOrder[i].z);

		clothVertices[j + 3] = (drawOrder[i + 1].x);
		clothVertices[j + 4] = (drawOrder[i + 1].y);
		clothVertices[j + 5] = (drawOrder[i + 1].z);

		clothVertices[j + 6] = (drawOrder[i + 2].x);
		clothVertices[j + 7] = (drawOrder[i + 2].y);
		clothVertices[j + 8] = (drawOrder[i + 2].z);

		clothElements[i] = i;
		clothElements[i + 1] = i + 1;
		clothElements[i + 2] = i + 2;

		clothColors[i] = 0.5;
		clothColors[i + 1] = 0.0;
		clothColors[i + 2] = 1.0;
	}

	// geerate and bind buffer for clothVertices
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
	glm::mat4 mvp = frustum * view; // TODO: frustum is messing things up
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
	int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size); // use glGetBufferParameteriv to get the buffer size
	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

	//clean up
	glDisableVertexAttribArray(attribute_coord3d);
	glDisableVertexAttribArray(attribute_v_color);
	glDeleteBuffers(1, &ibo_cloth_elements);

}