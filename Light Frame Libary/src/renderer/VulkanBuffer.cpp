#include "pch.hpp"
#include "VulkanBuffer.hpp"
#include "VulkanContext.hpp"

VulkanBuffer::VulkanBuffer(vk::DeviceSize bufferSize, VkBufferUsageFlags bufferUsage, vk::MemoryPropertyFlags memoryProperty)
	: size{ bufferSize }
{
	assert(bufferSize > 0);

	VkBufferCreateInfo bufferCI {
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = bufferSize,
		.usage = bufferUsage
	};

	VmaAllocationCreateInfo allocationCI{
		.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
		.usage = VMA_MEMORY_USAGE_AUTO
	};

	VkBuffer buffer;
	vmaCreateBuffer(vc::Get().allocator, &bufferCI, &allocationCI, &buffer, &allocation, nullptr);
	m_handle = vk::Buffer(buffer);

	bind();
}

VulkanBuffer::~VulkanBuffer()
{
	
}

void VulkanBuffer::free()
{
	
}

vk::DescriptorBufferInfo VulkanBuffer::descriptorInfo(vk::DeviceSize size, vk::DeviceSize offset)
{
	return { m_handle, offset, size };
}

void VulkanBuffer::map(vk::DeviceSize size, vk::DeviceSize offset)
{
	
}

void VulkanBuffer::unmap()
{
	
}

void VulkanBuffer::writeToBuffer(void* data, vk::DeviceSize size, vk::DeviceSize offset)
{
	
}

void VulkanBuffer::bind(vk::DeviceSize offset)
{
}

void VulkanBuffer::flush(vk::DeviceSize size, vk::DeviceSize offset)
{
	
}