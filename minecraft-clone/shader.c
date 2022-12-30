#include "shader.h"

#include "glad/glad.h"

#include "ftime.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "cube.h"

char* read_from_file(const char* fileName);
unsigned int load_texture(const char* fileName, bool is_png);

static unsigned int VBO, VAO, EBO, shader_program;

#define MAX_TEXTURE_COUNT 10

typedef struct texture {
    unsigned int ID;
}texture;

typedef struct image_path {
    const char* path;
    bool is_png;
}image_path;

texture textures[MAX_TEXTURE_COUNT] = {0};

image_path texture_path[] =
{
    {"D:/Workspace/grass_top.jpg", false},
    {"D:/Workspace/grass_side.jpg", false},
    {"D:/Workspace/grass_bottom.jpg", false},
    {"D:/Workspace/awesomeface.png", true},
    {"D:/Workspace/outline.png", true}
};

void load_resources() 
{
   unsigned int available_texture_count = sizeof(texture_path) / sizeof(image_path);
   for (size_t i = 0; i < available_texture_count; i++)
   {
        textures[i].ID = load_texture(texture_path[i].path, texture_path[i].is_png);
   }

   use_program();

   glUniform1i(glGetUniformLocation(shader_program, "texture1"), 0);
   glUniform1i(glGetUniformLocation(shader_program, "texture2"), 1);
   glUniform1i(glGetUniformLocation(shader_program, "texture3"), 2);
   glUniform1i(glGetUniformLocation(shader_program, "texture4"), 3);
   glUniform1i(glGetUniformLocation(shader_program, "texture5"), 4);
}

void update_shader() 
{
    for (size_t i = 0; i < MAX_TEXTURE_COUNT; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i].ID);
    }

    use_program();
}

void bind_data()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_quad), vertices_quad, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_quad), indices_quad, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}

void draw() 
{ 
    //glDrawArrays(GL_TRIANGLES, 0, 36); 

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
void bind_vertex_array() { glBindVertexArray(VAO); }

unsigned int create_program(const char* vertex_path, const char* fragment_path)
{
    const char* vertex_code   = read_from_file(vertex_path);
    const char* fragment_code = read_from_file(fragment_path);

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_code, 0);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, 0, infoLog);
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_code, 0);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, 0, infoLog);
    }
    // link shaders
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertexShader);
    glAttachShader(shader_program, fragmentShader);
    glLinkProgram(shader_program);
    // check for linking errors
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, 0, infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shader_program;
}

void delete_shader_program() 
{
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader_program);
}

void use_program() 
{
	glUseProgram(shader_program);
}

unsigned int get_program_ID()
{
    return shader_program;
}

unsigned int get_vertex_array() 
{
    return VAO;
}

char* read_from_file(const char* fileName)
{
    FILE* fp = NULL;
    long size = 0;
    char* shaderContent = 0;

    /* Read File to get size */
    fopen_s(&fp, fileName, "rb");
    if (fp == NULL) {
        return "";
    }
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp) + 1;
    fclose(fp);

    /* Read File for Content */
    fopen_s(&fp, fileName, "rb");
    shaderContent = memset(malloc(size), '\0', size);
    fread(shaderContent, 1, size - 1, fp);
    fclose(fp);

    return shaderContent;
}

unsigned int load_texture(const char* fileName, bool is_png) 
{
    // load and create a texture 
    // -------------------------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char* data = stbi_load(fileName, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, is_png ? GL_RGBA : GL_RGB, width, height, 0, is_png ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load texture\n");
    }
    stbi_image_free(data);

    return texture;
}

void setMat4(const char* name, mat4 mat) 
{
    glUniformMatrix4fv(glGetUniformLocation(shader_program, name), 1, GL_FALSE, mat[0]);
}

void active_texture_1()
{
    glUniform1i(glGetUniformLocation(shader_program, "texture_index"), 1);
}

void active_texture_2()
{
    glUniform1i(glGetUniformLocation(shader_program, "texture_index"), 2);
}

void active_texture_3()
{
    glUniform1i(glGetUniformLocation(shader_program, "texture_index"), 3);
}

void active_texture_4()
{
    glUniform1i(glGetUniformLocation(shader_program, "texture_index"), 4);
}
void active_texture_5() 
{
    glUniform1i(glGetUniformLocation(shader_program, "texture_index"), 5);
}