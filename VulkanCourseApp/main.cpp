//#define GLM_FORCE_RADIANS
//#define GLM_FORCE_DEPTH_ZERO_TO_ONE
//#include <glm/mat4x4.hpp>

#define GLFW_INCLUDE_VULKAN
#include "VulkanRenderer.h"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>
#include <iostream>

GLFWwindow* window;
VulkanRenderer vulkanRenderer;

void initWindow(std::string wName= "Test Window", const int width = 1920, const int height = 1080)
{
	//Initialize GLFW
	glfwInit();

	//Set GLFW to not work with OpenGL
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, wName.c_str(), nullptr, nullptr);
}

int main()
{
	//Create Window
	initWindow("Test Window", 1920, 1080);

	//Create instance of Vulkan Renderer
	if (vulkanRenderer.init(window) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}
	
	//Loop Until Close
	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}

	//Destroys instance
	vulkanRenderer.cleanup();

	//Destroy Window and Stop GLFW
	glfwDestroyWindow(window);
	glfwTerminate();
	
	return 0;
}