#include "camera.h"

#include "graphic.h"

#include "shader.h"

#include "cglm/cam.h"
#include "cglm/vec3.h"

mat4 model = GLM_MAT4_IDENTITY_INIT;
mat4 view = GLM_MAT4_IDENTITY_INIT;
mat4 projection = GLM_MAT4_IDENTITY_INIT;

vec3 cameraPos   = { 50.0f, 13.0f, 50.0f };
vec3 cameraFront = { 0.0f, 0.0f, -1.0f };
vec3 cameraUp    = { 0.0f, 1.0f, 0.0f };

float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float fov = 90.0f;

float camera_speed = 10.f; // adjust accordingly

void initialize_camera() 
{
	float width  = (float) get_window_width();
	float height = (float) get_window_height();

	glm_perspective(glm_rad(45.0f), width / height, 0.1f, 100.0f, projection);
	setMat4("projection", projection);
}

float* get_matrix_ptr(const char* name)
{
	if (name == "view") return view[0];
	if (name == "projection") return projection[0];

	return 0;
}

void update_camera(float delta_time) 
{
	camera_speed = 15.f * delta_time;

	// camera/view transformation
	glm_mat4_identity(view); // make sure to initialize matrix to identity matrix first
	float radius = 10.0f;
	float camX = (float)(sin(glfwGetTime()) * radius);
	float camZ = (float)(cos(glfwGetTime()) * radius);

	vec3 center;
	glm_vec_add(cameraPos, cameraFront, center);
	glm_lookat(cameraPos, center, cameraUp, view);
	setMat4("view", view);
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
	glm_vec_muladds(cameraFront, camera_speed, cameraPos);
}
void move_camera_back()
{
	vec3 scale = GLM_VEC3_ONE_INIT;
	glm_vec_scale(cameraFront, camera_speed, scale);
	glm_vec_sub(cameraPos, scale, cameraPos);
}
void move_camera_left()
{
	vec3 cross = GLM_VEC3_ONE_INIT;
	glm_cross(cameraFront, cameraUp, cross);
	glm_normalize(cross);
	glm_vec_scale(cross, camera_speed, cross);
	glm_vec_sub(cameraPos, cross, cameraPos);
}
void move_camera_right()
{
	vec3 cross = GLM_VEC3_ONE_INIT;
	glm_cross(cameraFront, cameraUp, cross);
	glm_normalize(cross);
	glm_vec_scale(cross, camera_speed, cross);
	glm_vec_add(cameraPos, cross, cameraPos);
}

vector get_camera_position()
{
	return (vector)
	{
		cameraPos[0], cameraPos[1], cameraPos[2]
	};
}

vector get_camera_direction()
{
	return (vector)
	{
		cameraFront[0], cameraFront[1], cameraFront[2]
	};
}