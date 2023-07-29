#include "pch.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanContext.hpp"
#include "Mesh.hpp"
#include "shaders.u32"

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

void PipelineLayout::teardown()
{
	if (m_handle) vc::Get().device.destroy(m_handle);
}

void Pipeline::construct(const VulkanSwapchain& swapchain, PipelineLayout& pipelineLayout)
{
	auto const vertShaderModule = vc::Get().device.createShaderModule({ .codeSize = sizeof(vertShaderCode), .pCode = vertShaderCode });
	auto const fragShaderModule = vc::Get().device.createShaderModule({ .codeSize = sizeof(fragShaderCode), .pCode = fragShaderCode });

	const vk::PipelineShaderStageCreateInfo shaderStages[]
	{
		{ .stage = vk::ShaderStageFlagBits::eVertex, .module = vertShaderModule, .pName = "main" },
		{ .stage = vk::ShaderStageFlagBits::eFragment, .module = fragShaderModule, .pName = "main" }
	};

	vk::VertexInputBindingDescription constexpr bindingDescription {
		.binding = 0,
		.stride = sizeof(Vertex),
		.inputRate = vk::VertexInputRate::eVertex
	};

	vk::VertexInputAttributeDescription constexpr posAttributeDescription {
		.location = 0,
		.binding = 0,
		.format = vk::Format::eR32G32Sfloat,
		.offset = offsetof(Vertex, position)
	};

	vk::VertexInputAttributeDescription constexpr colorAttributeDescription {
		.location = 1,
		.binding = 0,
		.format = vk::Format::eR32G32B32Sfloat,
		.offset = offsetof(Vertex, color)
	};

	vk::VertexInputAttributeDescription constexpr attributeDescriptions[]{ posAttributeDescription, colorAttributeDescription };

	vk::PipelineVertexInputStateCreateInfo constexpr vertexInput {
		.vertexBindingDescriptionCount = 1,
		.pVertexBindingDescriptions = &bindingDescription,
		.vertexAttributeDescriptionCount = 2,
		.pVertexAttributeDescriptions = attributeDescriptions
	};

	vk::PipelineInputAssemblyStateCreateInfo constexpr inputAssembly {
		.topology = vk::PrimitiveTopology::eTriangleList,
		.primitiveRestartEnable = false
	};

	vk::PipelineViewportStateCreateInfo constexpr viewportState {
		.viewportCount = 1,
		.scissorCount = 1
	};

	vk::PipelineRasterizationStateCreateInfo constexpr rasterizer {
		.depthClampEnable = false,
		.rasterizerDiscardEnable = false,
		.polygonMode = vk::PolygonMode::eFill,
		.cullMode = vk::CullModeFlagBits::eNone,
		.frontFace = vk::FrontFace::eClockwise,
		.depthBiasEnable = false,
		.lineWidth = 1.f
	};

	vk::PipelineMultisampleStateCreateInfo constexpr multisampling {
		.rasterizationSamples = vk::SampleCountFlagBits::e1,
		.sampleShadingEnable = false
	};

	vk::PipelineColorBlendAttachmentState constexpr colorBlendAttachment {
		.blendEnable = false,
		.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
	};

	vk::PipelineColorBlendStateCreateInfo constexpr colorBlending {
		.logicOpEnable = false,
		.logicOp = vk::LogicOp::eCopy,
		.attachmentCount = 1,
		.pAttachments = &colorBlendAttachment
	};

	std::array<vk::DynamicState, 2> constexpr dynamicStates { vk::DynamicState::eViewport, vk::DynamicState::eScissor};

	vk::PipelineDynamicStateCreateInfo constexpr dynamicState {
		.dynamicStateCount = 2,
		.pDynamicStates = dynamicStates.data()
	};

	vk::PipelineRenderingCreateInfo const renderingInfo {
		.colorAttachmentCount = 1,
		.pColorAttachmentFormats = &swapchain.format
	};

	try
	{
		m_handle = vc::Get().device.createGraphicsPipeline(
			nullptr,
			vk::GraphicsPipelineCreateInfo{
				.pNext = &renderingInfo,
				.stageCount = 2,
				.pStages = shaderStages,
				.pVertexInputState = &vertexInput,
				.pInputAssemblyState = &inputAssembly,
				.pViewportState = &viewportState,
				.pRasterizationState = &rasterizer,
				.pMultisampleState = &multisampling,
				.pColorBlendState = &colorBlending,
				.pDynamicState = &dynamicState,
				.layout = pipelineLayout
			}
		).value;
	}
	catch (const vk::SystemError& e)
	{
		spdlog::critical(e.what());
	}

	vc::Get().device.destroy(fragShaderModule);
	vc::Get().device.destroy(vertShaderModule);	
}

void Pipeline::teardown()
{
	if (m_handle) vc::Get().device.destroy(m_handle);
}
