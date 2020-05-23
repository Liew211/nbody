#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdexcept>
#include <math.h>
#include <vector>

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

/* Distance formula in 2 dimensions */
double distance(double p1[2], double p2[2])
{
	return sqrt(pow((p2[1] - p1[1]), 2) + pow((p2[0] - p1[0]), 2));
}

void updatePositions(double positions[], double velocities[])
{
	/* Find distances between each body */

	double distance_ab = distance(positions, positions + 2);
	double distance_ac = distance(positions, positions + 4);
	double distance_bc = distance(positions + 2, positions + 4);

	// Display center of mass of system
	// std::cout << (positions[0] + positions[2] + positions[4]) << " "
	// 		  << (positions[1] + positions[3] + positions[5]) << std::endl;

	/* Calculate accelerations */

	/* a_x = K * (cos(theta_b) / r_b ^ 2 + cos(theta_c) / r_c ^ 2 )
	   a_y = K * (sin(theta_b) / r_b ^ 2 + sin(theta_c) / r_c ^ 2 ) */

	std::vector<double> accelerations[3] = {
		{0.0, 0.0},
		{0.0, 0.0},
		{0.0, 0.0}};

	const double K = 0.000001;

	accelerations[0][0] = K * ((positions[2] - positions[0]) / pow(distance_ab, 3) + (positions[4] - positions[0]) / pow(distance_ac, 3));
	accelerations[0][1] = K * ((positions[3] - positions[1]) / pow(distance_ab, 3) + (positions[5] - positions[1]) / pow(distance_ac, 3));

	accelerations[1][0] = K * ((positions[0] - positions[2]) / pow(distance_ab, 3) + (positions[4] - positions[2]) / pow(distance_bc, 3));
	accelerations[1][1] = K * ((positions[1] - positions[3]) / pow(distance_ab, 3) + (positions[5] - positions[3]) / pow(distance_bc, 3));

	accelerations[2][0] = K * ((positions[0] - positions[4]) / pow(distance_ac, 3) + (positions[2] - positions[4]) / pow(distance_bc, 3));
	accelerations[2][1] = K * ((positions[1] - positions[5]) / pow(distance_ac, 3) + (positions[3] - positions[5]) / pow(distance_bc, 3));

	positions[0] += velocities[0];
	positions[1] += velocities[1];
	
	positions[2] += velocities[2];
	positions[3] += velocities[3];
	
	positions[4] += velocities[4];
	positions[5] += velocities[5];

	velocities[0] += accelerations[0][0];
	velocities[1] += accelerations[0][1];

	velocities[2] += accelerations[1][0];
	velocities[3] += accelerations[1][1];

	velocities[4] += accelerations[2][0];
	velocities[5] += accelerations[2][1];
}

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