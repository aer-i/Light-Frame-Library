#pragma once
#include "VulkanTexture.hpp"
#include "lf2d.hpp"

class VulkanTexturePool
{
public:
	void create();
	void teardown();

	void loadTexture(std::string_view filepath, bool pixelated);
	void loadTexture(void* buffer, vk::DeviceSize bufferSize);

	vk::DescriptorSetLayout descriptorSetLayout;
	vk::DescriptorSet descriptorSet;
	vk::DescriptorPool descriptorPool;
	vk::PhysicalDeviceDescriptorIndexingProperties descriptorIndexingProperties;
	std::vector<VulkanTexture> textures;
};

