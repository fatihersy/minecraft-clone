#pragma once

#include <stdint.h>

#include "fdefines.h"

typedef enum block_type 
{
	OUTLINE = 0,
	DIRT = 1,
	GRASS = 2
};

typedef struct block
{
	bool is_active;
	uint8_t block_type;
} block;

typedef struct sides
{
	bool up;
	bool down;
	bool right;
	bool left;
	bool front;
	bool back;
}sides;

void render_block(float x, float y, float z, sides side, uint8_t block_type);
