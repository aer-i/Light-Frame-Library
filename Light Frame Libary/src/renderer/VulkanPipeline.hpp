#pragma once
#include "VulkanSwapchain.hpp"

class PipelineLayout
{
public:
	class Builder
	{
	public:
		Builder() = default;
		Builder& addDescriptorSetLayout(const vk::DescriptorSetLayout& setLayout);
		Builder& addPushConstantRange(const vk::PushConstantRange& pushConstantRange);
		PipelineLayout build() const;

	private:
		std::vector<vk::DescriptorSetLayout> m_setLayouts;
		std::vector<vk::PushConstantRange> m_pushConstantRanges;
	};

	PipelineLayout() = default;
	PipelineLayout(const std::vector<vk::DescriptorSetLayout>& setLayouts, const std::vector<vk::PushConstantRange>& pushConstantRanges);
	void teardown();

	constexpr operator vk::PipelineLayout() const { return m_handle; }

private:
	vk::PipelineLayout m_handle;
};

class Pipeline
{
public:
	Pipeline() = default;
	void construct(const VulkanSwapchain& swapchain, PipelineLayout& pipelineLayout);
	void teardown();

	constexpr operator vk::Pipeline() const { return m_handle; }

private:
	vk::Pipeline m_handle;
};

