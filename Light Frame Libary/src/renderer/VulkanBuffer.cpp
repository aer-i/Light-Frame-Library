#include "pch.hpp"
#include "VulkanBuffer.hpp"
#include "VulkanContext.hpp"


VulkanBuffer::~VulkanBuffer()
{
	
}

void VulkanBuffer::create(vk::DeviceSize bufferSize, VkBufferUsageFlags bufferUsage)
{
	assert(bufferSize > 0);

	this->size = bufferSize;

	VkBufferCreateInfo bufferCI{
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = bufferSize,
		.usage = bufferUsage
	};

	VmaAllocationCreateInfo allocationCI{
		.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
		.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE
	};

	VkBuffer buffer;
	vmaCreateBuffer(vc::Get().allocator, &bufferCI, &allocationCI, &buffer, &allocation, nullptr);

	m_handle = vk::Buffer(buffer);

	map();
}

void VulkanBuffer::free()
{
	this->unmap();
	if (m_handle) vmaDestroyBuffer(vc::Get().allocator, m_handle, allocation);
}

vk::DescriptorBufferInfo VulkanBuffer::descriptorInfo(vk::DeviceSize size, vk::DeviceSize offset)
{
	return { m_handle, offset, size };
}

void VulkanBuffer::map(vk::DeviceSize size, vk::DeviceSize offset)
{
	vmaMapMemory(vc::Get().allocator, allocation, &mapped);
}

void VulkanBuffer::unmap()
{
	if (mapped)
	{
		vmaUnmapMemory(vc::Get().allocator, allocation);
		mapped = nullptr;
	}
}

void VulkanBuffer::writeToBuffer(void* data, vk::DeviceSize size, vk::DeviceSize offset)
{
	
}

void VulkanBuffer::flush(vk::DeviceSize size, vk::DeviceSize offset)
{
	
}