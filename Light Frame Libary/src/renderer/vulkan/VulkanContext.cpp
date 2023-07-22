#include "pch.hpp"
#include "VulkanContext.hpp"
#include "Instance.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

VulkanContext::VulkanContext() = default;

VulkanContext::~VulkanContext()
{

	if (instance) instance.destroy();

	spdlog::info("Destroyed vulkan context");
}

void VulkanContext::create(lfWindow& window)
{
	vi::dispatcherInit();

	instance = vi::createInstance(window.getTitle());

	vi::dispatcherLoadInstance(instance);

	spdlog::info("Created vulkan context");
}
