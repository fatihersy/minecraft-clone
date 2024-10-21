#include "fmath.h"
#include <math.h>
#include <stdio.h>

vector add_vector(vector a, vector b) 
{
	float x = (a.x + b.x);
	float y = (a.y + b.y);
	float z = (a.z + b.z);
	return (vector)
	{
		(x < 0) ? 0 : x,
		(y < 0) ? 0 : y,
		(z < 0) ? 0 : z
	};
}

void print_vector(vector a) 
{
	printf("x: %f, y: %f, z: %f\n", a.x, a.y, a.z);
}

int round_up(float value) { return (int)round(value); }

int round_down(float value) { return (int)floor(value); }