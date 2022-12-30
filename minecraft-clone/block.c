#include "block.h"

#include "shader.h"

void render_block(float x, float y, float z, sides side, uint8_t texture_index)
{
    mat4 model = GLM_MAT4_IDENTITY_INIT;

    bind_vertex_array();

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