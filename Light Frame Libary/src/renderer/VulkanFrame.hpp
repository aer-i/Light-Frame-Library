#pragma once

struct VulkanFrame
{
	void create();
	void teardown();

	vk::CommandPool commandPool;
	vk::CommandBuffer commandBuffer;
	vk::Semaphore imageAvailable;
	vk::Semaphore renderFinished;
	vk::Fence fence;
};
