#pragma once
#include <stdbool.h>
#include <stdint.h>


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

void render_block(float x, float y, float z, sides side, uint8_t texture_index);

