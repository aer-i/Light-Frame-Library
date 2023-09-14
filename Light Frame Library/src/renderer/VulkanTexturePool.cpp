#include "pch.hpp"
#include "VulkanTexturePool.hpp"
#include "VulkanContext.hpp"

void VulkanTexturePool::create()
{
	vk::DescriptorPoolSize poolSize {
		.type = vk::DescriptorType::eCombinedImageSampler,
		.descriptorCount = vc::Get().deviceProperties.properties.limits.maxSamplerAllocationCount
	};

	descriptorPool = vc::Get().device.createDescriptorPool({
		.flags = vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind,
		.maxSets = 1,
		.poolSizeCount = 1,
		.pPoolSizes = &poolSize
	});

	vk::DescriptorSetLayoutBinding layoutBinding {
		.binding = 0,
		.descriptorType = vk::DescriptorType::eCombinedImageSampler,
		.descriptorCount = vc::Get().deviceProperties.properties.limits.maxSamplerAllocationCount,
		.stageFlags = vk::ShaderStageFlagBits::eFragment
	};

	vk::DescriptorBindingFlags constexpr descriptorBindingFlags =
		vk::DescriptorBindingFlagBits::eVariableDescriptorCount | vk::DescriptorBindingFlagBits::ePartiallyBound | vk::DescriptorBindingFlagBits::eUpdateAfterBind | vk::DescriptorBindingFlagBits::eUpdateUnusedWhilePending;

	vk::DescriptorSetLayoutBindingFlagsCreateInfo const setLayoutBindingFlags {
		.bindingCount = 1,
		.pBindingFlags = &descriptorBindingFlags
	};

	descriptorSetLayout = vc::Get().device.createDescriptorSetLayout({
		.pNext = &setLayoutBindingFlags,
		.flags = vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool,
		.bindingCount = 1,
		.pBindings = &layoutBinding
	});

	uint32_t variableDescCounts[] = { vc::Get().deviceProperties.properties.limits.maxSamplerAllocationCount };

	vk::DescriptorSetVariableDescriptorCountAllocateInfo variableDescriptorCountAllocInfo {
		.descriptorSetCount = 1,
		.pDescriptorCounts = variableDescCounts
	};

	descriptorSet = vc::Get().device.allocateDescriptorSets({
		.pNext = &variableDescriptorCountAllocInfo,
		.descriptorPool = descriptorPool,
		.descriptorSetCount = 1,
		.pSetLayouts = &descriptorSetLayout
	}).front();


	lf2d::Texture("");
}

void VulkanTexturePool::teardown()
{
	for (auto& texture : textures)
	{
		texture.teardown();
	}

	vc::Get().device.destroy(descriptorSetLayout);
	vc::Get().device.destroy(descriptorPool);
}

void VulkanTexturePool::loadTexture(std::string_view filepath, bool pixelated)
{
	textures.emplace_back(filepath, pixelated);

	std::vector<vk::DescriptorImageInfo> textureDescriptors(textures.size());
	for (uint32_t i = 0; i < textureDescriptors.size(); i++)
	{
		textureDescriptors[i] = textures[i].getImageInfo();
	}

	vc::Get().device.updateDescriptorSets(
		vk::WriteDescriptorSet {
		.dstSet = descriptorSet,
			.dstBinding = 0,
			.dstArrayElement = 0,
			.descriptorCount = static_cast<uint32_t>(textures.size()),
			.descriptorType = vk::DescriptorType::eCombinedImageSampler,
			.pImageInfo = textureDescriptors.data()
	},
		nullptr
	);
}

void VulkanTexturePool::loadTexture(void* buffer, vk::DeviceSize bufferSize, uint32_t width, uint32_t height)
{
	textures.emplace_back(buffer, bufferSize, width, height);

	std::vector<vk::DescriptorImageInfo> textureDescriptors(textures.size());
	for (uint32_t i = 0; i < textureDescriptors.size(); i++)
	{
		textureDescriptors[i] = textures[i].getImageInfo();
	}

	vc::Get().device.updateDescriptorSets(
		vk::WriteDescriptorSet{
		.dstSet = descriptorSet,
			.dstBinding = 0,
			.dstArrayElement = 0,
			.descriptorCount = static_cast<uint32_t>(textures.size()),
			.descriptorType = vk::DescriptorType::eCombinedImageSampler,
			.pImageInfo = textureDescriptors.data()
		},
		nullptr
	);
}