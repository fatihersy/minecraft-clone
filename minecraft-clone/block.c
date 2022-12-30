#include "block.h"

#include "shader.h"

void create_outline(float x, float y, float z, sides side);
void create_grass(float x, float y, float z, sides side);
void create_dirt(float x, float y, float z, sides side);


void render_block(float x, float y, float z, sides side, uint8_t block_type)
{
    bind_vertex_array();

        if (block_type == 0) create_outline(x, y, z, side);
    else if (block_type == 1) create_dirt(x, y, z, side);
    else if (block_type == 2) create_grass(x, y, z, side);

}

void create_outline(float x, float y, float z, sides side)
{
    mat4 model = GLM_MAT4_IDENTITY_INIT;

    active_texture_5();
    if (!side.up)
    {
        glm_mat4_identity(model);
        glm_translate(model, (vec3) { x, y + 0.5001, z });
        glm_rotate(model, glm_rad(90), (vec3) { 90.0f, 0.0f, 0.0f });
        setMat4("model", model);
        draw();
    }

    active_texture_5();
    if (!side.down)
    {
        glm_mat4_identity(model);
        glm_translate(model, (vec3) { x, y - 0.5001, z });
        glm_rotate(model, glm_rad(90), (vec3) { 90.0f, 0.0f, 0.0f });
        setMat4("model", model);
        draw();
    }

    active_texture_5();
    if (!side.right)
    {
        glm_mat4_identity(model);
        glm_translate(model, (vec3) { x + 0.5001, y, z });
        glm_rotate(model, glm_rad(90), (vec3) { 0.0f, 90.f, 0.f });
        setMat4("model", model);
        draw();
    }

    active_texture_5();
    if (!side.left)
    {
        glm_mat4_identity(model);
        glm_translate(model, (vec3) { x - 0.5001, y, z });
        glm_rotate(model, glm_rad(90), (vec3) { 0.0f, 90.f, 0.f });
        setMat4("model", model);
        draw();
    }

    active_texture_5();
    if (!side.front)
    {
        // make sure to initialize matrix to identity matrix first
        // calculate the model matrix for each object and pass it to shader before drawing
        glm_mat4_identity(model);
        glm_translate(model, (vec3) { x, y, z + 0.5001 });
        //float angle = 20.0f * i;
        glm_rotate(model, glm_rad(0), (vec3) { 0.0f, 0.0f, 90.0f });
        setMat4("model", model);
        draw();
    }

    active_texture_5();
    if (!side.back)
    {
        // make sure to initialize matrix to identity matrix first
        // calculate the model matrix for each object and pass it to shader before drawing
        glm_mat4_identity(model);
        glm_translate(model, (vec3) { x, y, z - 0.5001 });
        //float angle = 20.0f * i;
        glm_rotate(model, glm_rad(0), (vec3) { 0.0f, 0.0f, 90.0f });
        setMat4("model", model);
        draw();
    }
}

void create_dirt(float x, float y, float z, sides side) 
{
    mat4 model = GLM_MAT4_IDENTITY_INIT;

    active_texture_3();
    if (!side.up)
    {
        glm_mat4_identity(model);
        glm_translate(model, (vec3) { x, y + 0.5, z });
        glm_rotate(model, glm_rad(90), (vec3) { 90.0f, 0.0f, 0.0f });
        setMat4("model", model);
        draw();
    }

    active_texture_3();
    if (!side.down)
    {
        glm_mat4_identity(model);
        glm_translate(model, (vec3) { x, y - 0.5, z });
        glm_rotate(model, glm_rad(90), (vec3) { 90.0f, 0.0f, 0.0f });
        setMat4("model", model);
        draw();
    }

    active_texture_3();
    if (!side.right)
    {
        glm_mat4_identity(model);
        glm_translate(model, (vec3) { x + 0.5, y, z });
        glm_rotate(model, glm_rad(90), (vec3) { 0.0f, 90.f, 0.f });
        setMat4("model", model);
        draw();
    }

    active_texture_3();
    if (!side.left)
    {
        glm_mat4_identity(model);
        glm_translate(model, (vec3) { x - 0.5, y, z });
        glm_rotate(model, glm_rad(90), (vec3) { 0.0f, 90.f, 0.f });
        setMat4("model", model);
        draw();
    }

    active_texture_3();
    if (!side.front)
    {
        // make sure to initialize matrix to identity matrix first
        // calculate the model matrix for each object and pass it to shader before drawing
        glm_mat4_identity(model);
        glm_translate(model, (vec3) { x, y, z + 0.5 });
        //float angle = 20.0f * i;
        glm_rotate(model, glm_rad(0), (vec3) { 0.0f, 0.0f, 90.0f });
        setMat4("model", model);
        draw();
    }

    active_texture_3();
    if (!side.back)
    {
        // make sure to initialize matrix to identity matrix first
        // calculate the model matrix for each object and pass it to shader before drawing
        glm_mat4_identity(model);
        glm_translate(model, (vec3) { x, y, z - 0.5 });
        //float angle = 20.0f * i;
        glm_rotate(model, glm_rad(0), (vec3) { 0.0f, 0.0f, 90.0f });
        setMat4("model", model);
        draw();
    }
}

void create_grass(float x, float y, float z, sides side)
{
    mat4 model = GLM_MAT4_IDENTITY_INIT;


    active_texture_1();
    if (!side.up)
    {
        glm_mat4_identity(model);
        glm_translate(model, (vec3) { x, y + 0.5, z });
        glm_rotate(model, glm_rad(90), (vec3) { 90.0f, 0.0f, 0.0f });
        setMat4("model", model);
        draw();
    }

    active_texture_3();
    if (!side.down)
    {
        glm_mat4_identity(model);
        glm_translate(model, (vec3) { x, y - 0.5, z });
        glm_rotate(model, glm_rad(90), (vec3) { 90.0f, 0.0f, 0.0f });
        setMat4("model", model);
        draw();
    }

    active_texture_2();
    if (!side.right)
    {
        glm_mat4_identity(model);
        glm_translate(model, (vec3) { x + 0.5, y, z });
        glm_rotate(model, glm_rad(90), (vec3) { 0.0f, 90.f, 0.f });
        setMat4("model", model);
        draw();
    }

    active_texture_2();
    if (!side.left)
    {
        glm_mat4_identity(model);
        glm_translate(model, (vec3) { x - 0.5, y, z });
        glm_rotate(model, glm_rad(90), (vec3) { 0.0f, 90.f, 0.f });
        setMat4("model", model);
        draw();
    }

    active_texture_2();
    if (!side.front)
    {
        // make sure to initialize matrix to identity matrix first
        // calculate the model matrix for each object and pass it to shader before drawing
        glm_mat4_identity(model);
        glm_translate(model, (vec3) { x, y, z + 0.5 });
        //float angle = 20.0f * i;
        glm_rotate(model, glm_rad(0), (vec3) { 0.0f, 0.0f, 90.0f });
        setMat4("model", model);
        draw();
    }

    active_texture_2();
    if (!side.back)
    {
        // make sure to initialize matrix to identity matrix first
        // calculate the model matrix for each object and pass it to shader before drawing
        glm_mat4_identity(model);
        glm_translate(model, (vec3) { x, y, z - 0.5 });
        //float angle = 20.0f * i;
        glm_rotate(model, glm_rad(0), (vec3) { 0.0f, 0.0f, 90.0f });
        setMat4("model", model);
        draw();
    }
}