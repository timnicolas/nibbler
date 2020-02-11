#include "ANibblerGui.hpp"

ANibblerGui::ANibblerGui()
: input(),
  _gameInfo(nullptr) {}

ANibblerGui::~ANibblerGui() {
}

ANibblerGui::ANibblerGui(ANibblerGui const &src) {
	*this = src;
}

ANibblerGui &ANibblerGui::operator=(ANibblerGui const &rhs) {
	if (this != &rhs) {
		input = rhs.input;
	}
	return *this;
}

bool ANibblerGui::init(GameInfo * gameInfo) {
	_gameInfo = gameInfo;

	input.paused = _gameInfo->paused;
	input.direction.clear();
	input.usingBonus.clear();
	for (int id = 0; id < _gameInfo->nbPlayers; id++) {
		input.direction.push_back(_gameInfo->direction[id]);
		input.usingBonus.push_back(false);
	}

	return _init();
}

// -- GameInfo ------------------------------------------------------------------

GameInfo::GameInfo(int nbPlayers_)
: title("nibbler"),
  width(800),
  height(600),
  boardSize(20),
  rules(),
  nbPlayers(nbPlayers_) {
	rules.canExitBorder = true;
	for (int id = 0; id < nbPlayers; id++) {
		snakes.push_back(std::deque<Vec2>());
		nbBonus.push_back(0);
		direction.push_back(Direction::MOVE_UP);
		scores.push_back(0);
		isIA.push_back(false);
	}
	restart();
}

void GameInfo::restart() {
	for (int id = 0; id < nbPlayers; id++) {
		Direction::Enum dir = (id & 1) ? Direction::MOVE_UP : Direction::MOVE_DOWN;
		direction[id] = dir;
		scores[id] = 0;
	}
	paused = false;
	win = false;
	gameOver = false;
	winnerID = 0;
	food.clear();
	bonus.clear();
	wall.clear();
}

// -- Vec2 ---------------------------------------------------------------------

Vec2::Vec2() : x(0), y(0) {}
Vec2::Vec2(int x_, int y_) : x(x_), y(y_) {}
bool Vec2::operator==(Vec2 const & other) const {
	return x == other.x && y == other.y;
}

// -- Input --------------------------------------------------------------------

ANibblerGui::Input::Input() {
	reset();
}

ANibblerGui::Input::Input(ANibblerGui::Input const &src) {
	*this = src;
}

ANibblerGui::Input &ANibblerGui::Input::operator=(ANibblerGui::Input const &rhs) {
	if (this != &rhs) {
		direction = rhs.direction;
		loadGuiID = rhs.loadGuiID;
	}
	return *this;
}

void ANibblerGui::Input::reset() {
	quit = false;
	paused = false;
	restart = false;
	for (int id = 0; id < static_cast<int>(direction.size()); id++) {
		Direction::Enum dir = (id & 1) ? Direction::MOVE_UP : Direction::MOVE_DOWN;
		direction[id] = dir;
		usingBonus[id] = false;
	}
	loadGuiID = NO_GUI_LOADED;
}

// -- Others -------------------------------------------------------------------

uint32_t mixColor(uint32_t c1, uint32_t c2, float factor) {
	uint8_t cMin;
	uint8_t cMax;

	cMin = ((c1 >> 16) & 0xFF);
	cMax = ((c2 >> 16) & 0xFF);
	uint8_t r;
	if (cMin < cMax)
		r = cMin + (cMax - cMin) * factor;
	else
		r = cMin - (cMin - cMax) * factor;

	cMin = ((c1 >> 8) & 0xFF);
	cMax = ((c2 >> 8) & 0xFF);
	uint8_t g;
	if (cMin < cMax)
		g = cMin + (cMax - cMin) * factor;
	else
		g = cMin - (cMin - cMax) * factor;

	cMin = ((c1 >> 0) & 0xFF);
	cMax = ((c2 >> 0) & 0xFF);
	uint8_t b;
	if (cMin < cMax)
		b = cMin + (cMax - cMin) * factor;
	else
		b = cMin - (cMin - cMax) * factor;

	return (r << 16) + (g << 8) + (b << 0);
}

uint32_t getColor(int id, int colorNB) {
	if (id % 6 == 1) return (colorNB == 1) ? SNAKE_2_COLOR_1 : SNAKE_2_COLOR_2;
	if (id % 6 == 2) return (colorNB == 1) ? SNAKE_3_COLOR_1 : SNAKE_3_COLOR_2;
	if (id % 6 == 3) return (colorNB == 1) ? SNAKE_4_COLOR_1 : SNAKE_4_COLOR_2;
	if (id % 6 == 4) return (colorNB == 1) ? SNAKE_5_COLOR_1 : SNAKE_5_COLOR_2;
	if (id % 6 == 5) return (colorNB == 1) ? SNAKE_6_COLOR_1 : SNAKE_6_COLOR_2;
	return (colorNB == 1) ? SNAKE_1_COLOR_1 : SNAKE_1_COLOR_2;
}
