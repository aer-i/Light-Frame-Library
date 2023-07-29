#pragma once
#include "VulkanContext.hpp"
#include "VulkanSwapchain.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanFrame.hpp"
#include "Mesh.hpp"

class lfRenderer
{
public:
	lfRenderer() = default;
	~lfRenderer();

	inline void waitIdle() { vc::Get().device.waitIdle(); }

	void clearColor(lf2d::Color color);
	void setVsync(bool enabled);

	void create(bool);
	void beginFrame();
	void endFrame(Mesh& mesh, lf2d::Camera const& camera);

private:
	VulkanSwapchain m_swapchain;
	PipelineLayout m_defaultPipelineLayout;
	Pipeline m_defaultPipeline;
	uint32_t m_imageIndex = 0;
	uint32_t m_currentFrame = 0;
	std::vector<VulkanFrame> m_frames;
	std::array<float, 4> m_color = {0.f, 0.f, 0.f, 1.f};
	bool m_vsync = true;

	void recreateSwapchain();
	void teardown();
};