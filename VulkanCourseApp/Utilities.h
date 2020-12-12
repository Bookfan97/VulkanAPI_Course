#pragma once
#include <vector>

//Indices of Queue Families (if they exist)
struct QueueFamilyIndices
{
	int graphicsFamily = -1;		//Location of graphics queue family
	int presentationFamily = -1;		// Location of Presentation Queue Family

	//Check is queue families are valid
	bool isValid()
	{
		return graphicsFamily >= 0 && presentationFamily >= 0;
	}
};

struct SwapChainDetails {
	VkSurfaceCapabilitiesKHR surfaceCapabilities;		// Surface properties
	std::vector<VkSurfaceFormatKHR> formats;			// Surface image formats
	std::vector<VkPresentModeKHR> presentationModes;	// How images should be presented to screen
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct SwapchainImage
{
	VkImage image;
	VkImageView imageView;
};
