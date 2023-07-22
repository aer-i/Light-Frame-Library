#pragma once
#include "VulkanContext.hpp"

class lfRenderer
{
public:
	void create(lfWindow& window)
	{
		m_vc.create(window);
	}

private:
	VulkanContext m_vc;
};

