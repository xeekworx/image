#pragma once
#include <stdint.h>
#include <vector>
#include <memory>

namespace xeekworx {

	using byte = uint8_t; // C++11 alias

	class image {
	private:
		std::vector<byte> m_buffer;
		uint32_t m_width;
		uint32_t m_height;

		static void convert24to32(const byte * in_pixels, byte * out_pixels, const uint32_t num_pixels);

	public:
		enum class file_format { png, bmp, tga };
		enum flip_type : uint32_t { vertical = 1 << 0, horizontal = 1 << 1, };

		image(const byte * pixels, const uint32_t width, const uint32_t height, const uint32_t bits = 32);
		image(const uint32_t width, const uint32_t height);
		image(const std::string& file);
		virtual ~image();

		const byte * pixels() const { return (const byte *) m_buffer.data(); }
		const uint32_t& width() const { return m_width; }
		const uint32_t& height() const { return m_height; }
		const uint32_t bits() const { return 32U; }
		const uint32_t channels() const { return bits() / 8U; }
		const uint32_t pitch() const { return width() * channels(); }
		uint32_t length() const { return static_cast<uint32_t>(m_buffer.size()); }
		bool empty() const { return m_buffer.empty(); }
		
		void reset() { m_buffer.clear(); m_width = 0; m_height = 0; }
		void flip(const uint32_t type);
		void clear(const uint32_t color = 0x00000000);
		void resize(const uint32_t new_width, const uint32_t new_height);
		void grayscale(const bool preserve_alpha = true);

		byte& operator[](const int i) { return m_buffer[i]; }
		const byte& operator[](const int i) const { return m_buffer[i]; }

		// Supporting these formats: JPEG, PNG, BMP, PSD, TGA, GIF, HDR, PIC, PPM, and PGM
		void load(const std::string& file);
		// Supporting these formats: PNG, BMP, TGA
		void save(const std::string& file, const file_format format = file_format::png) const;

	};

	typedef std::shared_ptr<image> image_ptr;
};
