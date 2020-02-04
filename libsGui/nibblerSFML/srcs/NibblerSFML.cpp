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

bool NibblerSFML::draw(std::deque<Vec2> & snake, std::deque<Vec2> & food) {
	// clear screen
	_win.clear();

	// set the size of the square
	float startX = BORDER_SIZE;
	float startY = BORDER_SIZE;
	float size = _gameInfo->height - (2 * BORDER_SIZE);
	float step = size / _gameInfo->boardSize;

	// border
	sf::RectangleShape rect(sf::Vector2f(size + (2 * BORDER_SIZE), size + (2 * BORDER_SIZE)));
	rect.setPosition(startX - BORDER_SIZE, startY - BORDER_SIZE);
	rect.setFillColor(sf::Color(TO_SFML_COLOR(BORDER_COLOR)));
	_win.draw(rect);


	// draw board
	for (int i = 0; i < _gameInfo->boardSize; i++) {
		for (int j = 0; j < _gameInfo->boardSize; j++) {
			sf::RectangleShape rect(sf::Vector2f(step, step));
			rect.setPosition(startX + step * i, startY + step * j);
			uint32_t color = ((i + j) & 1) ? SQUARE_COLOR_1 : SQUARE_COLOR_2;
			rect.setFillColor(sf::Color(TO_SFML_COLOR(color)));
			_win.draw(rect);
		}
	}
	// draw snake
	int		i = 0;
	float	max = (snake.size() == 1) ? 1 : snake.size() - 1;
	for (auto it = snake.begin(); it != snake.end(); it++) {
		sf::RectangleShape rect(sf::Vector2f(step, step));
		rect.setPosition(startX + step * it->x, startY + step * it->y);
		uint32_t color = mixColor(SNAKE_COLOR_1, SNAKE_COLOR_2, i / max);
		rect.setFillColor(sf::Color(TO_SFML_COLOR(color)));
		_win.draw(rect);
		i++;
	}
	// draw food
	for (auto it = food.begin(); it != food.end(); it++) {
		sf::RectangleShape rect(sf::Vector2f(step, step));
		rect.setPosition(startX + step * it->x, startY + step * it->y);
		rect.setFillColor(sf::Color(TO_SFML_COLOR(FOOD_COLOR)));
		_win.draw(rect);
	}

	_win.display();
	return true;
}

extern "C" {
	ANibblerGui *makeNibblerSFML() {
		return new NibblerSFML();
	}
}
