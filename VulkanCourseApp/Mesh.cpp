#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, VkQueue transferQueue, 
	VkCommandPool transferCommandPool, std::vector<Vertex>* vertices, std::vector<uint32_t>* indices)
{
	vertexCount = vertices->size();
	indexCount = indices->size();
	physicalDevice = newPhysicalDevice;
	device = newDevice;
	createVertexBuffer(transferQueue, transferCommandPool, vertices);
	createIndexBuffer(transferQueue, transferCommandPool, indices);
	model.model = glm::mat4(1.0f);
}

void Mesh::setModel(glm::mat4 newModel)
{
	model.model = newModel;
}

Model Mesh::getModel()
{
	return model;
}

int Mesh::getVertexCount()
{
	return vertexCount;
}

VkBuffer Mesh::getVertexBuffer()
{
	return vertexBuffer;
}

int Mesh::getIndexCount()
{
	return indexCount;
}

VkBuffer Mesh::getIndexBuffer()
{
	return indexbuffer;
}

void Mesh::destroyBuffers()
{
	vkDestroyBuffer(device, vertexBuffer, nullptr);
	vkFreeMemory(device, vertexBufferMemory, nullptr);
	vkDestroyBuffer(device, indexbuffer, nullptr);
	vkFreeMemory(device, indexBufferMemory, nullptr);
}

Mesh::~Mesh()
{
}

void Mesh::createVertexBuffer(VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<Vertex>* vertices)
{
	VkDeviceSize bufferSize = sizeof(Vertex) * vertices->size();
	//Temp buffer to stage vertex data
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	createBuffer(physicalDevice, device, bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&stagingBuffer, &stagingBufferMemory);
	//Map memory to vertex buffer
	void* data;																				//Create pointer to a point in normal memory
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize,0, &data);		//Map vertex buffer memory to point
	memcpy(data, vertices->data(),(size_t)bufferSize);								//Copy memory from vertices vector to point
	vkUnmapMemory(device, stagingBufferMemory);												//Unmap the vertex buffer memory

	//Create buffer to transfer to
	createBuffer(physicalDevice, device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &vertexBuffer, &vertexBufferMemory);

	//Copy staging buffer
	copyBuffer(device, transferQueue, transferCommandPool, stagingBuffer, vertexBuffer, bufferSize);

	//Clean up staging buffer
	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void Mesh::createIndexBuffer(VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<uint32_t>* indices)
{
	//Get size of vuffer
	VkDeviceSize bufferSize = sizeof(uint32_t) * indices->size();

	//Temp buffer to stage vertex data
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	createBuffer(physicalDevice, device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);

	//Map memory to vertex buffer
	void* data;																				//Create pointer to a point in normal memory
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);		//Map vertex buffer memory to point
	memcpy(data, indices->data(), (size_t)bufferSize);								//Copy memory from vertices vector to point
	vkUnmapMemory(device, stagingBufferMemory);												//Unmap the vertex buffer memory

	//Create buffer for index data
	createBuffer(physicalDevice, device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &indexbuffer, &indexBufferMemory);
	//Copy from staging
	copyBuffer(device, transferQueue, transferCommandPool, stagingBuffer, indexbuffer, bufferSize);
	//Destroy and release
	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}
