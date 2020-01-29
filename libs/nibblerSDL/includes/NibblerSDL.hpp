#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include "INibblerGui.hpp"

#define WIDTH 800
#define HEIGHT 600

class NibblerSDL : public INibblerGui {
	public:
		NibblerSDL();
		virtual ~NibblerSDL();
		NibblerSDL(NibblerSDL const &src);
		NibblerSDL &operator=(NibblerSDL const &rhs);

		virtual bool	init();
		virtual bool	draw() const;

	private:
		SDL_Window *	_win;
};

typedef NibblerSDL *(*makerNibblerSDL)();
