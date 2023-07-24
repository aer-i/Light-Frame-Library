#pragma once

class VulkanBuffer
{
public:
	VulkanBuffer() = default;
	VulkanBuffer(vk::DeviceSize bufferSize, VkBufferUsageFlags bufferUsage, vk::MemoryPropertyFlags memoryProperty);
	~VulkanBuffer();
	void free();

	vk::DescriptorBufferInfo descriptorInfo(vk::DeviceSize size = VK_WHOLE_SIZE, vk::DeviceSize offset = 0);
	void map(vk::DeviceSize size = VK_WHOLE_SIZE, vk::DeviceSize offset = 0);
	void unmap();
	void writeToBuffer(void* data, vk::DeviceSize size = VK_WHOLE_SIZE, vk::DeviceSize offset = 0);
	void bind(vk::DeviceSize offset = 0);
	void flush(vk::DeviceSize size = VK_WHOLE_SIZE, vk::DeviceSize offset = 0);

	constexpr operator vk::Buffer() const { return m_handle; }
	constexpr operator vk::Buffer* () { return &m_handle; }
	constexpr operator bool() const { return m_handle ? true : false; }

	VmaAllocation allocation;
	void* mapped = nullptr;
	vk::DeviceSize size = 0;
	uint32_t count = 0;

private:
	vk::Buffer m_handle;
};

