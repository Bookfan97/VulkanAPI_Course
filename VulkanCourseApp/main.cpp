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

void initWindow(std::string wName = "Test Window", const int width = 1920, const int height = 1080)
{
	//Initialize GLFW
	glfwInit();

	//Set GLFW to not work with OpenGL
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, wName.c_str(), nullptr, nullptr);
}

bool VK_DEBUG;

int main()
{
	VK_DEBUG = false;
	//Create Window
	initWindow("Test Window", 1920, 1080);

	//Create instance of Vulkan Renderer
	if (vulkanRenderer.init(window) == EXIT_FAILURE)
	{
		return EXIT_FAILURE;
	}

	float angle = 0.0f;
	float deltaTime = 0.0f;
	float lastTime = 0.0f;

	// Loop until closed
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		float now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		angle += 10.0f * deltaTime;
		if (angle > 360.0f) { angle -= 360.0f; }

		glm::mat4 firstModel(1.0f);
		glm::mat4 secondModel(1.0f);

		firstModel = glm::translate(firstModel, glm::vec3(-2.0f, 0.0f, -5.0f));
		firstModel = glm::rotate(firstModel, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

		secondModel = glm::translate(secondModel, glm::vec3(2.0f, 0.0f, -5.0f));
		secondModel = glm::rotate(secondModel, glm::radians(-angle * 100), glm::vec3(0.0f, 0.0f, 1.0f));

		vulkanRenderer.updateModel(0, firstModel);
		vulkanRenderer.updateModel(1, secondModel);

		vulkanRenderer.draw();
	}

	//Destroys instance
	vulkanRenderer.cleanup();

	//Destroy Window and Stop GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}