#include <iostream>
#include <stdexcept>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


GLFWwindow *initializeOpenGL()
{

	/* Initialize the library */
	if (!glfwInit())
		throw std::runtime_error("GLFW not initialized");

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow *window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		throw std::runtime_error("Window not created");
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		throw std::runtime_error("GLEW not initialized");

	std::cout << glGetString(GL_VERSION) << std::endl;

	return window;
}