#include "NibblerOpenGL.hpp"
#include "Logging.hpp"

NibblerOpenGL::NibblerOpenGL() :
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

NibblerOpenGL::~NibblerOpenGL() {
	logInfo("exit OpenGL");
	_win.close();
}

NibblerOpenGL::NibblerOpenGL(NibblerOpenGL const &src) {
	*this = src;
}

NibblerOpenGL &NibblerOpenGL::operator=(NibblerOpenGL const &rhs) {
	if (this != &rhs) {
		logErr("unable to copy NibblerOpenGL");
	}
	return *this;
}

bool NibblerOpenGL::_init() {
	logInfo("loading OpenGL");

	_win.create(sf::VideoMode(_gameInfo->width, _gameInfo->height), _gameInfo->title + " OpenGL");

	if (!_font.loadFromFile(_gameInfo->font)) {
    	logErr("unable to load font " << _gameInfo->font);
		return false;
	}

    return true;
}

void NibblerOpenGL::updateInput() {
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

bool NibblerOpenGL::draw(std::deque<Vec2> & snake, std::deque<Vec2> & food) {
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
	rect.setFillColor(sf::Color(TO_OPENGL_COLOR(BORDER_COLOR)));
	_win.draw(rect);


	// draw board
	for (int i = 0; i < _gameInfo->boardSize; i++) {
		for (int j = 0; j < _gameInfo->boardSize; j++) {
			sf::RectangleShape rect(sf::Vector2f(step, step));
			rect.setPosition(startX + step * i, startY + step * j);
			uint32_t color = ((i + j) & 1) ? SQUARE_COLOR_1 : SQUARE_COLOR_2;
			rect.setFillColor(sf::Color(TO_OPENGL_COLOR(color)));
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
		rect.setFillColor(sf::Color(TO_OPENGL_COLOR(color)));
		_win.draw(rect);
		i++;
	}
	// draw food
	for (auto it = food.begin(); it != food.end(); it++) {
		sf::RectangleShape rect(sf::Vector2f(step, step));
		rect.setPosition(startX + step * it->x, startY + step * it->y);
		rect.setFillColor(sf::Color(TO_OPENGL_COLOR(FOOD_COLOR)));
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
		text.setFillColor(sf::Color(TO_OPENGL_COLOR(TEXT_COLOR)));

		text.setString("Score: " + std::to_string(snake.size()));
		text.setPosition(textX, textY);
		_win.draw(text);

		textY += textLnStep;
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
			text.setFillColor(sf::Color(TO_OPENGL_COLOR(TEXT_WIN_COLOR)));
			text.setString("You win !");
		}
		else if (_gameInfo->gameOver) {
			text.setFillColor(sf::Color(TO_OPENGL_COLOR(TEXT_GAMEOVER_COLOR)));
			text.setString("Game over");
		}
		else if (_gameInfo->paused) {
			text.setFillColor(sf::Color(TO_OPENGL_COLOR(TEXT_COLOR)));
			text.setString("Pause");
		}
		_win.draw(text);
	}

	_win.display();
	return true;
}

extern "C" {
	ANibblerGui *makeNibblerOpenGL() {
		return new NibblerOpenGL();
	}
}
