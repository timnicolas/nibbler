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

		virtual bool	init();
		virtual void	updateInput();
		virtual bool	draw();

	private:
		sf::RenderWindow	_win;
		sf::Event			_event;
};
