#include "camera.h"

#include "graphic.h"
#include "collision.h"
#include "shader.h"
#include "ftime.h"
#include "string.h"

#include "cglm/cam.h"
#include "cglm/vec3.h"

mat4 model = GLM_MAT4_IDENTITY_INIT;
mat4 view = GLM_MAT4_IDENTITY_INIT;
mat4 projection = GLM_MAT4_IDENTITY_INIT;

vec3 cameraPos   = { 128.0f, 10.0f, 128.0f };
vec3 cameraFront = { 1.0f, 0.0f, 1.0f };
vec3 cameraUp    = { 0.0f, 1.0f, 0.0f };

float player_width = 1.f;
float player_height = 1.8f;
float player_thickness = 0.3f;

vector collision_box = {0};

float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float fov = 60.0f;

float camera_speed = 10.f; // adjust accordingly

void initialize_camera() 
{
	float width  = (float) get_window_width();
	float height = (float) get_window_height();

	collision_box = (vector){ player_thickness, player_height, player_width };

	// pass projection matrix to shader (note that in this case it could change every frame)
	glm_perspective(glm_rad(fov), width / height, 0.1f, 100.0f, projection);
	setMat4("projection", projection);
}

float* get_matrix_ptr(const char* name)
{
	if (strcmp(name, "view") == 0) return view[0];
	if (strcmp(name, "projection") == 0) return projection[0];

	return 0;
}

void update_camera(float delta_time) 
{
	camera_speed = 15.f * delta_time;

	vec3 gravity = { 0.f, 0.0f, 0.f };

	if (!check_is_collide(
		(vector) { cameraPos[0] + collision_box.x, cameraPos[1] - collision_box.y, cameraPos[2] + collision_box.z},
		(vector) {gravity[0], gravity[1], gravity[2] },
		collision_box
	))
	{
		glm_vec3_add(cameraPos, gravity, cameraPos);
	}

	use_program();

	glm_mat4_identity(model);
	glm_perspective(glm_rad(fov), (float)get_window_width() / (float)get_window_height(), 0.1f, 100.0f, projection);
	setMat4("model", model);
	setMat4("view", view);
	setMat4("projection", projection);
	setVec3("cameraPos", cameraPos);

	// camera/view transformation
	glm_mat4_identity(view); // make sure to initialize matrix to identity matrix first
	float radius = 10.0f;
	float camX = (float)(sin(get_time()) * radius);
	float camZ = (float)(cos(get_time()) * radius);

	vec3 center;
	glm_vec3_add(cameraPos, cameraFront, center);
	glm_lookat(cameraPos, center, cameraUp, view);
	setMat4("view", view);
	setMat4("projection", projection);
}

void rotate_camera(float xoffset, float yoffset)
{
	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	vec3 front = 
	{
		(cos(glm_rad(yaw)) * cos(glm_rad(pitch))),
		sin(glm_rad(pitch)),
		sin(glm_rad(yaw))* cos(glm_rad(pitch))
	};

	glm_normalize_to(front, cameraFront);
}

void move_camera_front() 
{
	vector camera_pos = { cameraPos[0], cameraPos[1], cameraPos[2] };

	vector direction = { 
		cameraFront[0],
		0.f,
		cameraFront[2]
	};

	if (!check_is_collide(camera_pos,direction, collision_box)) 
	{
		glm_vec3_muladd(cameraFront, (vec3) {camera_speed, 0.f, camera_speed}, cameraPos);
	}
}
void move_camera_back()
{
	vector camera_pos = { cameraPos[0], cameraPos[1], cameraPos[2] };
	vector direction = { 
		-cameraFront[0], 
		0.f,
		-cameraFront[2]
	};

	if (!check_is_collide(camera_pos, direction, collision_box)) 
	{
		glm_vec3_muladd(cameraFront, (vec3) { -camera_speed, 0.f, -camera_speed }, cameraPos);
	}
}
void move_camera_left()
{
	vec3 cross = GLM_VEC3_ONE_INIT;
	vector camera_pos = { cameraPos[0], cameraPos[1], cameraPos[2] };

	glm_cross(cameraFront, cameraUp, cross);
	glm_normalize(cross);
	glm_vec3_scale(cross, camera_speed, cross);

	vector direction = { 
		-(cross[0] + (collision_box.x * cross[0])),
		-(cross[1] + (collision_box.y * cross[1])),
		-(cross[2] + (collision_box.z * cross[2]))
	};


	if (!check_is_collide(camera_pos, direction, collision_box)) 
	{
		glm_vec3_sub(cameraPos, cross, cameraPos);
	}
}
void move_camera_right()
{
	vec3 cross = GLM_VEC3_ONE_INIT;
	vector camera_pos = { cameraPos[0], cameraPos[1], cameraPos[2] };

	glm_cross(cameraFront, cameraUp, cross);
	glm_normalize(cross);
	glm_vec3_scale(cross, camera_speed, cross);

	vector direction = { 
		cross[0] + (collision_box.x * cross[0]),
		cross[1] + (collision_box.y * cross[1]),
		cross[2] + (collision_box.z * cross[2])
	};
	
	if (!check_is_collide(camera_pos, direction, collision_box)) 
	{
		glm_vec3_add(cameraPos, cross, cameraPos);
	}
}

vector get_camera_position()
{
	return (vector) { cameraPos[0], cameraPos[1], cameraPos[2] };
}

vector get_camera_direction()
{
	return (vector) { cameraFront[0], cameraFront[1], cameraFront[2] };
}

float get_window_ratio() 
{
	return (float)get_window_width() / get_window_height();
}

void set_camera_position(float x, float y, float z)
{
	cameraPos[0] = x;
	cameraPos[1] = y;
	cameraPos[2] = z;
}

void set_view_matrix() 
{
	setMat4("view", view);
}
void set_projection_matrix() 
{
	setMat4("projection", projection);
}