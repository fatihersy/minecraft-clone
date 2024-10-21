#include "shader.h"
#include "fdefines.h"

#include "glad/glad.h"
#include "cglm/call.h"
#include "ftime.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "skybox.h"
#include "geometry.h"

unsigned int create_program(const char* vertex_path, const char* fragment_path);
char* read_from_file(const char* fileName);
unsigned int load_texture(const char* fileName, b8 is_png);
unsigned int load_skybox();

static unsigned int cubemap_texture, skybox_VAO, block_VAO, cubeVAO;
static unsigned int main_program;
static unsigned int skybox_program;
static unsigned int cubemap_program;
static unsigned int current_program;

#define MAX_TEXTURE_COUNT 10

typedef struct texture {
    unsigned int ID;
}texture;

typedef struct image_path {
    const char* path;
    b8 is_png;
}image_path;

texture textures[MAX_TEXTURE_COUNT] = {0};

image_path texture_path[] =
{
    {"D:/Workspace/CLang/Resources/grass_top.jpg", false},
    {"D:/Workspace/CLang/Resources/grass_side.jpg", false},
    {"D:/Workspace/CLang/Resources/grass_bottom.jpg", false},
    {"D:/Workspace/CLang/Resources/awesomeface.png", true},
    {"D:/Workspace/CLang/Resources/utline.png", true}
};

image_path skybox_faces[] =
{
    {"D:/Workspace/CLang/Resources/skybox/right.jpg", false},
    {"D:/Workspace/CLang/Resources/skybox/left.jpg", false},
    {"D:/Workspace/CLang/Resources/skybox/top.jpg", false},
    {"D:/Workspace/CLang/Resources/skybox/bottom.jpg", false},
    {"D:/Workspace/CLang/Resources/skybox/front.jpg", false},
    {"D:/Workspace/CLang/Resources/skybox/back.jpg", false}
};

void load_resources() 
{
    main_program = create_program("D:/Workspace/CLang/Resources/camera.vs", "D:/Workspace/CLang/Resources/camera.fs");
    skybox_program = create_program("D:/Workspace/CLang/Resources/skybox.vs", "D:/Workspace/CLang/Resources/skybox.fs");
    cubemap_program = create_program("D:/Workspace/CLang/Resources/cubemaps.vs", "D:/Workspace/CLang/Resources/cubemaps.fs");

   unsigned int available_texture_count = sizeof(texture_path) / sizeof(image_path);
   for (size_t i = 0; i < available_texture_count; i++)
   {
        textures[i].ID = load_texture(texture_path[i].path, texture_path[i].is_png);
   }

   use_program();

   glUniform1i(glGetUniformLocation(main_program, "texture1"), 0);
   glUniform1i(glGetUniformLocation(main_program, "texture2"), 1);
   glUniform1i(glGetUniformLocation(main_program, "texture3"), 2);
   glUniform1i(glGetUniformLocation(main_program, "texture4"), 3);
   glUniform1i(glGetUniformLocation(main_program, "texture5"), 4);

   cubemap_texture = load_skybox();

   use_cubemap_program();
   setInt("skybox", 6);

   use_skybox_program();
   glUniform1i(glGetUniformLocation(skybox_program, "skybox"), 6);

}

void update_shader() 
{
    for (size_t i = 0; i < MAX_TEXTURE_COUNT; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i].ID);
    }

    use_program();

    update_skybox(skybox_VAO, cubemap_texture, cubeVAO);
}

void draw()
{
    //glDrawArrays(GL_TRIANGLES, 0, 36); 
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);
}
void bind_vertex_array()
{
    use_program();
    glBindVertexArray(block_VAO);
}

void bind_data()
{
    use_program();

    unsigned int block_VBO, block_EBO;
    glGenVertexArrays(1, &block_VAO);
    glGenBuffers(1, &block_VBO);
    glGenBuffers(1, &block_EBO);

    glBindVertexArray(block_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, block_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_quad), vertices_quad, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, block_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_quad), indices_quad, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a block_vao is active as the bound element buffer object IS stored in the block_vao; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the block_vao afterwards so other block_vao calls won't accidentally modify this block_vao, but this rarely happens. Modifying other
    // block_vaos requires a call to glBindVertexArray anyways so we generally don't unbind block_vaos (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // cube VAO
    unsigned int cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

     //skybox block_vao
    unsigned int skyboxVBO;
    glGenVertexArrays(1, &skybox_VAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skybox_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}


unsigned int create_program(const char* vertex_path, const char* fragment_path)
{
    const char* vertex_code   = read_from_file(vertex_path);
    const char* fragment_code = read_from_file(fragment_path);

    unsigned int program_ID;
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
    program_ID = glCreateProgram();
    glAttachShader(program_ID, vertexShader);
    glAttachShader(program_ID, fragmentShader);
    glLinkProgram(program_ID);
    // check for linking errors
    glGetProgramiv(program_ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program_ID, 512, 0, infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program_ID;
}

void delete_main_program() 
{
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &block_VAO);
    glDeleteProgram(main_program);
    glDeleteVertexArrays(1, &skybox_VAO);
    glDeleteProgram(skybox_program);
}

void use_program() 
{
	glUseProgram(main_program);
    current_program = main_program;
}

void use_cubemap_program()
{
    glUseProgram(cubemap_program);
    current_program = cubemap_program;
}

void use_skybox_program()
{
    glUseProgram(skybox_program);
    current_program = skybox_program;
}

unsigned int get_program_ID(const char* program)
{
    if (strcmp(program, "skybox") == 0) return skybox_program;
    if (strcmp(program, "main") == 0) return main_program;

    return 0;
}

unsigned int get_vertex_array() 
{
    return block_VAO;
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

unsigned int load_texture(const char* fileName, b8 is_png) 
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

unsigned int load_skybox()
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;

    unsigned int available_texture_count = sizeof(skybox_faces) / sizeof(image_path);
    for (unsigned int i = 0; i < available_texture_count; i++)
    {
        unsigned char* data = stbi_load(skybox_faces[i].path, &width, &height, &nrComponents, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            printf("Cubemap texture failed to load at path: %s\n", skybox_faces[i].path);
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return textureID;
}
// ------------------------------------------------------------------------
void setInt(const char* name, int value)
{
    glUniform1i(glGetUniformLocation(current_program, name), value);
}

void setMat4(const char* name, mat4 mat) 
{
    glUniformMatrix4fv(glGetUniformLocation(current_program, name), 1, GL_FALSE, mat[0]);
}
void setVec3(const char* name, vec3 vec)
{
    glUniform3fv(glGetUniformLocation(current_program, name), 1, &vec[0]);
}

void active_texture_1()
{
    glUniform1i(glGetUniformLocation(main_program, "texture_index"), 1);
}

void active_texture_2()
{
    glUniform1i(glGetUniformLocation(main_program, "texture_index"), 2);
}

void active_texture_3()
{
    glUniform1i(glGetUniformLocation(main_program, "texture_index"), 3);
}

void active_texture_4()
{
    glUniform1i(glGetUniformLocation(main_program, "texture_index"), 4);
}
void active_texture_5() 
{
    glUniform1i(glGetUniformLocation(main_program, "texture_index"), 5);
}