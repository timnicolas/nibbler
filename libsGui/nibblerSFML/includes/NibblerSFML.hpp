#pragma once

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "ANibblerGui.hpp"

#define WIDTH 800
#define HEIGHT 600

#define TO_SFML_COLOR(color) ((GET_R(color) << 24) + (GET_G(color) << 16) + (GET_B(color) << 8) + 0xFF)

class NibblerSFML : public ANibblerGui {
	public:
		NibblerSFML();
		virtual ~NibblerSFML();
		NibblerSFML(NibblerSFML const &src);
		NibblerSFML &operator=(NibblerSFML const &rhs);

		virtual void	updateInput();
		virtual bool	draw(std::deque<Vec2> & snake, std::deque<Vec2> & food);

	private:
		sf::RenderWindow	_win;
		sf::Event			_event;

		virtual bool	_init();
};
