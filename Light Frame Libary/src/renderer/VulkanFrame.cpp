#include "pch.hpp"
#include "VulkanFrame.hpp"
#include "VulkanContext.hpp"

void VulkanFrame::create()
{
#pragma region Commands
	commandPool = vc::Get().device.createCommandPool({ .flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer, .queueFamilyIndex = vc::Get().graphicsFamily });
	commandBuffer = vc::Get().device.allocateCommandBuffers({ .commandPool = commandPool, .level = {}, .commandBufferCount = 1 }).front();
#pragma endregion

#pragma region Synchronization Objects
	imageAvailable = vc::Get().device.createSemaphore({});
	renderFinished = vc::Get().device.createSemaphore({});
	fence = vc::Get().device.createFence({ .flags = vk::FenceCreateFlagBits::eSignaled });
#pragma endregion
}

void VulkanFrame::teardown()
{
	if (fence)			vc::Get().device.destroy(fence);
	if (renderFinished) vc::Get().device.destroy(renderFinished);
	if (imageAvailable) vc::Get().device.destroy(imageAvailable);
	if (commandPool)	vc::Get().device.destroy(commandPool);
}
