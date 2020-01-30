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
	if (this != &rhs) {
		std::cout << "[ERROR]: unable to copy NibblerSFML" << std::endl;
	}
	return *this;
}

bool NibblerSFML::init(GameInfo *gameInfo) {
	std::cout << "[INFO]: loading SFML" << std::endl;

	_gameInfo = gameInfo;

	_win.create(sf::VideoMode(_gameInfo->width, _gameInfo->height), _gameInfo->title);

    return true;
}

void NibblerSFML::updateInput() {
	while (_win.pollEvent(_event)) {
		switch (_event.type) {
			// window closed
			case sf::Event::Closed:
				input.quit = true;
				break;

			// key pressed
			case sf::Event::KeyPressed:
				if (_event.key.code == sf::Keyboard::Escape)
					input.quit = true;

				else if (_event.key.code == sf::Keyboard::Up)
					input.direction = ANibblerGui::Input::MOVE_UP;
				else if (_event.key.code == sf::Keyboard::Down)
					input.direction = ANibblerGui::Input::MOVE_DOWN;
				else if (_event.key.code == sf::Keyboard::Left)
					input.direction = ANibblerGui::Input::MOVE_LEFT;
				else if (_event.key.code == sf::Keyboard::Right)
					input.direction = ANibblerGui::Input::MOVE_RIGHT;


				else if (_event.key.code == sf::Keyboard::Num1)
					input.loadGuiID = 0;
				else if (_event.key.code == sf::Keyboard::Num2)
					input.loadGuiID = 1;
				else if (_event.key.code == sf::Keyboard::Num3)
					input.loadGuiID = 2;
				break;

			default:
				break;
		}
	}
}

bool NibblerSFML::draw(std::deque<Snake> &snake) {
	(void)snake;
	_win.clear();

	sf::RectangleShape rect(sf::Vector2f(10 + 100 * input.direction, 10 + 100 * (4 - input.direction)));
	rect.setPosition(100, 100);
    rect.setFillColor(sf::Color::Green);
	_win.draw(rect);


	_win.display();
	return true;
}

extern "C" {
	ANibblerGui *makeNibblerSFML() {
		return new NibblerSFML();
	}
}
