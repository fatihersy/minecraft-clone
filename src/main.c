#include "GLFW/glfw3.h"
#include "graphic.h"
#include "camera.h"
#include "world.h"
#include <stdlib.h>

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

b8 firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;

float delta_time = 0.0f;	// Time between current frame and last frame
float last_frame = 0.0f;

b8 polygon_mode = false;

float key_time = 0.f;

int main(void)
{
    create_window("Learn OpenGL", 1280, 720, mouse_callback, mouse_button_callback);
    
    initialize_camera();

    initialize_world();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(get_window_instance()))
    {
        //system("cls");
        double current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        // input
        // -----
        processInput(get_window_instance());

        graphics_begin_render();

        update_world();

        update_camera(delta_time);

        graphics_update_render();

        graphics_end_render();
    }


    terminate_window();

    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{

    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            move_camera_front();
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            move_camera_back();
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            move_camera_left();
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            move_camera_right();
    }
    {
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            glfwWaitEventsTimeout(0.7);
            if (polygon_mode)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                polygon_mode = false;
            }
            else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                polygon_mode = true;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
            set_camera_position(1, 16, 1);
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) 
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        break_block();
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        put_block();
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = (float)(xposIn);
    float ypos = (float)(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    rotate_camera(xoffset, yoffset);
}
