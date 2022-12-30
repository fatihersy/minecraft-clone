#include "world.h"

#include "camera.h"
#include "block.h"

// world space positions of our cubes
#define BOUND_X 100
#define BOUND_Y 100
#define BOUND_Z 100

#define MAX_WORLD_X 100
#define MAX_WORLD_Y 10
#define MAX_WORLD_Z 100

#define CHUNK 30

block grid[BOUND_X][BOUND_Y][BOUND_Z] = {0};

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
			}
		}
	}
}

void update_world() 
{
	vector camera_pos = get_camera_position();
	vector front = get_camera_direction();

	int cam_x = ((camera_pos.x - CHUNK) < 0) ? 0 : ( camera_pos.x - CHUNK) ;
	int cam_y = ((camera_pos.y - CHUNK) < 0) ? 0 : ( camera_pos.y - CHUNK) ;
	int cam_z = ((camera_pos.z - CHUNK) < 0) ? 0 : ( camera_pos.z - CHUNK) ;

	int bound_x = ((camera_pos.x + CHUNK) > BOUND_X) ? BOUND_X : ( camera_pos.x + CHUNK) ;
	int bound_y = ((camera_pos.y + CHUNK) > BOUND_Y) ? BOUND_Y : ( camera_pos.y + CHUNK) ;
	int bound_z = ((camera_pos.z + CHUNK) > BOUND_Z) ? BOUND_Z : ( camera_pos.z + CHUNK) ;

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
						((y - 1) < 0)      ? false  : grid[x][y - 1][z].is_active,
						((x + 1) >= BOUND_X) ? false:  grid[x + 1][y][z].is_active,
						((x - 1) < 0)       ?false  : grid[x - 1][y][z].is_active,
						((z + 1) >= BOUND_Z) ? false : grid[x][y][z + 1].is_active,
						((z - 1) < 0)      ? false  : grid[x][y][z - 1].is_active,
					};

					render_block(x,y,z,side, (z % 2) + 1);
				}
			}
		}
	}


	vector direction = camera_pos;
	for (unsigned int i = 0; i < 30; i++)
	{
		direction = add_vector(direction, front);
		grid
			[(int)direction.x]
			[(int)direction.y]
			[(int)direction.z].is_active = false;
	}
}
