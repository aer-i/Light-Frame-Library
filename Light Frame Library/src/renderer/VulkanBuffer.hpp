#pragma once

class VulkanBuffer
{
public:
	VulkanBuffer() = default;
	~VulkanBuffer() = default;
	void create(vk::DeviceSize bufferSize, VkBufferUsageFlags bufferUsage, VmaAllocationCreateFlags allocationFlags, VmaAllocationInfo* allocationInfo = nullptr);
	void free();

	vk::DescriptorBufferInfo descriptorInfo(vk::DeviceSize size = VK_WHOLE_SIZE, vk::DeviceSize offset = 0);
	void map();
	void unmap();
	void writeToBuffer(void* data, vk::DeviceSize size = VK_WHOLE_SIZE, vk::DeviceSize offset = 0);
	void flush(vk::DeviceSize size = VK_WHOLE_SIZE, vk::DeviceSize offset = 0);

	constexpr operator vk::Buffer() const { return m_handle; }
	constexpr operator vk::Buffer* () { return &m_handle; }
	constexpr operator bool() const { return m_handle ? true : false; }

	void* mapped = nullptr;
	VmaAllocation allocation{};
	vk::DeviceSize size = 0;
	uint32_t count = 0;

private:
	vk::Buffer m_handle;
};