#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Utilities.h"
#include <stdexcept>
#include <iostream>
#include <vector>
class VulkanRenderer
{
public:
	VulkanRenderer();
	int init(GLFWwindow* newWindow);
	void cleanup();
	~VulkanRenderer();
private:
	GLFWwindow* window;

	//Components
	VkInstance instance;

	struct my_struct
	{
		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;
	} mainDevice;

	VkQueue graphicsQueue;
	
	//----------------------Vulkan Functions------------------------

	//Create Functions
	void createInstance();
	void createLogicalDevice();

	//Get Functions
	void GetPhysicalDevice();

	//Support Functions

	//Check Funtions
	bool checkInstanceExtensionSupport(std::vector<const char*>* checkExtensions);
	bool checkDeviceSuitable(VkPhysicalDevice device);

	//Getter Functions
	QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device);
};



