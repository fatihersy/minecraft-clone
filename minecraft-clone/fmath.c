#include "fmath.h"

void initialize_perspective() 
{

}

unsigned int get_value_ptr()
{

}


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