#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "ANibblerGui.hpp"

class NibblerOpenGL : public ANibblerGui {
	public:
		NibblerOpenGL();
		virtual ~NibblerOpenGL();
		NibblerOpenGL(NibblerOpenGL const &src);
		NibblerOpenGL &operator=(NibblerOpenGL const &rhs);

		virtual void	updateInput();
		virtual bool	draw(std::deque<Vec2> & snake, std::deque<Vec2> & food);

	private:
		SDL_Window *	_win;
		SDL_Surface *	_surface;
		SDL_Event *		_event;

		virtual bool	_init();
};
