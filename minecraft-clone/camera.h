#pragma once

#include "fdefines.h"

void initialize_camera();

float* get_matrix_ptr(const char* name);

vector get_camera_position();
vector get_camera_direction();

float get_window_ratio();

void set_camera_position(float x, float y, float z);
void set_view_matrix();
void set_projection_matrix();

void update_camera(float delta_time);

void rotate_camera(float pitch, float yaw);

void move_camera_front();
void move_camera_back();
void move_camera_left();
void move_camera_right();