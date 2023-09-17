#pragma once

struct TextureFile
{
public:
	TextureFile(std::string_view filepath, uint32_t* width, uint32_t* height)
	{
		int channels;
		m_pixels = stbi_load(filepath.data(), reinterpret_cast<int*>(width), reinterpret_cast<int*>(height), &channels, STBI_rgb_alpha);
	}

	~TextureFile()
	{
		if (m_pixels)
			stbi_image_free(m_pixels);
	}

	uint8_t* getPixels() { return m_pixels; }

private:
	uint8_t* m_pixels = nullptr;
};

class VulkanTexture
{
public:
	VulkanTexture(std::string_view filepath, bool pixelated);
	VulkanTexture(void* buffer, vk::DeviceSize bufferSize, uint32_t width, uint32_t height);

	void teardown();

	inline uint32_t getWidth() const { return m_width; }
	inline uint32_t getHeight() const { return m_height; }
	inline vk::DescriptorImageInfo getImageInfo() const { return { m_sampler, m_imageView, vk::ImageLayout::eShaderReadOnlyOptimal }; }

private:
	void createDefaultTexture();

private:
	uint32_t m_width{}, m_height{};
	size_t m_size;
	vk::Image m_image;
	vk::ImageView m_imageView;
	vk::Sampler m_sampler;
	VmaAllocation m_allocation;
};

