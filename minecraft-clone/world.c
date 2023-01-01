#include "world.h"

#include "camera.h"
#include "block.h"
#include "fmath.h"

// world space positions of our cubes
#define BOUND_X 100
#define BOUND_Y 100
#define BOUND_Z 100

#define MAX_WORLD_X 100
#define MAX_WORLD_Y 10
#define MAX_WORLD_Z 100

#define CHUNK 30

block grid[BOUND_X][BOUND_Y][BOUND_Z] = {0};

vector selected_block = {0};

void initialize_world() 
{
	unsigned int grid_count = 0;

	for (size_t x = 0; x < MAX_WORLD_X; x++)
	{
		for (size_t y = 0; y < MAX_WORLD_Y; y++)
		{
			for (size_t z = 0; z < MAX_WORLD_Z; z++)
			{
				grid
				[(x >= MAX_WORLD_X) ? BOUND_X : x]
				[(y >= MAX_WORLD_Y) ? BOUND_Y : y]
				[(z >= MAX_WORLD_Z) ? BOUND_Z : z].is_active = true;

				grid[x][y][z].block_type = (y + 1 == MAX_WORLD_Y) ? GRASS : DIRT;
			}
		}
	}
}

void update_world() 
{
	vector camera_pos = get_camera_position();
	vector front = get_camera_direction();

	int cam_x = FMAX(camera_pos.x - CHUNK, 0);
	int cam_y = FMAX(camera_pos.y - CHUNK, 0);
	int cam_z = FMAX(camera_pos.z - CHUNK, 0);

	int bound_x = FMIN(camera_pos.x + CHUNK, BOUND_X);
	int bound_y = FMIN(camera_pos.y + CHUNK, BOUND_Y);
	int bound_z = FMIN(camera_pos.z + CHUNK, BOUND_Z);

	for (int  x = cam_x; x < bound_x; x++)
	{
		for (int  y = cam_y; y < bound_y; y++)
		{
			for (int  z = cam_z; z < bound_z; z++)
			{
				if (grid[x][y][z].is_active)
				{
					sides side = (sides)
					{
						((y + 1) >= BOUND_Y) ? false : grid[x][y + 1][z].is_active,
						((y - 1) < 0)       ? false : grid[x][y - 1][z].is_active,
						((x + 1) >= BOUND_X) ? false :  grid[x + 1][y][z].is_active,
						((x - 1) < 0)       ?false  : grid[x - 1][y][z].is_active,
						((z + 1) >= BOUND_Z) ? false : grid[x][y][z + 1].is_active,
						((z - 1) < 0)       ? false : grid[x][y][z - 1].is_active,
					};

					render_block(x, y, z, side, grid[x][y][z].block_type);
				}
			}
		}
	}


	vector direction = camera_pos;
	for (unsigned int i = 0; i < 10; i++)
	{
		direction = add_vector(direction, front);
		int x = round_up(direction.x);
		int y = round_up(direction.y);
		int z = round_up(direction.z);

		if(grid[x][y][z].is_active)
		{
			render_block(x, y, z, (sides){false}, OUTLINE);
			selected_block = (vector){ x, y, z };
			break;
		}
	}
}

bool is_position_emtpy(int x, int y, int z)
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
