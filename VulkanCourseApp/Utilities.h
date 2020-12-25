#pragma once
#define GLFW_INCLUDE_VULKAN
#include <vector>
#include <fstream>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

const int MAX_FRAME_DRAWS = 2;
const int MAX_OBJECTS = 2;

static void checkValidResult(VkResult result, std::string message)
{
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error(message);
	}
}
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

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 col;
};

static uint32_t findMemoryTypeIndex(VkPhysicalDevice physicalDevice, uint32_t allowedTypes, VkMemoryPropertyFlags properties)
{
	// Get properties of physical device memory
	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
	{
		if ((allowedTypes & (1 << i))														// Index of memory type must match corresponding bit in allowedTypes
			&& (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties)	// Desired property bit flags are part of memory type's property flags
		{
			// This memory type is valid, so return its index
			return i;
		}
	}
}

static void createBuffer(VkPhysicalDevice physicalDevice, VkDevice device, 
	VkDeviceSize bufferSize, VkBufferUsageFlags bufferUsage, VkMemoryPropertyFlags bufferProperties, 
	VkBuffer* buffer, VkDeviceMemory* bufferMemory)
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = bufferSize; //Sizw of buffer
	bufferInfo.usage = bufferUsage; //Multiple types of buffer possible
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; //Can share buffers
	checkValidResult(vkCreateBuffer(device, &bufferInfo, nullptr, buffer), "VERTEX BUFFER CREATION FAILED");

	//Get buffer memory requirements
	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(device, *buffer, &memoryRequirements);

	//Allocate memory to buffer
	VkMemoryAllocateInfo memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.allocationSize = memoryRequirements.size;
	memoryAllocateInfo.memoryTypeIndex = findMemoryTypeIndex(	//Index of memory type of Physical device
		physicalDevice,
		memoryRequirements.memoryTypeBits,
		bufferProperties
	);

	//Allocate memory
	checkValidResult(vkAllocateMemory(device, &memoryAllocateInfo, nullptr, bufferMemory), "VERTEX BUFFER MEMORY ALLOCATION FAILED");
	vkBindBufferMemory(device, *buffer, *bufferMemory, 0);
}

static void copyBuffer(VkDevice device, VkQueue transferQueue, VkCommandPool transferCommandPool,
	VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize bufferSize)
{
	//Command buffer to hold commands
	VkCommandBuffer transferCommandBuffer;

	//Command buffer details
	VkCommandBufferAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocateInfo.commandPool = transferCommandPool;
	allocateInfo.commandBufferCount = 1;

	//Allocate command buffer from pool
	vkAllocateCommandBuffers(device, &allocateInfo, &transferCommandBuffer);
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; //Only use once

	//Begin recording transfer commands
	vkBeginCommandBuffer(transferCommandBuffer, &beginInfo);

	//Region of data to copy to -> from
	VkBufferCopy bufferCopyRegion = {};
	bufferCopyRegion.srcOffset = 0;
	bufferCopyRegion.dstOffset = 0;
	bufferCopyRegion.size = bufferSize;

	//Command to copy src -> dst buffer
	vkCmdCopyBuffer(transferCommandBuffer, srcBuffer, dstBuffer, 1, &bufferCopyRegion); 
	vkEndCommandBuffer(transferCommandBuffer);

	//Queue submission
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &transferCommandBuffer;

	//Submit transfer command to transfer queue and wait till finished
	vkQueueSubmit(transferQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(transferQueue);

	//Free temp command buffer back to pool
	vkFreeCommandBuffers(device, transferCommandPool, 1, &transferCommandBuffer);
}