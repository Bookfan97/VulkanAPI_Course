#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Utilities.h"
#include <stdexcept>
#include <iostream>
#include <vector>
#include <set>
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
	VkDebugReportCallbackEXT callback;
	struct
	{
		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;
	} mainDevice;
	
	VkQueue graphicsQueue;
	VkQueue presentationQueue;
	VkSurfaceKHR surface;

	//----------------------Vulkan Functions------------------------
	
	//Create Functions
	void createInstance();
	void createDebugCallback();
	void createLogicalDevice();
	void createSurface();

	//Get Functions
	void getPhysicalDevice();

	//Support Functions

	//Check Funtions
	bool checkInstanceExtensionSupport(std::vector<const char*>* checkExtensions);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
#ifdef VK_DEBUG
		const bool enableValidationLayers = true;
#else
	const bool enableValidationLayers = false;
#endif
		const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};
	bool checkValidationLayerSupport();

	bool checkDeviceSuitable(VkPhysicalDevice device);

	//Getter Functions
	QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device);
	SwapChainDetails getSwapChainDetails(VkPhysicalDevice device);

};

