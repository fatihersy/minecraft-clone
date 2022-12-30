#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "cglm/call.h"

unsigned int create_program(const char* vertex_path, const char* fragment_path);

void load_resources();

void update_shader();

void bind_data();

unsigned int get_program_ID();

void use_program();

void draw();

void bind_vertex_array();

unsigned int get_vertex_array();

void delete_shader_program();

void setMat4(const char* name, mat4 mat);

void active_texture_1();
void active_texture_2();
void active_texture_3();
void active_texture_4();
