#pragma once

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "ANibblerGui.hpp"

#define WIDTH 800
#define HEIGHT 600

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
