#include "ftime.h"

#include "GLFW/glfw3.h"

double get_time() 
{
	return glfwGetTime();
}