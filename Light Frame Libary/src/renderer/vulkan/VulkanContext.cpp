#include "pch.hpp"
#include "VulkanContext.hpp"
#include "Instance.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

VulkanContext::VulkanContext()
{
	instance = vi::createInstance("Geam");
}

VulkanContext::~VulkanContext()
{
	instance.destroy();
}
