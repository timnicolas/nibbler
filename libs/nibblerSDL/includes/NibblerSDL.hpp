
#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "ANibblerGui.hpp"

class NibblerSDL : public ANibblerGui {
	public:
		NibblerSDL();
		virtual ~NibblerSDL();
		NibblerSDL(NibblerSDL const &src);
		NibblerSDL &operator=(NibblerSDL const &rhs);

		virtual bool	init(GameInfo *gameInfo);
		virtual void	updateInput();
		virtual bool	draw(std::deque<Snake> &snake);

	private:
		SDL_Window *	_win;
		SDL_Surface *	_surface;
		SDL_Event *		_event;
};
