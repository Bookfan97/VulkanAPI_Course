#pragma once
#include <vector>
#include <fstream>

const int MAX_FRAME_DRAWS = 2;

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

static std::vector<char> readFile(const std::string& filename)
{
	//Open stream from file
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	//Check if file stream successfully opened
	if (!file.is_open())
	{
		throw std::runtime_error("FAILED TO OPEN FILE");
	}

	//Get current read position, resize buffer
	size_t fileSize = (size_t)file.tellg();
	std::vector<char> fileBuffer(fileSize);

	//Move read position to start of file
	file.seekg(0);

	//Read the file data into the buffer
	file.read(fileBuffer.data(), fileSize);

	//Close stream
	file.close();

	return fileBuffer;
}