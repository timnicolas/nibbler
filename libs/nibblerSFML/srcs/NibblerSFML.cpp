#include "NibblerSFML.hpp"

NibblerSFML::NibblerSFML() :
  _win() {}

NibblerSFML::~NibblerSFML() {
	std::cout << "[INFO]: exit SFML" << std::endl;
	_win.close();
}

NibblerSFML::NibblerSFML(NibblerSFML const &src) {
	*this = src;
}

NibblerSFML &NibblerSFML::operator=(NibblerSFML const &rhs) {
	(void)rhs;
	// if (this != &rhs)
	// 	;
	return *this;
}

bool NibblerSFML::init() {
	std::cout << "[INFO]: loading SFML" << std::endl;

	_win .create(sf::VideoMode(WIDTH, HEIGHT), TITLE);

    return true;
}

void NibblerSFML::updateInput() {
	while (_win.pollEvent(_event)) {
		if (_event.type == sf::Event::Closed)
			input.quit = true;
	}
}

bool NibblerSFML::draw() {
	_win.clear();

	sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
	_win.draw(shape);


	_win.display();
	return true;
}

extern "C" {
	NibblerSFML *makeNibblerSFML() {
		return new NibblerSFML();
	}
}
