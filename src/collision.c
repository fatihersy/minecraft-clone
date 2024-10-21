#include "collision.h"

#include <math.h>

#include "world.h"
#include "fmath.h"

b8 check_is_collide(vector position, vector direction, vector collision_box)
{
	direction = add_vector(position, direction);
	int x = round_up(direction.x);
	int y = round_up(direction.y);
	int z = round_up(direction.z);

	return is_position_emtpy(x, y, z);
}