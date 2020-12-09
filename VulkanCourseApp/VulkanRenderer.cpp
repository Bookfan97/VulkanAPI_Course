#include "VulkanRenderer.h"

VulkanRenderer::VulkanRenderer()
{
}

VulkanRenderer::~VulkanRenderer()
{
}

int VulkanRenderer::init(GLFWwindow* newWindow)
{
	window = newWindow;
	try
	{
		createInstance();
		GetPhysicalDevice();
		createLogicalDevice();
	}
	catch (const std::runtime_error &e)
	{
		printf("ERROR: %s\n", e.what());
		return EXIT_FAILURE;
	}
	
	return 0;
}

void VulkanRenderer::cleanup()
{
	vkDestroyDevice(mainDevice.logicalDevice, nullptr);
	//TODO: 2nd paramter is deallocator
	vkDestroyInstance(instance, nullptr);
}

void VulkanRenderer::createInstance()
{
	//Information about the application itself (developer use only)
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan App";					//Custom App name
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);		//Custom version of the application
	appInfo.pEngineName = "No Engine";							//Custom Engine Name
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);			//Custome Engine Version
	appInfo.apiVersion = VK_API_VERSION_1_0;					//Vulkan API Version
	
	//Creation information for a vulkan instance
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	//Create list ot hold instance extensions
	std::vector<const char*> instanceExtensions = std::vector<const char*>();

	//Set up extensions instance will use
	uint32_t glfwExtentsionCount = 0;							//GLFW may require multiple extensions
	const char** glfwExtenstions;								//Extensions passed as array of cstrings

	//Get extension count from GLFW
	glfwExtenstions = glfwGetRequiredInstanceExtensions(&glfwExtentsionCount);

	//Add GLFW extensions to list of extensions
	for (size_t i = 0; i < glfwExtentsionCount; ++i)
	{
		instanceExtensions.push_back(glfwExtenstions[i]);
	}

	//Check instance extension supported
	if (!checkInstanceExtensionSupport(&instanceExtensions))
	{
		throw std::runtime_error("vkInstance does not support required extension");
	}

	createInfo.enabledExtensionCount = static_cast<uint32_t> (instanceExtensions.size());
	createInfo.ppEnabledExtensionNames = instanceExtensions.data();

	//TODO: Set up validation layers that instance will use
	createInfo.enabledLayerCount=0;
	createInfo.ppEnabledLayerNames = nullptr;

	//Create instance
	//TODO: 2nd parameter is memory management, look at later
	VkResult result = vkCreateInstance(&createInfo, nullptr,&instance);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create a Vulkan instance");
	}
}

void VulkanRenderer::createLogicalDevice()
{
	//Get the queue family indices for the chosen physical device
	QueueFamilyIndices indices = getQueueFamilies(mainDevice.physicalDevice);
	
	//Physical device features the logical devices will be using
	VkPhysicalDeviceFeatures device_features = {};

	//Queue the logical device create info
	VkDeviceQueueCreateInfo queue_create_info = {};
	queue_create_info.queueFamilyIndex = indices.graphicsFamily;	//Index of the family to create a queue from
	queue_create_info.queueCount = 1;								//Number of queues to create
	float priority = 1.0f;
	queue_create_info.pQueuePriorities = &priority;					//Vulkan needs to know how to handle multiple queues

	//Information to create a logical device
	VkDeviceCreateInfo device_create_info = {};
	device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_create_info.queueCreateInfoCount = 1;					//Number of queue create info
	device_create_info.pQueueCreateInfos = &queue_create_info;		//List og queue create info
	device_create_info.enabledExtensionCount = 0;					//Num of enabled logical devices
	device_create_info.ppEnabledExtensionNames = nullptr;			// list of enabled logical devices
	device_create_info.pEnabledFeatures = &device_features;			//Physical device features logical

	//Create the logical device for the physical device
	VkResult result = vkCreateDevice(mainDevice.physicalDevice, &device_create_info, nullptr, &mainDevice.logicalDevice);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create logical device");
	}

	//Queues are created at the same time as the device, handle queues
	vkGetDeviceQueue(mainDevice.logicalDevice, indices.graphicsFamily, 0, &graphicsQueue);
}

void VulkanRenderer::GetPhysicalDevice()
{
	//Get a list of possible devices
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	//Check for devices that support vulkan
	if (deviceCount == 0)
	{
		throw std::runtime_error("No devices found that support Vulkan");
	}
	
	//Get list of physical devices
	std::vector<VkPhysicalDevice> deviceList(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, deviceList.data());

	for (const auto &device : deviceList)
	{
		if (checkDeviceSuitable(device))
		{
			mainDevice.physicalDevice = device;
			break;
		}
	}
	
}

inline bool VulkanRenderer::checkDeviceSuitable(VkPhysicalDevice device)
{
	//Information about the device itself
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	
	//Information about what the device can do
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	printf("%s\n",deviceProperties.deviceName);

	QueueFamilyIndices indices = getQueueFamilies(device);
	return indices.isValid();
}

QueueFamilyIndices VulkanRenderer::getQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;
	uint32_t queueFamilyCount = 0;

	//Get all properties for given device
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyList.data());

	//Check if each queue family has at least one required type of queue
	int i = 0;
	for (const auto &queueFamily : queueFamilyList)
	{
		//check if queue family has at least 1 wueue in hat family
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i; //if valid, get index	
		}

		//Check if in valid state
		if (indices.isValid())
		{
			break;
		}
		i++;
	}
	return indices;
}

bool VulkanRenderer::checkInstanceExtensionSupport(std::vector<const char*>* checkExtensions)
{
	//Need to get number of extensions to create the array
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	//Create list of vkExtensionProperties using count
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	//Check if given extensions are available
	for (const auto &checkExtension : *checkExtensions)
	{
		bool hasExtension = false;
		for (const auto extension : extensions)
		{
			if (strcmp(checkExtension, extension.extensionName))
			{
				hasExtension = true;
				break;
			}
		}
		if(!hasExtension)
		{
			return false;
		}
	}
	return true;
}
