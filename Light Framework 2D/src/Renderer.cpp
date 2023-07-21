#include "pch.hpp"
#include "lf2d.hpp"

static vk::Instance instance;
static vk::DebugUtilsMessengerEXT debugMessenger;
static vk::SurfaceKHR surface;
static vk::PhysicalDevice gpu;
static vk::Device device;
static vk::Queue graphicsQueue, presentQueue;
static uint32_t graphicsFamily, presentFamily;

namespace lf2d
{
	Renderer::Renderer()
	{
		glfwInit();
	}

	Renderer::~Renderer()
	{
		glfwTerminate();
	}

	void Renderer::beginRendering()
	{
		glfwPollEvents();
	}

	void Renderer::endRendering()
	{

	}
}