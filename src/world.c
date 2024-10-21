#include "world.h"

#include "camera.h"
#include "block.h"
#include "fmath.h"
#include "perlin.h"
#include <stdio.h>

// world space positions of our cubes
#define BOUND_X 256
#define BOUND_Y 20
#define BOUND_Z 256

#define MAX_WORLD_X 256
#define MAX_WORLD_Y 16
#define MAX_WORLD_Z 256

#define CHUNK 45

block grid[BOUND_X][BOUND_Y][BOUND_Z] = {0};

vector selected_block = {0};

float window_ratio;

void initialize_world() 
{
	unsigned int grid_count = 0;

	for (size_t x = 0; x < MAX_WORLD_X; x++)
	{
		for (size_t z = 0; z < MAX_WORLD_Z; z++)
		{
			int max_y = round_up(perlin2d(x, z, 0.1f, 4.0f) * 10);

			for (size_t y = 0; y < max_y; y++)
			{

				grid[x][y][z].is_active = true;

				grid[x][y][z].block_type = (y + 1 == max_y) ? GRASS : DIRT;
			}
		}
	}

	window_ratio = get_window_ratio();
}

void update_world() 
{
	vector camera_pos = get_camera_position();
	vector front = get_camera_direction();

	int chunk_x = (front.x < 0) ? -CHUNK : CHUNK;
	int chunk_z = (front.z < 0) ? -CHUNK : CHUNK;

	int near_x = FMAX(camera_pos.x + -chunk_x, 0);
	int near_y = FMAX(camera_pos.y - 15, 0);
	int near_z = FMAX(camera_pos.z + -chunk_z, 0);

	int far_x = FMIN(camera_pos.x + chunk_x, BOUND_X);
	int far_y = FMIN(camera_pos.y + CHUNK, MAX_WORLD_Y);
	int far_z = FMIN(camera_pos.z + chunk_z, BOUND_Z);

	int max_x = FMAX(far_x, near_x);
	int min_x = FMIN(far_x, near_x);
	int max_z = FMAX(far_z, near_z);
	int min_z = FMIN(far_z, near_z);
	
	max_x = FCLAMP(max_x, 0, 256);
	min_x = FCLAMP(min_x, 0, 256);
	max_z = FCLAMP(max_z, 0, 256);
	min_z = FCLAMP(min_z, 0, 256);

	for (int x = min_x; x < max_x; x++)
	{
		for (int z = min_z; z < max_z; z++)
		{
			for (int y = near_y; y < far_y; y++)
			{
				if (grid[x][y][z].is_active)
				{
					sides side = (sides)
					{
						((y + 1) >= BOUND_Y) ? false : grid[x][y + 1][z].is_active,
						((y - 1) < 0) ? false : grid[x][y - 1][z].is_active,
						((x + 1) >= BOUND_X) ? false : grid[x + 1][y][z].is_active,
						((x - 1) < 0) ? false : grid[x - 1][y][z].is_active,
						((z + 1) >= BOUND_Z) ? false : grid[x][y][z + 1].is_active,
						((z - 1) < 0) ? false : grid[x][y][z - 1].is_active,
					};

					render_block(x, y, z, side, grid[x][y][z].block_type);
				}
			}
		}
	}


	vector direction = camera_pos;
	for (unsigned int i = 0; i < 15; i++)
	{
		direction = add_vector(direction, front);

		int x = round_down(direction.x);
		int y = round_down(direction.y);
		int z = round_down(direction.z);

		if (x > MAX_WORLD_X) break;
		if (y > MAX_WORLD_Y) break;
		if (z > MAX_WORLD_Z) break;

		if(grid[x][y][z].is_active)
		{
			render_block(x, y, z, (sides){false}, OUTLINE);
			selected_block = (vector){ x, y, z };
			break;
		}
	}
}

b8 is_position_emtpy(int x, int y, int z)
{
	return grid[x][y][z].is_active;
}

void put_block() 
{
	int x = (int)selected_block.x;
	int y = (int)selected_block.y;
	int z = (int)selected_block.z;

	grid[x][y+1][z].is_active = true;
	grid[x][y+1][z].block_type = DIRT;
}

void break_block()
{
	grid[(int)selected_block.x][(int)selected_block.y][(int)selected_block.z].is_active = false ;
}

/*

	for (int x = cam_x; x < bound_x; x++)
	{
		for (int y = cam_y; y < bound_y; y++)
		{
			for (int z = cam_z; z < bound_z; z++)
			{
				if (grid[x][y][z].is_active)
				{
					sides side = (sides)
					{
						((y + 1) >= BOUND_Y) ? false : grid[x][y + 1][z].is_active,
						((y - 1) < 0) ? false : grid[x][y - 1][z].is_active,
						((x + 1) >= BOUND_X) ? false : grid[x + 1][y][z].is_active,
						((x - 1) < 0) ? false : grid[x - 1][y][z].is_active,
						((z + 1) >= BOUND_Z) ? false : grid[x][y][z + 1].is_active,
						((z - 1) < 0) ? false : grid[x][y][z - 1].is_active,
					};

					render_block(x, y, z, side, grid[x][y][z].block_type);
				}
			}
		}
	}


*/