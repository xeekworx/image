#include <SDL.h>
#include "image_sdlpreview.h"
#include <iostream>

using namespace xeekworx;

image_sdlpreview::image_sdlpreview(const std::string& title, const image& source, const int x, const int y)
	: m_sdlwindow(NULL), m_image_surface(NULL)
{
	if(NULL == (m_sdlwindow = SDL_CreateWindow(
		title.c_str(), 
		x, y,
		source.width(), source.height(), 
		0))) 
	{
		throw std::exception(SDL_GetError());
	}
	else {
		SDL_SetWindowData(m_sdlwindow, "this", this);
	}

	int bpp = 0;
	uint32_t rmask = 0, gmask = 0, bmask = 0, amask = 0;
	SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ABGR8888, &bpp, &rmask, &gmask, &bmask, &amask);

	if(NULL == (m_image_surface = SDL_CreateRGBSurfaceFrom(
		(void *) source.pixels(),
		source.width(), source.height(),
		source.bits(), source.pitch(),
		rmask, gmask, bmask, amask)))
	{
		destroy();
		throw std::exception(SDL_GetError());
	}

	SDL_Surface* destination = SDL_GetWindowSurface(m_sdlwindow);
	if(destination == NULL) {
		destroy();
		throw std::exception(SDL_GetError());
	}

	SDL_BlitSurface(m_image_surface, NULL, destination, NULL);
	SDL_UpdateWindowSurface(m_sdlwindow);
}

image_sdlpreview::~image_sdlpreview()
{
	destroy();
}

void image_sdlpreview::destroy()
{
	if(m_image_surface) {
		SDL_FreeSurface(m_image_surface);
		m_image_surface = NULL;
	}

	if(m_sdlwindow) {
		SDL_SetWindowData(m_sdlwindow, "this", NULL);
		SDL_DestroyWindow(m_sdlwindow);
		m_sdlwindow = NULL;
	}
}

uint32_t image_sdlpreview::id() const
{
	return SDL_GetWindowID(m_sdlwindow);
}

void image_sdlpreview::handle_event(const SDL_Event * e)
{
	if(e && e->type == SDL_WINDOWEVENT && e->window.windowID == id()) {
		switch(e->window.event) {
		case SDL_WINDOWEVENT_CLOSE:
			destroy();
			break;
		}
	}
}