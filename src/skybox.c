#include "skybox.h"

#include "glad/glad.h"

#include "shader.h"
#include "camera.h"

void update_skybox(unsigned int skybox_VAO, unsigned int cubemap_texture, unsigned int cubeVAO)
{
    use_cubemap_program();

    glBindVertexArray(cubeVAO);
    glActiveTexture(GL_TEXTURE0 + cubemap_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    // draw skybox as last
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    use_skybox_program();

     //remove translation from the view matrix
    set_view_matrix();
    set_projection_matrix();

     //skybox cube
    glBindVertexArray(skybox_VAO);
    glActiveTexture(GL_TEXTURE0 + cubemap_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}