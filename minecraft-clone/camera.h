#pragma once

#include "fmath.h"

void initialize_camera();

float* get_matrix_ptr(const char* name);

vector get_camera_position();
vector get_camera_direction();

void update_camera(float delta_time);

void rotate_camera(float pitch, float yaw);

void move_camera_front();
void move_camera_back();
void move_camera_left();
void move_camera_right();