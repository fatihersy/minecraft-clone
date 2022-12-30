#pragma once

#ifndef FMATH_H
#define FMATH_H

typedef struct vector
{
	float x, y, z;
}vector;

void initialize_perspective();

unsigned int get_value_ptr();

vector add_vector(vector a, vector b);

#endif // !FMATH_H


