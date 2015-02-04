#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>

using namespace glm;

//-----------------------
// function declarations 
//-----------------------
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void error_callback(int error, const char* description);

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
    
	// run untill Esc is pressed
    while (!glfwWindowShouldClose(window)) {
		
		double time = glfwGetTime();
		int width, height;

		//set up viewport
		glfwGetFramebufferSize(window, &width, %height);
		glViewport(0, 0, width, height);

		//draw here
		
	
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

static void key_callback (GLFWwindow* window, int key, int scancode, int action, int mods) {
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE); 
}

static void error_callback(int error, const char* description) {
	fputs(description, stderr);
}