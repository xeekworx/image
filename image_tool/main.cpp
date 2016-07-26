#include <iostream>
#include <memory>
#include <SDL.h>
#include "image_sdlpreview.h"

int main(int argc, char *argv[])
{
	std::string image_path;
	uint32_t flip_type = 0;
	SDL_Event event = { 0 };
	int should_quit = 0;

	for(int argi = 1; argi < argc; ++argi) {
		std::string arg = argv[argi];
		if(arg[0] != '-') {
			if(image_path.empty()) image_path = arg;
			else {
				std::cout << "Only one image path supported" << std::endl;
				return 0;
			}
		}
		else {
			if(arg == "-f") flip_type |= xeekworx::image::flip_type::vertical;
			else if(arg == "-m") flip_type |= xeekworx::image::flip_type::horizontal;
			else {
				std::cout << "Unrecognized option: " << arg << std::endl;
				return 0;
			}
		}
	}

	if(image_path.empty()) {
		std::cout << "Please give an image path" << std::endl;
		return 0;
	}

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
		return -1;
	}

	{
		std::unique_ptr<xeekworx::image> img;
		std::unique_ptr<xeekworx::image_sdlpreview> preview;
		try {
			img = std::make_unique<xeekworx::image>(image_path);
			if(flip_type != 0) {
				img->flip(flip_type);
			}
			img->grayscale();
			preview = std::make_unique<xeekworx::image_sdlpreview>("Xeekworx Image Preview", *img);
		}
		catch(std::exception e) {
			std::cout << "Failed to open image and create preview window: " << e.what() << std::endl;
			SDL_Quit();
			return -1;
		}

		while(SDL_WaitEvent(&event) && !should_quit) {
			preview->handle_event(&event);

			switch(event.type) {
			case SDL_QUIT:
				should_quit = 1;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
				case SDLK_ESCAPE:
					should_quit = 1;
					break;
				}
				break;
			}
		}
	}

	SDL_Quit();
	return 0;
}