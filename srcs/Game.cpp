#include <stdlib.h>
#include "Game.hpp"
#include "nibbler.hpp"

Game::Game() :
  _dynGuiManager(),
  _gameInfo(nullptr),
  _snake(),
  _needExtend(),
  _speedMs(s.u("speedMs")) {}

bool Game::init() {
	_gameInfo = new GameInfo(s.u("nbPlayers"));
	_gameInfo->width = s.j("screen").u("width");
	_gameInfo->height = s.j("screen").u("height");
	_gameInfo->boardSize = s.u("boardSize");
	_gameInfo->minBoardSize = s.update<uint64_t>("boardSize").getMin();
	_gameInfo->maxBoardSize = s.update<uint64_t>("boardSize").getMax();
	_gameInfo->rules.canExitBorder = s.b("canExitBorder");
	_gameInfo->font = s.s("font");

	for (int i = 0; i < _gameInfo->nbPlayers; i++) {
		_snake.push_back(std::deque<Vec2>());
		_needExtend.push_back(0);
		_lastDeletedSnake.push_back(Vec2());
	}

	try {
		_dynGuiManager.loadGui(s.u("startGui"));
		if (_dynGuiManager.nibblerGui->init(_gameInfo) == false)
			return false;
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
	_food.clear();
	if (s.u("snakeSize") > userData.u("highScore")) {
		userData.u("highScore") = s.u("snakeSize");
	}
	int startY = _gameInfo->boardSize / 2;
	for (int id = 0; id < _gameInfo->nbPlayers; id++) {
		_needExtend[id] = 0;
		int startX = _gameInfo->boardSize / (_gameInfo->nbPlayers + 1) * (id + 1);
		_gameInfo->direction[id] = (id & 1) ? Direction::MOVE_UP : Direction::MOVE_DOWN;
		_snake[id].clear();
		for (int y = 0; y < static_cast<int>(s.u("snakeSize")); y++) {
			int posY = startY + ((id & 1) ? y : -y);
			_snake[id].push_back({startX, posY});
		}
	}
	_dynGuiManager.nibblerGui->input.reset();
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
			for (int id = 0; id < _gameInfo->nbPlayers; id++) {
				_move(_gameInfo->direction[id], id);
			}
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
	for (int id = 0; id < _gameInfo->nbPlayers; id++) {
		auto it = std::find(_food.begin(), _food.end(), _snake[id][0]);
		if (it != _food.end()) {  // if snake is eating
			_needExtend[id]++;
			_food.erase(it);
		}
	}

	// add food
	while (_food.size() < s.u("nbFood")) {
		for (int i = 0; i < 100; i++) {
			Vec2 newFood = {
				static_cast<int>(rand() % _gameInfo->boardSize),
				static_cast<int>(rand() % _gameInfo->boardSize),
			};
			bool ok = true;
			for (int id = 0; id < _gameInfo->nbPlayers; id++) {
				auto it = std::find(_snake[id].begin(), _snake[id].end(), newFood);
				if (it != _snake[id].end()) {  // if food is not on the snake
					ok = false;
					break;
				}
			}
			if (ok) {  // no snakes on the food
				_food.push_back(newFood);
				break;
			}
		}
	}
}

void Game::_move(Direction::Enum direction, int id) {
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

	if (_snake[id].size() > 0) {
		Vec2 newVec2(_snake[id][0].x + addX, _snake[id][0].y + addY);
		if (_gameInfo->rules.canExitBorder) {
			if (newVec2.x < 0) newVec2.x = _gameInfo->boardSize - 1;
			else if (newVec2.x >= _gameInfo->boardSize) newVec2.x = 0;
			if (newVec2.y < 0) newVec2.y = _gameInfo->boardSize - 1;
			else if (newVec2.y >= _gameInfo->boardSize) newVec2.y = 0;
		}
		_snake[id].push_front(newVec2);
		if (_needExtend[id] > 0) {
			_needExtend[id]--;
			if (_snake[id].size() > userData.u("highScore")) {
				userData.u("highScore") = _snake[id].size();
			}
		}
		else {
			_lastDeletedSnake[id] = _snake[id].back();
			_snake[id].pop_back();
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
		if (_dynGuiManager.nibblerGui->init(_gameInfo) == false)
			throw GameException("unable to load GUI");

		_dynGuiManager.nibblerGui->input.loadGuiID = NO_GUI_LOADED;
	}

	if (_gameInfo->nbPlayers == 1) {
		_updateSinglePlayer();
	}
	else {
		_updateMultiPlayer();
	}

	// update paused mode
	if (_gameInfo->win || _gameInfo->gameOver) {
		_gameInfo->paused = true;
	}
	else {
		_gameInfo->paused = _dynGuiManager.nibblerGui->input.paused;
	}

	// update direction
	for (int id = 0; id < _gameInfo->nbPlayers; id++) {
		if (_snake[id].size() == 0)
			continue;
		if (_gameInfo->direction[id] != _dynGuiManager.nibblerGui->input.direction[id]) {
			if (_snake[id].size() <= 1) {
				_gameInfo->direction[id] = _dynGuiManager.nibblerGui->input.direction[id];
			}
			else {
				Vec2 direction(_snake[id][0].x - _snake[id][1].x, _snake[id][0].y - _snake[id][1].y);
				if (direction.x > 1) direction.x = -1;
				else if (direction.x < -1) direction.x = 1;
				if (direction.y > 1) direction.y = -1;
				else if (direction.y < -1) direction.y = 1;

				if (_dynGuiManager.nibblerGui->input.direction[id] == Direction::MOVE_UP && direction.y != 1)
					_gameInfo->direction[id] = _dynGuiManager.nibblerGui->input.direction[id];
				else if (_dynGuiManager.nibblerGui->input.direction[id] == Direction::MOVE_DOWN && direction.y != -1)
					_gameInfo->direction[id] = _dynGuiManager.nibblerGui->input.direction[id];
				else if (_dynGuiManager.nibblerGui->input.direction[id] == Direction::MOVE_LEFT && direction.x != 1)
					_gameInfo->direction[id] = _dynGuiManager.nibblerGui->input.direction[id];
				else if (_dynGuiManager.nibblerGui->input.direction[id] == Direction::MOVE_RIGHT && direction.x != -1)
					_gameInfo->direction[id] = _dynGuiManager.nibblerGui->input.direction[id];
			}
		}
	}

	_gameInfo->bestScore = userData.u("highScore");
}

void Game::_updateSinglePlayer() {
	int id = 0;
	// update win
	if (_snake[id].size() >= _gameInfo->boardSize * _gameInfo->boardSize) {
		_gameInfo->win = true;
	}
	// update gameOver
	if (_gameInfo->gameOver == false) {
		if (_snake[id].size() == 0) {
			_gameInfo->gameOver = true;
		}
		else if (_snake[id][0].x < 0 || _snake[id][0].x >= _gameInfo->boardSize
		|| _snake[id][0].y < 0 || _snake[id][0].y >= _gameInfo->boardSize) {
			_gameInfo->gameOver = true;
		}
		auto it = std::find(++_snake[id].begin(), _snake[id].end(), _snake[id][0]);
		if (it != _snake[id].end()) {
			_gameInfo->gameOver = true;
		}
		if (_gameInfo->gameOver) {
			_snake[id].push_back(_lastDeletedSnake[id]);
			_snake[id].pop_front();
		}
	}
}

void Game::_updateMultiPlayer() {
	if (_gameInfo->gameOver || _gameInfo->win)
		return;

	// update win
	uint32_t allSnakesSize = 0;
	uint32_t biggerSnakeSize = 0;
	uint32_t biggerSnakeID = 0;
	int nbSnakes = 0;
	for (int id = 0; id < _gameInfo->nbPlayers; id++) {
		allSnakesSize += _snake[id].size();
		if (_snake[id].size() > biggerSnakeSize) {
			biggerSnakeSize = _snake[id].size();
			biggerSnakeID = id;
		}
		if (_snake[id].size() > 0)
			nbSnakes++;
	}
	if (nbSnakes == 1 || allSnakesSize >= _gameInfo->boardSize * _gameInfo->boardSize) {
		_gameInfo->win = true;
		_gameInfo->winnerID = biggerSnakeID;
		return;
	}

	// update gameOver
	int allDie = true;
	for (int id = 0; id < _gameInfo->nbPlayers; id++) {
		if (_snake[id].size() > 0) {
			allDie = false;
			break;
		}
	}
	if (allDie) {
		_gameInfo->gameOver = true;
		return;
	}

	// update snake die
	for (int id = 0; id < _gameInfo->nbPlayers; id++) {
		if (_snake[id].size() == 0)
			continue;
		if (_snake[id][0].x < 0 || _snake[id][0].x >= _gameInfo->boardSize
		|| _snake[id][0].y < 0 || _snake[id][0].y >= _gameInfo->boardSize) {
			_snake[id].clear();
		}
		auto it = std::find(++_snake[id].begin(), _snake[id].end(), _snake[id][0]);
		if (it != _snake[id].end()) {
			_snake[id].clear();
		}
		for (int id2 = 0; id2 < _gameInfo->nbPlayers; id2++) {
			if (id == id2)
				continue;
			auto it = std::find(_snake[id2].begin(), _snake[id2].end(), _snake[id][0]);
			if (it != _snake[id2].end()) {
				_snake[id].clear();
			}
		}
	}
}

// -- Exceptions errors --------------------------------------------------------
Game::GameException::GameException()
: std::runtime_error("[GameException]") {}

Game::GameException::GameException(const char* what_arg)
: std::runtime_error(std::string(std::string("[GameException] ") + what_arg).c_str()) {}
