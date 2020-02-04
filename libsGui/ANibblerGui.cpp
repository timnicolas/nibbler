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
	input.direction = _gameInfo->direction;

	return _init();
}

// -- GameInfo ------------------------------------------------------------------

GameInfo::GameInfo()
: title("nibbler"),
  width(800),
  height(600),
  boardSize(20),
  rules() {
	rules.canExitBorder = true;
	restart();
}

void GameInfo::restart() {
	direction = Direction::MOVE_UP;
	paused = false;
	win = false;
	gameOver = false;
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
	direction = Direction::MOVE_UP;
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
