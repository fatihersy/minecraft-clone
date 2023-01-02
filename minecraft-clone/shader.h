#pragma once

#include "cglm/call.h"

void load_resources();

void update_shader();

void bind_data();

unsigned int get_program_ID(const char* program);

void use_program();
void use_cubemap_program();
void use_skybox_program();

void draw();

void bind_vertex_array();

unsigned int get_vertex_array();

void setInt(const char* name, int value);
void setMat4(const char* name, mat4 mat);
void setVec3(const char* name, vec3 vec);

void active_texture_1();
void active_texture_2();
void active_texture_3();
void active_texture_4();
void active_texture_5();
