#pragma once
#include "VulkanContext.hpp"
#include "VulkanSwapchain.hpp"

class lfRenderer
{
public:
	void create()
	{
		m_vc.create();
	}

private:
	VulkanContext m_vc;
	VulkanSwapchain m_swapchain;
};

