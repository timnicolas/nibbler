#include "NibblerSFML.hpp"
#include "Logging.hpp"

NibblerSFML::NibblerSFML() :
  _win() {
	// init logging
	#if DEBUG
		logging.setLoglevel(LOGDEBUG);
		logging.setPrintFileLine(LOGWARN, true);
		logging.setPrintFileLine(LOGERROR, true);
		logging.setPrintFileLine(LOGFATAL, true);
	#else
		logging.setLoglevel(LOGINFO);
	#endif
}

NibblerSFML::~NibblerSFML() {
	logInfo("exit SFML");
	_win.close();
}

NibblerSFML::NibblerSFML(NibblerSFML const &src) {
	*this = src;
}

NibblerSFML &NibblerSFML::operator=(NibblerSFML const &rhs) {
	if (this != &rhs) {
		logErr("unable to copy NibblerSFML");
	}
	return *this;
}

bool NibblerSFML::_init() {
	logInfo("loading SFML");

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

				else if (_event.key.code == sf::Keyboard::Space)
					input.paused = !input.paused;
				else if (_event.key.code == sf::Keyboard::R)
					input.restart = true;

				else if (_event.key.code == sf::Keyboard::Up)
					input.direction = Direction::MOVE_UP;
				else if (_event.key.code == sf::Keyboard::Down)
					input.direction = Direction::MOVE_DOWN;
				else if (_event.key.code == sf::Keyboard::Left)
					input.direction = Direction::MOVE_LEFT;
				else if (_event.key.code == sf::Keyboard::Right)
					input.direction = Direction::MOVE_RIGHT;


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

bool NibblerSFML::draw(std::deque<Vec2> &Vec2) {
	(void)Vec2;
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
