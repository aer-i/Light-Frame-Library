#pragma once
#include "VulkanContext.hpp"
#include "VulkanSwapchain.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanTexturePool.hpp"
#include "VulkanFrame.hpp"
#include "Mesh.hpp"
#include "Text.hpp"

class lfRenderer
{
public:
	lfRenderer() = default;
	~lfRenderer();

	inline void waitIdle() { vc::Get().device.waitIdle(); }
	inline float getDeltaTime() const { return m_deltaTime; }

	void clearColor(lf2d::Color color);
	void setVsync(bool enabled);
	void loadTexture(std::string_view filepath, bool pixelated);
	void loadTexture(void* buffer, vk::DeviceSize bufferSize, uint32_t width, uint32_t height);
	void unloadTexture(int textureID);

	void create(bool);
	void beginFrame(lf2d::Camera* camera);
	void endFrame(Mesh& mesh);

private:
	VulkanSwapchain m_swapchain;
	PipelineLayout m_defaultPipelineLayout;
	Pipeline m_defaultPipeline;
	Pipeline m_textPipeline;
	VulkanTexturePool m_texturePool;
	uint32_t m_imageIndex = 0;
	uint32_t m_currentFrame = 0;
	std::vector<VulkanFrame> m_frames;
	std::array<float, 4> m_color = {0.f, 0.f, 0.f, 1.f};
	lf2d::Camera* m_currentCamera;
	float m_deltaTime = 0.f;
	bool m_vsync = true;

	void recreateSwapchain();
	void teardown();
};