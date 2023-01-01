#pragma once

#ifndef FDEFINES_H
#define FDEFINES_H

#define true 1
#define false 0

#define FMAX(x,y) (x > y ? x : y)
#define FMIN(x,y) (x > y ? y : x)

#define FCLAMP(value, min, max) (value <= min) ? min : (value >= max) ? max : value;

typedef _Bool bool;

typedef struct vector
{
	float x, y, z;
}vector;

#endif