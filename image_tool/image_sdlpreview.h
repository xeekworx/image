#pragma once
#include <stdint.h>
#include <string>
#include <climits>
#include "../image.h"

namespace xeekworx {

	class image_sdlpreview {
	private:
		SDL_Window * m_sdlwindow;
		SDL_Surface * m_image_surface;

	public:
		image_sdlpreview(const std::string& title, const image& source, const int x = SDL_WINDOWPOS_CENTERED, const int y = SDL_WINDOWPOS_CENTERED);
		~image_sdlpreview();

		void destroy();
		bool is_destroyed() const { return m_sdlwindow == NULL; }
		uint32_t id() const;
		void handle_event(const SDL_Event * e);
	};

}