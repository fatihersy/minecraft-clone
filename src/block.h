#pragma once

#include <stdint.h>

#include "fdefines.h"

typedef enum block_type 
{
	OUTLINE = 0,
	DIRT = 1,
	GRASS = 2
} block_type;

typedef struct block
{
	b8 is_active;
	uint8_t block_type;
} block;

typedef struct sides
{
	b8 up;
	b8 down;
	b8 right;
	b8 left;
	b8 front;
	b8 back;
}sides;

void render_block(float x, float y, float z, sides side, uint8_t block_type);
