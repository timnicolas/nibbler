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
		logErr("don't use NibblerSFML copy operator");
	}
	return *this;
}

bool NibblerSFML::_init() {
	logInfo("loading SFML");

	_win.create(sf::VideoMode(_gameInfo->width, _gameInfo->height), _gameInfo->title + " SFML");

	if (!_font.loadFromFile(_gameInfo->font)) {
    	logErr("unable to load font " << _gameInfo->font);
		return false;
	}

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

				// player 1
				else if (_event.key.code == sf::Keyboard::Up)
					input.direction[0] = Direction::MOVE_UP;
				else if (_event.key.code == sf::Keyboard::Down)
					input.direction[0] = Direction::MOVE_DOWN;
				else if (_event.key.code == sf::Keyboard::Left)
					input.direction[0] = Direction::MOVE_LEFT;
				else if (_event.key.code == sf::Keyboard::Right)
					input.direction[0] = Direction::MOVE_RIGHT;

				// player 2
				else if (_event.key.code == sf::Keyboard::W)
					input.direction[1] = Direction::MOVE_UP;
				else if (_event.key.code == sf::Keyboard::S)
					input.direction[1] = Direction::MOVE_DOWN;
				else if (_event.key.code == sf::Keyboard::A)
					input.direction[1] = Direction::MOVE_LEFT;
				else if (_event.key.code == sf::Keyboard::D)
					input.direction[1] = Direction::MOVE_RIGHT;


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

bool NibblerSFML::draw(std::vector<std::deque<Vec2>> & snakes, std::deque<Vec2> & food) {
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
	// draw snakes
	for (int id = 0; id < _gameInfo->nbPlayers; id++) {
		int		i = 0;
		float	max = (snakes[id].size() == 1) ? 1 : snakes[id].size() - 1;
		uint32_t c1 = SNAKE_1_COLOR_1;
		uint32_t c2 = SNAKE_1_COLOR_2;
		if (id % 3 == 1) {
			c1 = SNAKE_2_COLOR_1;
			c2 = SNAKE_2_COLOR_2;
		}
		if (id % 3 == 2) {
			c1 = SNAKE_3_COLOR_1;
			c2 = SNAKE_3_COLOR_2;
		}
		for (auto it = snakes[id].begin(); it != snakes[id].end(); it++) {
			sf::RectangleShape rect(sf::Vector2f(step, step));
			rect.setPosition(startX + step * it->x, startY + step * it->y);
			uint32_t color = mixColor(c1, c2, i / max);
			rect.setFillColor(sf::Color(TO_SFML_COLOR(color)));
			_win.draw(rect);
			i++;
		}
	}
	// draw food
	for (auto it = food.begin(); it != food.end(); it++) {
		sf::RectangleShape rect(sf::Vector2f(step, step));
		rect.setPosition(startX + step * it->x, startY + step * it->y);
		rect.setFillColor(sf::Color(TO_SFML_COLOR(FOOD_COLOR)));
		_win.draw(rect);
	}

    {
		// right band information
		sf::Text text;
		float textSize = _gameInfo->width / 35.0;
		float textX = size + 2 * BORDER_SIZE + 10;
		float textY = 5;
		float textLnStep = textSize * 1.2;
		text.setFont(_font);
		text.setCharacterSize(textSize);
		text.setFillColor(sf::Color(TO_SFML_COLOR(TEXT_COLOR)));

		if (_gameInfo->nbPlayers == 1) {
			text.setString("Score : " + std::to_string(_gameInfo->scores[0]));
			text.setPosition(textX, textY);
			_win.draw(text);
			textY += textLnStep;
		}
		else {
			for (int id = 0; id < _gameInfo->nbPlayers; id++) {
				uint32_t c1 = SNAKE_1_COLOR_1;
				if (id % 3 == 1)
					c1 = SNAKE_2_COLOR_1;
				if (id % 3 == 2)
					c1 = SNAKE_3_COLOR_1;
				text.setFillColor(sf::Color(TO_SFML_COLOR(c1)));
				text.setString("Score " + std::to_string(id + 1) + " : " + std::to_string(_gameInfo->scores[id]));
				text.setPosition(textX, textY);
				_win.draw(text);
				textY += textLnStep;
			}
			text.setFillColor(sf::Color(TO_SFML_COLOR(TEXT_COLOR)));
		}

		text.setString("Best: " + std::to_string(_gameInfo->bestScore));
		text.setPosition(textX, textY);
		_win.draw(text);

		textY += textLnStep;
		textY += textLnStep;
		text.setString("space: pause");
		text.setPosition(textX, textY);
		_win.draw(text);
		textY += textLnStep;
		text.setString("arrow: turn");
		text.setPosition(textX, textY);
		_win.draw(text);
		textY += textLnStep;
		text.setString("r: restart");
		text.setPosition(textX, textY);
		_win.draw(text);
		textY += textLnStep;
	}

	if (_gameInfo->win || _gameInfo->gameOver || _gameInfo->paused) {
		sf::Text text;
		float textSize = _gameInfo->width / 10.0;
		float textX = size / 3;
		float textY = _gameInfo->height / 2 - textSize;
		text.setFont(_font);
		text.setCharacterSize(textSize);
		text.setPosition(textX, textY);

		if (_gameInfo->win) {
			text.setFillColor(sf::Color(TO_SFML_COLOR(TEXT_WIN_COLOR)));
			if (_gameInfo->nbPlayers == 1)
				text.setString("You win !");
			else
				text.setString("Player " + std::to_string(_gameInfo->winnerID + 1) + " win !");
		}
		else if (_gameInfo->gameOver) {
			text.setFillColor(sf::Color(TO_SFML_COLOR(TEXT_GAMEOVER_COLOR)));
			text.setString("Game over");
		}
		else if (_gameInfo->paused) {
			text.setFillColor(sf::Color(TO_SFML_COLOR(TEXT_COLOR)));
			text.setString("Pause");
		}
		_win.draw(text);
	}

	_win.display();
	return true;
}

extern "C" {
	ANibblerGui *makeNibblerSFML() {
		return new NibblerSFML();
	}
}
