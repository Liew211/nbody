#include <iostream>
#include <fstream>
#include <stdexcept>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "window.h"
#include "bodies.h"



int main(int argc, char *argv[])
{
	for (int i = 0; i < argc; ++i)
		std::cout << argv[i] << "\n";

	// Number of bodies
	const int n = 3; // std::strtol(argv[1], NULL, 10);

	GLFWwindow *window;

	try
	{
		window = initializeOpenGL();
	}
	catch (std::runtime_error &e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}

	std::ifstream bodies{"initialConditions.txt"};

	double *positions = new double[2 * n];
	double *velocities = new double[2 * n];


	for (int i = 0; i < 2 * n; i++)
	{
		if (!(bodies >> positions[i]))
		{
			return -1;
		}
	}


	for (int i = 0; i < 2 * n; i++)
	{
		if (!(bodies >> velocities[i]))
		{
			return -1;
		}
	}

	unsigned int buffer;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(double), positions, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(double), 0);

		glPointSize(6);
		glDrawArrays(GL_POINTS, 0, 3);

		updatePositions(positions, velocities);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}