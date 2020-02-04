#pragma once

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "ANibblerGui.hpp"

#define WIDTH 800
#define HEIGHT 600

#define TO_OPENGL_COLOR(color) ((GET_R(color) << 24) + (GET_G(color) << 16) + (GET_B(color) << 8) + 0xFF)

class NibblerOpenGL : public ANibblerGui {
	public:
		NibblerOpenGL();
		virtual ~NibblerOpenGL();
		NibblerOpenGL(NibblerOpenGL const &src);
		NibblerOpenGL &operator=(NibblerOpenGL const &rhs);

		virtual void	updateInput();
		virtual bool	draw(std::deque<Vec2> & snake, std::deque<Vec2> & food);

	private:
		sf::RenderWindow	_win;
		sf::Event			_event;
		sf::Font			_font;

		virtual bool	_init();
};
