#include "pch.hpp"
#include "VulkanBuffer.hpp"
#include "VulkanContext.hpp"

// Day 4

// What is that???
// I hate memory allocation

// I like low level things but this is very complex

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
	unmap();
	if (memory)
		vc::device().free(memory);
	if (m_handle)
		vc::device().destroy(m_handle);

	m_handle = nullptr;
}

vk::DescriptorBufferInfo VulkanBuffer::descriptorInfo(vk::DeviceSize size, vk::DeviceSize offset)
{
	return { m_handle, offset, size };
}

void VulkanBuffer::map(vk::DeviceSize size, vk::DeviceSize offset)
{
	if (size == VK_WHOLE_SIZE)
	{
		mapped = vc::device().mapMemory(memory, offset, this->size);
	}
	else
	{
		mapped = vc::device().mapMemory(memory, offset, size);
	}
}

void VulkanBuffer::unmap()
{
	if (mapped)
	{
		vc::device().unmapMemory(memory);
		mapped = nullptr;
	}
}

void VulkanBuffer::writeToBuffer(void* data, vk::DeviceSize size, vk::DeviceSize offset)
{
	if (size == VK_WHOLE_SIZE)
	{
		memcpy(mapped, data, this->size);
	}
	else
	{
		u8* memOffset = (u8*)mapped;
		memOffset += offset;
		memcpy(memOffset, data, size);
	}
}

void VulkanBuffer::bind(vk::DeviceSize offset)
{
	vc::device().bindBufferMemory(m_handle, memory, offset);
}

void VulkanBuffer::flush(vk::DeviceSize size, vk::DeviceSize offset)
{
	const auto& range = vk::MappedMemoryRange()
		.setOffset(offset)
		.setMemory(memory)
		.setSize(size);

	vc::device().flushMappedMemoryRanges(range);
}