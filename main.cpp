#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>

using namespace glm;

int main(void) {
    // Initialise GLFW
    if (!glfwInit()) {
	fprintf( stderr, "Failed to initialize GLFW\n" );
	return -1;
    }
    
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // window not resizable
    
    // Open a window and create its OpenGL context 
    GLFWwindow* window = glfwCreateWindow(1024, 768, "Cloth simulation", NULL, NULL);
    
    if (!window) {
	glfwTerminate();
	exit(EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent(window); // Initialize GLEW 
    glewExperimental=true;
    
    if (glewInit() != GLEW_OK) {
	fprintf(stderr, "Failed to initialize GLEW\n");
	return -1;
    }

    glfwSetKeyCallback(window, key_callback); //set key callback for window
    
    while (!glfwWindowShouldClose(window)) {
	double time = glfwGetTime();
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
