#include <stdlib.h>
#include "Game.hpp"
#include "nibbler.hpp"

Game::Game() :
  _dynGuiManager(),
  _gameInfo(nullptr),
  _snake(),
  _speedMs(s.u("speedMs")),
  _needExtend(0) {}

bool Game::init() {
	_gameInfo = new GameInfo();
	_gameInfo->width = s.j("screen").u("width");
	_gameInfo->height = s.j("screen").u("height");
	_gameInfo->boardSize = s.u("boardSize");
	_gameInfo->rules.canExitBorder = s.b("canExitBorder");
	try {
		_dynGuiManager.loadGui(s.u("startGui"));
		_dynGuiManager.nibblerGui->init(_gameInfo);
	}
	catch(DynGuiManager::DynGuiManagerException const & e) {
		logErr(e.what());
		return false;
	}

	restart();
	return true;
}

void Game::restart() {
	_gameInfo->restart();
	_dynGuiManager.nibblerGui->input.reset();
	_snake.clear();
	_food.clear();
	_needExtend = 0;
	int startX = _gameInfo->boardSize / 2;
	int startY = startX;
	if (s.u("snakeSize") > userData.u("highScore")) {
		userData.u("highScore") = s.u("snakeSize");
	}
	for (int y = 0; y < static_cast<int>(s.u("snakeSize")); y++) {
		_snake.push_back({startX, startY + y});
	}
}

Game::Game(Game const &src) {
	*this = src;
}

Game::~Game() {
	delete _gameInfo;
}

Game &Game::operator=(Game const &rhs) {
	(void)rhs;
	// if (this != &rhs) {}
	return *this;
}

void Game::run() {
	float						loopTime = 1000 / s.j("screen").u("fps");
	std::chrono::milliseconds	time_start;
	uint32_t					lastMoveTime = 0;
	#if DEBUG_FPS_LOW == true
		bool firstLoop = true;
	#endif

	while (_dynGuiManager.nibblerGui->input.quit == false) {
		time_start = getMs();

		_dynGuiManager.nibblerGui->updateInput();


		// move snake
		uint32_t now = getMs().count();
		if (_gameInfo->paused == false && now - lastMoveTime > _speedMs) {
			_move(_gameInfo->direction);
			lastMoveTime = now;
		}

		// update game
		_updateFood();
		_update();

		// draw on screen
		_dynGuiManager.nibblerGui->draw(_snake, _food);

		// fps
		std::chrono::milliseconds time_loop = getMs() - time_start;
		if (time_loop.count() > loopTime) {
			#if DEBUG_FPS_LOW == true
				if (!firstLoop)
					logDebug("update loop slow -> " << time_loop.count() << "ms / " << loopTime << "ms ("
					<< s.j("screen").u("fps") << "fps)");
			#endif
		}
		else {
			usleep((loopTime - time_loop.count()) * 1000);
		}
		#if DEBUG_FPS_LOW == true
			firstLoop = false;
		#endif
	}
}

void Game::_updateFood() {
	// check snake eating
	auto it = std::find(_food.begin(), _food.end(), _snake[0]);
	if (it != _food.end()) {  // if snake is eating
		_needExtend++;
		_food.erase(it);
	}

	// add food
	if (_food.size() == 0) {
		for (int i = 0; i < 100; i++) {
			Vec2 newFood = {
				static_cast<int>(rand() % _gameInfo->boardSize),
				static_cast<int>(rand() % _gameInfo->boardSize),
			};
			auto it = std::find(_snake.begin(), _snake.end(), newFood);
			if (it == _snake.end()) {  // if food is not on the snake
				_food.push_back(newFood);
				break;
			}
		}
	}
}

void Game::_move(Direction::Enum direction) {
	int addX = 0;
	int addY = 0;
	if (direction == Direction::MOVE_UP)
		addY--;
	else if (direction == Direction::MOVE_DOWN)
		addY++;
	else if (direction == Direction::MOVE_LEFT)
		addX--;
	else if (direction == Direction::MOVE_RIGHT)
		addX++;

	if (_snake.size() > 0) {
		Vec2 newVec2(_snake[0].x + addX, _snake[0].y + addY);
		if (_gameInfo->rules.canExitBorder) {
			if (newVec2.x < 0) newVec2.x = _gameInfo->boardSize - 1;
			else if (newVec2.x >= _gameInfo->boardSize) newVec2.x = 0;
			if (newVec2.y < 0) newVec2.y = _gameInfo->boardSize - 1;
			else if (newVec2.y >= _gameInfo->boardSize) newVec2.y = 0;
		}
		_snake.push_front(newVec2);
		if (_needExtend > 0) {
			_needExtend--;
			if (_snake.size() > userData.u("highScore")) {
				userData.u("highScore") = _snake.size();
			}
		}
		else {
			_lastDeletedSnake = _snake.back();
			_snake.pop_back();
		}
	}
}

void Game::_update() {
	// restart
	if (_dynGuiManager.nibblerGui->input.restart == true) {
		_dynGuiManager.nibblerGui->input.restart = false;
		restart();
		return;
	}

	// change GUI
	if (_dynGuiManager.nibblerGui->input.loadGuiID < NB_GUI && \
	_dynGuiManager.nibblerGui->input.loadGuiID != _dynGuiManager.getCurrentGuiID()) {
		// change Gui
		_gameInfo->paused = true;
		_dynGuiManager.loadGui(_dynGuiManager.nibblerGui->input.loadGuiID);
		_dynGuiManager.nibblerGui->init(_gameInfo);

		_dynGuiManager.nibblerGui->input.loadGuiID = NO_GUI_LOADED;
	}

	// update win
	if (_snake.size() >= _gameInfo->boardSize * _gameInfo->boardSize) {
		_gameInfo->win = true;
	}
	// update gameOver
	if (_gameInfo->gameOver == false) {
		if (_snake.size() == 0) {
			_gameInfo->gameOver = true;
		}
		else if (_snake[0].x < 0 || _snake[0].x >= _gameInfo->boardSize
		|| _snake[0].y < 0 || _snake[0].y >= _gameInfo->boardSize) {
			_gameInfo->gameOver = true;
		}
		auto it = std::find(++_snake.begin(), _snake.end(), _snake[0]);
		if (it != _snake.end()) {
			_gameInfo->gameOver = true;
		}
		if (_gameInfo->gameOver) {
			_snake.push_back(_lastDeletedSnake);
			_snake.pop_front();
		}
	}


	// update paused mode
	if (_gameInfo->win || _gameInfo->gameOver) {
		_gameInfo->paused = true;
	}
	else {
		_gameInfo->paused = _dynGuiManager.nibblerGui->input.paused;
	}

	// update direction
	if (_gameInfo->direction != _dynGuiManager.nibblerGui->input.direction) {
		if (_snake.size() <= 1) {
			_gameInfo->direction = _dynGuiManager.nibblerGui->input.direction;
		}
		else {
			Vec2 direction(_snake[0].x - _snake[1].x, _snake[0].y - _snake[1].y);
			if (direction.x > 1) direction.x = -1;
			else if (direction.x < -1) direction.x = 1;
			if (direction.y > 1) direction.y = -1;
			else if (direction.y < -1) direction.y = 1;

			if (_dynGuiManager.nibblerGui->input.direction == Direction::MOVE_UP && direction.y != 1)
				_gameInfo->direction = _dynGuiManager.nibblerGui->input.direction;
			else if (_dynGuiManager.nibblerGui->input.direction == Direction::MOVE_DOWN && direction.y != -1)
				_gameInfo->direction = _dynGuiManager.nibblerGui->input.direction;
			else if (_dynGuiManager.nibblerGui->input.direction == Direction::MOVE_LEFT && direction.x != 1)
				_gameInfo->direction = _dynGuiManager.nibblerGui->input.direction;
			else if (_dynGuiManager.nibblerGui->input.direction == Direction::MOVE_RIGHT && direction.x != -1)
				_gameInfo->direction = _dynGuiManager.nibblerGui->input.direction;
		}
	}
}

// -- Exceptions errors --------------------------------------------------------
Game::GameException::GameException()
: std::runtime_error("[GameException]") {}

Game::GameException::GameException(const char* what_arg)
: std::runtime_error(std::string(std::string("[GameException] ") + what_arg).c_str()) {}
