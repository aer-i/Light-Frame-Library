#pragma once
#include "VulkanContext.hpp"
#include "VulkanSwapchain.hpp"
#include "VulkanFrame.hpp"

class lfRenderer
{
public:
	lfRenderer() = default;
	~lfRenderer();

	inline void waitIdle() { vc::Get().device.waitIdle(); }
	void create();
	void beginFrame();
	void endFrame();

private:
	VulkanSwapchain m_swapchain;
	std::vector<VulkanFrame> m_frames;
	VulkanFrame* m_currentFrame;
	uint32_t m_imageIndex = 0;
	uint32_t m_frameIndex = 0;

	void recreateSwapchain();
	void teardown();
};

