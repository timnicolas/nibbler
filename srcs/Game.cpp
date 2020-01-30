#include "Game.hpp"
#include "nibbler.hpp"

Game::Game() :
  _dynGuiManager(),
  _gameInfo(nullptr),
  _snake() {}

bool Game::init(std::string title, uint16_t width, uint16_t height, uint8_t boardSize) {
	_gameInfo = new GameInfo();
	_gameInfo->title = title;
	_gameInfo->width = width;
	_gameInfo->height = height;
	_gameInfo->boardSize = boardSize;
	try {
		_dynGuiManager.loadGui(0);
		_dynGuiManager.nibblerGui->init(_gameInfo);
	}
	catch(DynGuiManager::DynGuiManagerException const & e) {
		logErr(e.what());
		return false;
	}
	return true;
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
	float						loopTime = 1000 / FPS;
	std::chrono::milliseconds	time_start;
	ANibblerGui					*nibblerGui = nullptr;
	#if DEBUG_FPS_LOW == true
		bool firstLoop = true;
	#endif

	nibblerGui = _dynGuiManager.nibblerGui;

	while (nibblerGui->input.quit == false) {
		time_start = getMs();

		nibblerGui->updateInput();

		// logDebug("moving direction " << nibblerGui->input.direction);

		// verify id viability
		if (nibblerGui->input.loadGuiID < NB_GUI && \
		nibblerGui->input.loadGuiID != _dynGuiManager.getCurrentGuiID()) {
			// change Gui
			_dynGuiManager.loadGui(nibblerGui->input.loadGuiID);
			nibblerGui = _dynGuiManager.nibblerGui;
			nibblerGui->init(_gameInfo);

			nibblerGui->input.loadGuiID = NO_GUI_LOADED;
		}

		nibblerGui->draw(_snake);

		// fps
		std::chrono::milliseconds time_loop = getMs() - time_start;
		if (time_loop.count() > loopTime) {
			#if DEBUG_FPS_LOW == true
				if (!firstLoop)
					logDebug("update loop slow -> " << time_loop.count() << "ms / " << loopTime << "ms (" << FPS << "fps)");
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

// -- Exceptions errors --------------------------------------------------------
Game::GameException::GameException()
: std::runtime_error("[GameException]") {}

Game::GameException::GameException(const char* what_arg)
: std::runtime_error(std::string(std::string("[GameException] ") + what_arg).c_str()) {}
