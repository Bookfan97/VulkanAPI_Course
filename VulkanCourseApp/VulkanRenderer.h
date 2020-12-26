#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Utilities.h"
#include <stdexcept>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <array>
#include "Mesh.h"
#include "VulkanValidation.h"

class VulkanRenderer
{
public:
	VulkanRenderer();

	int init(GLFWwindow * newWindow);

	void updateModel(int modelId, glm::mat4 newModel);

	void draw();
	void cleanup();
	~VulkanRenderer();
	void createMVP();
	void createMeshData();
	void createMesh();
private:
	GLFWwindow* window;
	int currentFrame = 0;
	
	//Scene
	std::vector<Mesh> meshList;

	struct UboViewProjection
	{
		glm::mat4 projection;
		glm::mat4 view;
	} uboViewProjection;

	//===========Components=================
	//Main
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
	VkSwapchainKHR swapchain;
	std::vector<SwapchainImage> swapChainImages;
	std::vector<VkFramebuffer> swapchainFramebuffers;
	std::vector<VkCommandBuffer> commandBuffers;

	VkImage depthBufferImage;
	VkDeviceMemory depthBufferImageMemory;
	VkImageView depthBufferImageView;

	// - Descriptors
	VkDescriptorSetLayout descriptorSetLayout;
	VkPushConstantRange pushConstantRange;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
	std::vector<VkBuffer> vpUniformBuffer;
	std::vector<VkDeviceMemory> vpUniformBufferMemory;
	std::vector<VkBuffer> modelDUniformBuffer;
	std::vector<VkDeviceMemory> modelDUniformBufferMemory;

	/*VkDeviceSize minUniformBufferOffset;
	size_t modelUniformAlignment;*/
	//Model* modelTransferSpace;
	
	// - Pipeline
	VkPipeline graphicsPipeline;
	VkPipelineLayout pipelineLayout;
	VkRenderPass renderPass;

	// - Utility
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	
	// - Pools
	VkCommandPool graphicsCommandPool;

	//Synchronization
	std::vector<VkSemaphore> imageAvailable;
	std::vector<VkSemaphore> renderFinished;
	std::vector<VkFence> drawFences;
	//----------------------Vulkan Functions------------------------

	//Create Functions
	void createInstance();
	void createDebugCallback();
	void createLogicalDevice();
	void createSurface();
	void createSwapchain();
	void createRenderPass();
	void createDescriptorSetLayout();
	void createPushConstantRange();
	void createGraphicsPipeline();
	void createDepthBufferImage();
	void createFramebuffers();
	void createCommandPool();
	void createCommandBuffers();
	void createSynchronization();
	void createUniformBuffers();
	void createDescriptorPool();
	void createDescriptorSets();

	//Update Functions
	void updateUniformBuffers(uint32_t imageIndex);
	
	//Record functions
	void recordCommands(uint32_t currentImage);

	//Get Functions
	void getPhysicalDevice();

	// - Allocate Functions
	void allocateDynamicBufferTransferSpace();

	//- Support Functions

	//Check Funtions
	bool checkInstanceExtensionSupport(std::vector<const char*>* checkExtensions);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool checkValidationLayerSupport();
	bool checkDeviceSuitable(VkPhysicalDevice device);

	//Getter Functions
	QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device);
	SwapChainDetails getSwapChainDetails(VkPhysicalDevice device);

	// -- Choose Functions
	VkSurfaceFormatKHR chooseBestSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
	VkPresentModeKHR chooseBestPresentationMode(const std::vector<VkPresentModeKHR> presentationModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities);
	VkFormat chooseSupportedFormat(const std::vector<VkFormat> &formats, VkImageTiling tiling, VkFormatFeatureFlags featureFlags);

	//Create Functions
	VkImage createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags useFlags, VkMemoryPropertyFlags propFlags, VkDeviceMemory* imageMemory);
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	VkShaderModule createShaderModule(const std::vector<char>& code);
};
