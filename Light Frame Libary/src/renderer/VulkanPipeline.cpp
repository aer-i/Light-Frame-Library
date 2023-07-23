#include "pch.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanContext.hpp"

PipelineLayout::Builder& PipelineLayout::Builder::addDescriptorSetLayout(const vk::DescriptorSetLayout& setLayout)
{
	m_setLayouts.push_back(setLayout);
	return *this;
}

PipelineLayout::Builder& PipelineLayout::Builder::addPushConstantRange(const vk::PushConstantRange& pushConstantRange)
{
	m_pushConstantRanges.push_back(pushConstantRange);
	return *this;
}

PipelineLayout PipelineLayout::Builder::build() const
{
	return PipelineLayout(m_setLayouts, m_pushConstantRanges);
}

PipelineLayout::PipelineLayout(const std::vector<vk::DescriptorSetLayout>& setLayouts, const std::vector<vk::PushConstantRange>& pushConstantRanges)
{
	m_handle = vc::Get().device.createPipelineLayout({
		.setLayoutCount = static_cast<uint32_t>(setLayouts.size()),
		.pSetLayouts = setLayouts.data(),
		.pushConstantRangeCount = static_cast<uint32_t>(pushConstantRanges.size()),
		.pPushConstantRanges = pushConstantRanges.data()
	});
}

PipelineLayout::~PipelineLayout()
{
	
}

//Why that stupid pipeline IS SO HUGEEEEE

//Someone tell me how to use timeline semaphores pls

void Pipeline::construct(const vi::Swapchain& swapchain, PipelineLayout& pipelineLayout)
{
	auto vertShaderModule

	const vk::PipelineShaderStageCreateInfo shaderStages[]
	{
		{ {}, vk::ShaderStageFlagBits::eVertex, vertShaderModule, "main" },
		{ {}, vk::ShaderStageFlagBits::eFragment, fragShaderModule, "main" }
	};

	vk::PipelineInputAssemblyStateCreateInfo inputAssembly	{
		.topology = vk::PrimitiveTopology::eTriangleList,
		.primitiveRestartEnable = false
	};

	vk::PipelineRasterizationStateCreateInfo rasterizer{
		.depthClampEnable =,
		.rasterizerDiscardEnable =,
		.polygonMode =,
		.cullMode =,
		.frontFace =,
		.depthBiasEnable =,
		.depthBiasConstantFactor =,
		.depthBiasClamp =,
		.depthBiasSlopeFactor =,
		.lineWidth = };

	vk::PipelineMultisampleStateCreateInfo multisampling{};

	vk::PipelineDepthStencilStateCreateInfo  depthStencil{};

	vk::PipelineColorBlendAttachmentState colorBlendAttachment {
		.blendEnable =,
			.srcColorBlendFactor =,
			.dstColorBlendFactor =,
			.colorBlendOp =,
			.srcAlphaBlendFactor =,
			.dstAlphaBlendFactor =,
			.alphaBlendOp =,
			.colorWriteMask = };

	vk::PipelineColorBlendStateCreateInfo colorBlending {
		.sType =,
			.pNext =,
			.flags =,
			.logicOpEnable =,
			.logicOp =,
			.attachmentCount =,
			.pAttachments =,
			.blendConstants =
	};

	constexpr vk::PipelineViewportStateCreateInfo viewportState({ .viewportCount = 1, .scissorCount = 1 });
	const vk::PipelineVertexInputStateCreateInfo vertexInputInfo({.vertexBindingDescriptionCount = , .pVertexBindingDescriptions = , .vertexAttributeDescriptionCount = , .pVertexAttributeDescriptions = });
	const std::vector dynamicStates{ vk::DynamicState::eViewport, vk::DynamicState::eScissor };
	const vk::PipelineDynamicStateCreateInfo dynamicState({}, dynamicStates);


	m_handle = vc::device().createGraphicsPipeline(nullptr, pipelineCI).value;
}

Pipeline::~Pipeline()
{
	
}