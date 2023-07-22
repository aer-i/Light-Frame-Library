#pragma once
#include "VulkanContext.hpp"
#include "VulkanSwapchain.hpp"

class lfRenderer
{
public:
	lfRenderer() = default;
	~lfRenderer();

	void create()
	{
		vc::Create();
		m_swapchain.create();
	}

	void beginFrame();
	void endFrame();

private:
	VulkanSwapchain m_swapchain;

	void recreateSwapchain();
};

