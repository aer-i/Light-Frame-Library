#include "pch.hpp"
#include "VulkanContext.hpp"
#include "VulkanTexture.hpp"
#include "VulkanBuffer.hpp"

VulkanTexture::VulkanTexture(std::string_view filepath, bool pixelated)
{
	if (filepath == "")
	{
		createDefaultTexture();
		return;
	}

	TextureFile file(filepath, &m_width, &m_height);
	while (!file.getPixels())
	{
		printf("[error] Can't load texture: '%s'\n", filepath.data());

		createDefaultTexture();
		return;
	}

	m_size = 4 * m_width * m_height;

	VkImageCreateInfo imageCI = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		.imageType = VK_IMAGE_TYPE_2D,
		.format = VK_FORMAT_R8G8B8A8_SRGB,
		.extent = VkExtent3D{m_width, m_height, 1},
		.mipLevels = 1,
		.arrayLayers = 1,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.tiling = VK_IMAGE_TILING_OPTIMAL,
		.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
	};

	VmaAllocationCreateInfo allocationCI = {
		.usage = VMA_MEMORY_USAGE_GPU_ONLY
	};
	
	VkImage image;
	vmaCreateImage(vc::Get().allocator, &imageCI, &allocationCI, &image, &m_allocation, nullptr);
	m_image = vk::Image(image);

	VulkanBuffer stagingBuffer;

	stagingBuffer.create(m_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);
	stagingBuffer.map();
	stagingBuffer.writeToBuffer(file.getPixels());
	stagingBuffer.unmap();


	vc::TransitionImageLayout(image, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
	vc::CopyBufferToImage(stagingBuffer, m_image, m_width, m_height);
	vc::TransitionImageLayout(image, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);

	stagingBuffer.free();

	m_imageView = vc::CreateImageView(image, vk::ImageViewType::e2D, vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor);

	m_sampler = vc::Get().device.createSampler({
		.magFilter = pixelated ? vk::Filter::eNearest : vk::Filter::eLinear,
		.minFilter = pixelated ? vk::Filter::eNearest : vk::Filter::eLinear,
		.addressModeU = vk::SamplerAddressMode::eRepeat,
		.addressModeV = vk::SamplerAddressMode::eRepeat,
		.addressModeW = vk::SamplerAddressMode::eRepeat,
		.compareEnable = false,
		.compareOp = vk::CompareOp::eNever,
		.borderColor = vk::BorderColor::eIntOpaqueBlack,
		.unnormalizedCoordinates = false
	});
}

void VulkanTexture::teardown()
{
	if (m_sampler)		vc::Get().device.destroy(m_sampler);
	if (m_imageView)	vc::Get().device.destroy(m_imageView);
	if (m_image)		vmaDestroyImage(vc::Get().allocator, m_image, m_allocation);
}

void VulkanTexture::createDefaultTexture()
{
	m_width = 1;
	m_height = 1;

	m_size = 4;

	VkImageCreateInfo imageCI = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		.imageType = VK_IMAGE_TYPE_2D,
		.format = VK_FORMAT_R8G8B8A8_SRGB,
		.extent = VkExtent3D{m_width, m_height, 1},
		.mipLevels = 1,
		.arrayLayers = 1,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.tiling = VK_IMAGE_TILING_OPTIMAL,
		.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
	};

	VmaAllocationCreateInfo allocationCI = {
		.usage = VMA_MEMORY_USAGE_GPU_ONLY
	};

	VkImage image;
	vmaCreateImage(vc::Get().allocator, &imageCI, &allocationCI, &image, &m_allocation, nullptr);
	m_image = vk::Image(image);

	VulkanBuffer stagingBuffer;

	uint8_t pixels[] = { 255, 255, 255, 255 };

	stagingBuffer.create(m_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT);
	stagingBuffer.map();
	stagingBuffer.writeToBuffer(pixels);
	stagingBuffer.unmap();

	vc::TransitionImageLayout(image, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
	vc::CopyBufferToImage(stagingBuffer, m_image, m_width, m_height);
	vc::TransitionImageLayout(image, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);

	stagingBuffer.free();

	m_imageView = vc::CreateImageView(image, vk::ImageViewType::e2D, vk::Format::eR8G8B8A8Srgb, vk::ImageAspectFlagBits::eColor);

	m_sampler = vc::Get().device.createSampler({
		.magFilter = vk::Filter::eNearest,
		.minFilter = vk::Filter::eNearest,
		.addressModeU = vk::SamplerAddressMode::eRepeat,
		.addressModeV = vk::SamplerAddressMode::eRepeat,
		.addressModeW = vk::SamplerAddressMode::eRepeat,
		.compareEnable = false,
		.compareOp = vk::CompareOp::eNever,
		.borderColor = vk::BorderColor::eIntOpaqueBlack,
		.unnormalizedCoordinates = false
	});
}
