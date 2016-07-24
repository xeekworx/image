#include "image.h"
#include <algorithm>
#include <iterator>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "stb/stb_image_resize.h"

using namespace xeekworx;

image::image(const byte * pixels, const uint32_t width, const uint32_t height, const uint32_t bits)
	: m_width(width), m_height(height)
{
	if(bits != 24 && bits != 32) {
		throw std::exception("Image can only be 24 or 32 bits per pixel");
	}

	const size_t length = this->width() * channels() * this->height();
	m_buffer.resize(length);
	if(bits == 24) {
		image::convert24to32(pixels, m_buffer.data(), this->width() * this->height());
	}
	else {
		std::memcpy(m_buffer.data(), pixels, length);
	}
}

image::image(const uint32_t width, const uint32_t height)
	: m_width(width), m_height(height)
{
	const size_t length = this->width() * channels() * this->height();
	m_buffer.resize(length);
	clear(0);
}

image::image(const std::string& file)
{
	load(file);
}

image::~image()
{

}

void image::convert24to32(const byte * in_pixels, byte * out_pixels, const uint32_t num_pixels)
{
	struct uint24 { uint8_t data[3]; };
	struct uint32 { uint8_t data[4]; };

	if(in_pixels && out_pixels) {

		for(uint32_t i = 0; i < num_pixels; ++i) {
			for(uint32_t c = 0; c < 3; ++c)
				((uint32 *) out_pixels)[i].data[c] = ((uint24 *) in_pixels)[i].data[c];
			((uint32 *) out_pixels)[i].data[3] = 255U;
		}

	}
}

void image::flip(const uint32_t type)
{
	if(!empty()) {
		const uint32_t stride = width() * channels();
		byte * row = new byte[stride];
		byte * buffer = &m_buffer[0];
		byte * low = buffer;
		byte * high = buffer + ((height() - 1) * stride);

		for(; low < high; low += stride, high -= stride) {
			if(type & flip_type::vertical) {
				std::memcpy(row, low, stride);
				std::memcpy(low, high, stride);
				std::memcpy(high, row, stride);
			}
			if(type & flip_type::horizontal) {
				std::memcpy(row, low, stride);
				std::reverse((uint32_t *) row, (uint32_t *) row + width());
				std::memcpy(low, row, stride);

				std::memcpy(row, high, stride);
				std::reverse((uint32_t *) row, (uint32_t *) row + width());
				std::memcpy(high, row, stride);
			}
		}

		delete[] row;
	}
}

void image::clear(const uint32_t color)
{
	if(!empty()) {
		std::fill_n((uint32_t *) this->pixels(), width() * height(), color);
	}
}

void image::resize(const uint32_t new_width, const uint32_t new_height)
{
	const byte * old_pixels = this->pixels();
	byte * new_pixels = new byte[new_width * channels() * new_height];
	stbir_resize_uint8(old_pixels, width(), height(), width() * channels(), new_pixels, new_width, new_height, new_width * channels(), channels());
	m_buffer.resize(new_width * channels() * new_height);
	std::memcpy(m_buffer.data(), new_pixels, new_width * channels() * new_height);
	m_width = new_width;
	m_height = new_height;
	delete[] new_pixels;
}

void image::grayscale(const bool preserve_alpha)
{
	struct pixel { uint8_t r, g, b, a; };
	pixel * buffer = (pixel *) pixels();

	for(uint32_t i = 0; i < width() * height(); ++i) {
		buffer[i].r = buffer[i].g = buffer[i].b = 0.2989*buffer[i].r + 0.5870*buffer[i].g + 0.1140*buffer[i].b;
		if(!preserve_alpha) buffer[i].a = 255;
	}
}

void image::load(const std::string& file)
{
	if(!empty()) m_buffer.clear();

	int width = 0, height = 0, comp = 0;
	stbi_uc * buffer = NULL;

	if(NULL == (buffer = stbi_load(file.c_str(), &width, &height, &comp, STBI_rgb_alpha)))
		throw std::exception(stbi_failure_reason());

	const size_t length = width * STBI_rgb_alpha * height;
	
	m_buffer.resize(length);
	std::memcpy(m_buffer.data(), buffer, length);

	this->m_width = (uint32_t) width;
	this->m_height = (uint32_t) height;

	if(buffer) stbi_image_free(buffer);
}

void image::save(const std::string& file, const file_format format) const
{
	int result = 0;
	if(!empty()) {
		switch(format) {
		case file_format::bmp:
			result = stbi_write_bmp(file.c_str(), (int) width(), (int) height(), (int) channels(), (const stbi_uc *) m_buffer.data());
			break;
		case file_format::png:
			result = stbi_write_png(file.c_str(), (int) width(), (int) height(), (int) channels(), (const stbi_uc *) m_buffer.data(), 0);
			break;
		case file_format::tga:
			result = stbi_write_tga(file.c_str(), (int) width(), (int) height(), (int) channels(), (const stbi_uc *) m_buffer.data());
			break;
		}

		if(!result) {
			throw std::exception("Failed to write image");
		}
	}
	else throw std::exception("Empty image cannot be saved");
}
