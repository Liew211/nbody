#include <iostream>
#include <math.h>
#include <vector>

#include "bodies.h"


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
		{0.0, 0.0} };

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