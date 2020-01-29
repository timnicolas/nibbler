#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "ANibblerGui.hpp"

class NibblerSDL : public ANibblerGui {
	public:
		NibblerSDL();
		virtual ~NibblerSDL();
		NibblerSDL(NibblerSDL const &src);
		NibblerSDL &operator=(NibblerSDL const &rhs);

		virtual bool	init();
		virtual void	updateInput();
		virtual bool	draw();

	private:
		SDL_Window *	_win;
		SDL_Surface *	_surface;
		SDL_Event *		_event;
};
