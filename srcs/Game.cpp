#include <stdlib.h>
#include "Game.hpp"
#include "nibbler.hpp"

Game::Game() :
  dynSoundManager(),
  dynGuiManager(),
  _gameInfo(nullptr),
  _needExtend(),
  _speedMs(s.u("speedMs")) {}

bool Game::init() {
	_gameInfo = new GameInfo(s.u("nbPlayers") + s.j("ai").u("nbAI"));
	_gameInfo->width = s.j("screen").u("width");
	_gameInfo->height = s.j("screen").u("height");
	_gameInfo->boardSize = s.u("boardSize");
	_gameInfo->minBoardSize = s.update<uint64_t>("boardSize").getMin();
	_gameInfo->maxBoardSize = s.update<uint64_t>("boardSize").getMax();
	_gameInfo->rules.canExitBorder = s.b("canExitBorder");
	_gameInfo->font = s.s("font");

	for (int i = 0; i < _gameInfo->nbPlayers; i++) {
		_needExtend.push_back(0);
		_lastDeletedSnake.push_back(Vec2());
		if (i >= static_cast<int>(s.u("nbPlayers"))) {
			_gameInfo->isIA[i] = true;
		}
	}

	try {
		// this will load GUI et SOUND
		_changeGui(s.u("startGui"), s.u("startSound"));
	}
	catch(DynManager<ANibblerGui>::DynManagerException const & e) {
		logErr(e.what());
		return false;
	}
	catch(DynManager<ANibblerSound>::DynManagerException const & e) {
		logErr(e.what());
		return false;
	}
	catch(GameException const & e) {
		logErr(e.what());
		return false;
	}

	restart();
	return true;
}

void Game::restart() {
	_gameInfo->restart();
	_gameInfo->paused = s.b("pauseOnStart");
	_speedMs = s.u("speedMs");
	if (s.u("snakeSize") > userData.u("highScore")) {
		userData.u("highScore") = s.u("snakeSize");
	}
	int startY = _gameInfo->boardSize / 2;
	for (int id = 0; id < _gameInfo->nbPlayers; id++) {
		_needExtend[id] = 0;
		int startX = static_cast<float>(_gameInfo->boardSize) / (_gameInfo->nbPlayers + 1) * (id + 1);
		_gameInfo->direction[id] = (id & 1) ? Direction::MOVE_UP : Direction::MOVE_DOWN;
		_gameInfo->snakes[id].clear();
		_gameInfo->nbBonus[id] = 0;
		for (int y = 0; y < static_cast<int>(s.u("snakeSize")); y++) {
			int posY = startY + ((id & 1) ? y : -y);
			_gameInfo->snakes[id].push_back({startX, posY});
		}
	}
	dynGuiManager.obj->input.reset();
	dynGuiManager.obj->input.paused = _gameInfo->paused;
	dynSoundManager.obj->stopAllSounds();
	dynSoundManager.obj->restart();
}

Game::Game(Game const &src) {
	*this = src;
}

Game::~Game() {
	delete _gameInfo;
	dynGuiManager.unload();
	dynSoundManager.unload();
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
	uint32_t					nbMoves = 0;
	#if DEBUG_FPS_LOW == true
		bool firstLoop = true;
	#endif
	while (dynGuiManager.obj->input.quit == false) {
		time_start = getMs();

		dynGuiManager.obj->updateInput();

		// move snake
		uint32_t now = getMs().count();
		if (_gameInfo->paused == false && now - lastMoveTime > _speedMs) {
			for (int id = 0; id < _gameInfo->nbPlayers; id++) {
				if (_gameInfo->isIA[id])
					_moveIA(_gameInfo->direction[id], id);
				else
					_move(_gameInfo->direction[id], id);
			}
			_updateWall();
			nbMoves++;
			if (s.i("increasingSpeedStep") != -1 && nbMoves % s.i("increasingSpeedStep") == 0) {
				if (_speedMs > s.u("maxSpeedMs"))
					_speedMs--;
			}
			lastMoveTime = now;
		}

		// update game
		_updateFood();
		_updateBonus();
		_update();

		// draw on screen
		dynGuiManager.obj->draw();

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
		auto it = std::find(_gameInfo->food.begin(), _gameInfo->food.end(), _gameInfo->snakes[id][0]);
		if (it != _gameInfo->food.end()) {  // if snake is eating
			_needExtend[id]++;
			_gameInfo->food.erase(it);
		}
	}

	// add food
	while (_gameInfo->food.size() < s.u("nbFood")) {
		for (int i = 0; i < 100; i++) {
			Vec2 newFood = {
				static_cast<int>(rand() % _gameInfo->boardSize),
				static_cast<int>(rand() % _gameInfo->boardSize),
			};
			bool ok = true;
			for (int id = 0; id < _gameInfo->nbPlayers; id++) {
				auto it = std::find(_gameInfo->snakes[id].begin(), _gameInfo->snakes[id].end(), newFood);
				if (it != _gameInfo->snakes[id].end()) {  // if food is not on the snake
					ok = false;
					break;
				}
			}
			auto it = std::find(_gameInfo->bonus.begin(), _gameInfo->bonus.end(), newFood);
			if (it != _gameInfo->bonus.end()) {
				ok = false;
				break;
			}
			for (auto it = _gameInfo->wall.begin(); it != _gameInfo->wall.end(); it++) {
				if (it->pos == newFood) {
					ok = false;
					break;
				}
			}
			if (ok) {  // no snakes on the food
				_gameInfo->food.push_back(newFood);
				break;
			}
		}
	}
}

void Game::_updateBonus() {
	if (_gameInfo->nbPlayers == 1)
		return;

	// check snake get bonus
	for (int id = 0; id < _gameInfo->nbPlayers; id++) {
		auto it = std::find(_gameInfo->bonus.begin(), _gameInfo->bonus.end(), _gameInfo->snakes[id][0]);
		if (it != _gameInfo->bonus.end()) {  // if snake is gettting a bonus
			_gameInfo->nbBonus[id]++;
			_gameInfo->bonus.erase(it);
		}
	}

	// add bonus
	while (_gameInfo->bonus.size() < s.u("nbBonus")) {
		for (int i = 0; i < 100; i++) {
			Vec2 newBonus = {
				static_cast<int>(rand() % _gameInfo->boardSize),
				static_cast<int>(rand() % _gameInfo->boardSize),
			};
			bool ok = true;
			for (int id = 0; id < _gameInfo->nbPlayers; id++) {
				auto it = std::find(_gameInfo->snakes[id].begin(), _gameInfo->snakes[id].end(), newBonus);
				if (it != _gameInfo->snakes[id].end()) {  // if bonus is not on the snake
					ok = false;
					break;
				}
			}
			auto it = std::find(_gameInfo->food.begin(), _gameInfo->food.end(), newBonus);
			if (it != _gameInfo->food.end()) {
				ok = false;
				break;
			}
			for (auto it = _gameInfo->wall.begin(); it != _gameInfo->wall.end(); it++) {
				if (it->pos == newBonus) {
					ok = false;
					break;
				}
			}
			if (ok) {  // no snakes on the bonus
				_gameInfo->bonus.push_back(newBonus);
				break;
			}
		}
	}
}

void Game::_updateWall() {
	for (auto it = _gameInfo->wall.begin(); it != _gameInfo->wall.end(); it++) {
		if (it->life > 0) {
			it->life--;
		}
		if (it->life == 0) {
			_gameInfo->wall.erase(it);
		}
	}
}

/*
check the 4 positions around the snake's head:
- if there is a food -> go to the food (update direction)
- create a possibleDir array to tell witch positions are available
then, change direction if:
- there is a food
- there is an obstacle forward
- randomly (~ every aichangeDirProba)
go to a direction without obstacle or in a random direction (~ every aiStrength)
*/
void Game::_moveIA(Direction::Enum lastDir, int id) {
	Vec2	forward;
	bool	isFood = false;
	int		foodDir;
	bool	possibleDir[4] = {true, true, true, true};  // UP | DOWN | LEFT | RIGHT
	// check all possible directions for the IA
	int i = 0;
	for (int addY = -1; addY <= 1; addY += 2) {  // -1 1
		if (isFood)
			break;
		int addX = 0;
		forward = Vec2(_gameInfo->snakes[id][0].x + addX, _gameInfo->snakes[id][0].y + addY);

		if (_gameInfo->rules.canExitBorder == false
		&& (forward.x < 0 || forward.x >= _gameInfo->boardSize || forward.y < 0 || forward.y >= _gameInfo->boardSize)) {
			possibleDir[i] = false;
		}
		else {
			// snake
			for (int id2 = 0; id2 < _gameInfo->nbPlayers; id2++) {
				auto it = std::find(_gameInfo->snakes[id2].begin(), _gameInfo->snakes[id2].end(), forward);
				if (it != _gameInfo->snakes[id2].end()) {
					possibleDir[i] = false;
				}
			}
			// wall
			for (auto it = _gameInfo->wall.begin(); it != _gameInfo->wall.end(); it++) {
				if (it->pos == forward) {
					possibleDir[i] = false;
				}
			}
		}
		// check for food
		auto it = std::find(_gameInfo->food.begin(), _gameInfo->food.end(), forward);
		if (it != _gameInfo->food.end()) {
			isFood = true;
			foodDir = i;
		}
		i++;
	}
	for (int addX = -1; addX <= 1; addX += 2) {  // -1 1
		if (isFood)
			break;
		int addY = 0;
		forward = Vec2(_gameInfo->snakes[id][0].x + addX, _gameInfo->snakes[id][0].y + addY);

		if (_gameInfo->rules.canExitBorder == false
		&& (forward.x < 0 || forward.x >= _gameInfo->boardSize || forward.y < 0 || forward.y >= _gameInfo->boardSize)) {
			possibleDir[i] = false;
		}
		else {
			// snake
			for (int id2 = 0; id2 < _gameInfo->nbPlayers; id2++) {
				auto it = std::find(_gameInfo->snakes[id2].begin(), _gameInfo->snakes[id2].end(), forward);
				if (it != _gameInfo->snakes[id2].end()) {
					possibleDir[i] = false;
				}
			}
			// wall
			for (auto it = _gameInfo->wall.begin(); it != _gameInfo->wall.end(); it++) {
				if (it->pos == forward) {
					possibleDir[i] = false;
				}
			}
		}
		// check for food or bonus
		auto it = std::find(_gameInfo->food.begin(), _gameInfo->food.end(), forward);
		if (it != _gameInfo->food.end()) {
			isFood = true;
			foodDir = i;
		}
		auto it2 = std::find(_gameInfo->bonus.begin(), _gameInfo->bonus.end(), forward);
		if (it2 != _gameInfo->bonus.end()) {
			isFood = true;
			foodDir = i;
		}
		i++;
	}
	// print possibles directions
	// for (int i = 0; i < 4; i++) { std::cout << std::boolalpha << possibleDir[i] << " "; } std::cout << std::endl;
	Direction::Enum dir = lastDir;
	if (isFood)
		dir = static_cast<Direction::Enum>(foodDir);
	else if (possibleDir[lastDir] == false || rand() % s.j("ai").u("changeDirProba") == 0) {
		int order[4] = {0, 1, 2, 3};
		for (int i = 0; i < 30; i++) {
			int id1 = rand() % 4;
			int id2 = rand() % 4;
			int tmp = order[id1];
			order[id1] = order[id2];
			order[id2] = tmp;
		}
		for (int i = 0; i < 4; i++) {
			if ((possibleDir[order[i]] && i != lastDir) || rand() % s.j("ai").u("strength") == 0) {
				dir = static_cast<Direction::Enum>(order[i]);
				break;
			}
		}
	}
	// if the direction is the inverse of the last direction
	if ((lastDir == Direction::MOVE_UP && dir == Direction::MOVE_DOWN)
	|| (lastDir == Direction::MOVE_DOWN && dir == Direction::MOVE_UP)
	|| (lastDir == Direction::MOVE_LEFT && dir == Direction::MOVE_RIGHT)
	|| (lastDir == Direction::MOVE_RIGHT && dir == Direction::MOVE_LEFT))
		dir = lastDir;  // keep last direction
	_gameInfo->direction[id] = dir;
	_move(_gameInfo->direction[id], id);
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

	if (_gameInfo->snakes[id].size() > 0) {
		Vec2 newVec2(_gameInfo->snakes[id][0].x + addX, _gameInfo->snakes[id][0].y + addY);
		if (_gameInfo->rules.canExitBorder) {
			if (newVec2.x < 0) newVec2.x = _gameInfo->boardSize - 1;
			else if (newVec2.x >= _gameInfo->boardSize) newVec2.x = 0;
			if (newVec2.y < 0) newVec2.y = _gameInfo->boardSize - 1;
			else if (newVec2.y >= _gameInfo->boardSize) newVec2.y = 0;
		}
		_gameInfo->snakes[id].push_front(newVec2);
		if (_needExtend[id] > 0) {
			_needExtend[id]--;
			if (_gameInfo->snakes[id].size() > userData.u("highScore")) {
				userData.u("highScore") = _gameInfo->snakes[id].size();
			}
		}
		else {
			_lastDeletedSnake[id] = _gameInfo->snakes[id].back();
			_gameInfo->snakes[id].pop_back();
			if (dynGuiManager.obj->input.usingBonus[id] && _gameInfo->nbBonus[id] > 0) {
				_gameInfo->nbBonus[id]--;
				_gameInfo->wall.push_back({_lastDeletedSnake[id], static_cast<int>(s.i("wallLife"))});
			}
		}
	}
}

void Game::_changeGui(int guiID, int soundID) {
	_gameInfo->paused = true;

	dynSoundManager.unload();
	dynGuiManager.unload();

	dynSoundManager.load(soundID);
	if (dynSoundManager.obj->init() == false)
		throw GameException("unable to load Sound");

	if (dynSoundManager.obj->loadMusic("masterMusic", s.s("masterMusic"), s.u("musicLevel")) == false)
		throw GameException("unable to load Sound");
	if (dynSoundManager.obj->loadSound("win", s.s("soundWin"), s.u("soundLevel")) == false)
		throw GameException("unable to load Sound");
	if (dynSoundManager.obj->loadSound("loose", s.s("soundLoose"), s.u("soundLevel")) == false)
		throw GameException("unable to load Sound");
	dynSoundManager.obj->playMusic("masterMusic");
	dynSoundManager.obj->restart();

	dynGuiManager.load(guiID);
	if (dynGuiManager.obj->init(_gameInfo) == false)
		throw GameException("unable to load GUI");

	dynGuiManager.obj->input.loadGuiID = NO_GUI_LOADED;
}

void Game::_update() {
	// restart
	if (dynGuiManager.obj->input.restart == true) {
		dynGuiManager.obj->input.restart = false;
		restart();
		return;
	}

	// change GUI
	if (dynGuiManager.obj->input.loadGuiID < dynGuiManager.getNbDyn() && \
	dynGuiManager.obj->input.loadGuiID != dynGuiManager.getCurrentID()) {
		_changeGui(dynGuiManager.obj->input.loadGuiID, s.u("startSound"));
	}

	bool lastGameOver = _gameInfo->gameOver;
	bool lastWin = _gameInfo->win;
	if (_gameInfo->nbPlayers == 1) {
		_updateSinglePlayer();
	}
	else {
		_updateMultiPlayer();
	}
	if (lastGameOver == false && _gameInfo->gameOver) {
		dynSoundManager.obj->pause(true);
		dynSoundManager.obj->playSound("loose");
	}
	else if (lastWin == false && _gameInfo->win) {
		dynSoundManager.obj->pause(true);
		dynSoundManager.obj->playSound("win");
	}

	// update scores
	for (int id = 0; id < _gameInfo->nbPlayers; id++) {
		if (_gameInfo->snakes[id].size() > 0)
			_gameInfo->scores[id] = _gameInfo->snakes[id].size();
	}

	// update paused mode
	if (_gameInfo->win || _gameInfo->gameOver) {
		_gameInfo->paused = true;
	}
	else {
		_gameInfo->paused = dynGuiManager.obj->input.paused;
	}

	// update direction
	for (int id = 0; id < _gameInfo->nbPlayers; id++) {
		if (_gameInfo->snakes[id].size() == 0 || _gameInfo->isIA[id])
			continue;
		if (_gameInfo->direction[id] != dynGuiManager.obj->input.direction[id]) {
			if (_gameInfo->snakes[id].size() <= 1) {
				_gameInfo->direction[id] = dynGuiManager.obj->input.direction[id];
			}
			else {
				Vec2 direction(_gameInfo->snakes[id][0].x - _gameInfo->snakes[id][1].x,
					_gameInfo->snakes[id][0].y - _gameInfo->snakes[id][1].y);
				if (direction.x > 1) direction.x = -1;
				else if (direction.x < -1) direction.x = 1;
				if (direction.y > 1) direction.y = -1;
				else if (direction.y < -1) direction.y = 1;

				if (dynGuiManager.obj->input.direction[id] == Direction::MOVE_UP && direction.y != 1)
					_gameInfo->direction[id] = dynGuiManager.obj->input.direction[id];
				else if (dynGuiManager.obj->input.direction[id] == Direction::MOVE_DOWN && direction.y != -1)
					_gameInfo->direction[id] = dynGuiManager.obj->input.direction[id];
				else if (dynGuiManager.obj->input.direction[id] == Direction::MOVE_LEFT && direction.x != 1)
					_gameInfo->direction[id] = dynGuiManager.obj->input.direction[id];
				else if (dynGuiManager.obj->input.direction[id] == Direction::MOVE_RIGHT && direction.x != -1)
					_gameInfo->direction[id] = dynGuiManager.obj->input.direction[id];
			}
		}
	}

	_gameInfo->bestScore = userData.u("highScore");
}

void Game::_updateSinglePlayer() {
	int id = 0;
	// update win
	if (_gameInfo->snakes[id].size() >= _gameInfo->boardSize * _gameInfo->boardSize) {
		_gameInfo->win = true;
	}
	// update gameOver
	if (_gameInfo->gameOver == false) {
		if (_gameInfo->snakes[id].size() == 0) {
			_gameInfo->gameOver = true;
		}
		else if (_gameInfo->snakes[id][0].x < 0 || _gameInfo->snakes[id][0].x >= _gameInfo->boardSize
		|| _gameInfo->snakes[id][0].y < 0 || _gameInfo->snakes[id][0].y >= _gameInfo->boardSize) {
			_gameInfo->gameOver = true;
		}
		auto it = std::find(++_gameInfo->snakes[id].begin(), _gameInfo->snakes[id].end(), _gameInfo->snakes[id][0]);
		if (it != _gameInfo->snakes[id].end()) {
			_gameInfo->gameOver = true;
		}
		// wall
		for (auto it = _gameInfo->wall.begin(); it != _gameInfo->wall.end(); it++) {
			if (it->pos == _gameInfo->snakes[id][0]) {
				_gameInfo->snakes[id].clear();
				continue;
			}
		}
		if (_gameInfo->gameOver) {
			_gameInfo->snakes[id].push_back(_lastDeletedSnake[id]);
			_gameInfo->snakes[id].pop_front();
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
		allSnakesSize += _gameInfo->snakes[id].size();
		if (_gameInfo->snakes[id].size() > biggerSnakeSize) {
			biggerSnakeSize = _gameInfo->snakes[id].size();
			biggerSnakeID = id;
		}
		if (_gameInfo->snakes[id].size() > 0)
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
		if (_gameInfo->isIA[id] == false && _gameInfo->snakes[id].size() > 0) {
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
		if (_gameInfo->snakes[id].size() == 0)
			continue;
		if (_gameInfo->snakes[id][0].x < 0 || _gameInfo->snakes[id][0].x >= _gameInfo->boardSize
		|| _gameInfo->snakes[id][0].y < 0 || _gameInfo->snakes[id][0].y >= _gameInfo->boardSize) {
			_gameInfo->snakes[id].clear();
			continue;
		}
		// snake
		auto it = std::find(++_gameInfo->snakes[id].begin(), _gameInfo->snakes[id].end(), _gameInfo->snakes[id][0]);
		if (it != _gameInfo->snakes[id].end()) {
			_gameInfo->snakes[id].clear();
			continue;
		}
		// others snakes
		for (int id2 = 0; id2 < _gameInfo->nbPlayers; id2++) {
			if (id == id2)
				continue;
			auto it = std::find(_gameInfo->snakes[id2].begin(), _gameInfo->snakes[id2].end(), _gameInfo->snakes[id][0]);
			if (it != _gameInfo->snakes[id2].end()) {
				_gameInfo->snakes[id].clear();
				continue;
			}
		}
		// wall
		for (auto it = _gameInfo->wall.begin(); it != _gameInfo->wall.end(); it++) {
			if (it->pos == _gameInfo->snakes[id][0]) {
				_gameInfo->snakes[id].clear();
				continue;
			}
		}
	}
}

// -- Exceptions errors --------------------------------------------------------
Game::GameException::GameException()
: std::runtime_error("[GameException]") {}

Game::GameException::GameException(const char* what_arg)
: std::runtime_error(std::string(std::string("[GameException] ") + what_arg).c_str()) {}
