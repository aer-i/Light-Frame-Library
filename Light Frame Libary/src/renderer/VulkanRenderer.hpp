#pragma once
#include "VulkanContext.hpp"
#include "VulkanSwapchain.hpp"
#include "VulkanFrame.hpp"

namespace lf2d { struct Color; }

class lfRenderer
{
public:
	lfRenderer() = default;
	~lfRenderer();

	inline void waitIdle() { vc::Get().device.waitIdle(); }

	void clearColor(lf2d::Color const& color);
	void create();
	void beginFrame();
	void endFrame();

private:
	VulkanSwapchain m_swapchain;
	std::vector<VulkanFrame> m_frames;
	VulkanFrame* m_currentFrame;
	uint32_t m_imageIndex = 0;
	uint32_t m_frameIndex = 0;
	std::array<float, 4> m_color = {0.f, 0.f, 0.f, 1.f};

	void recreateSwapchain();
	void teardown();
};

