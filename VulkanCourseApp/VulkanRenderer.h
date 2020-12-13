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
#ifdef VK_DEBUG
	const bool enableValidationLayers = true;
#else
	const bool enableValidationLayers = false;
#endif

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};
	bool checkValidationLayerSupport();
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
