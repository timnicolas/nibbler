#include "Game.hpp"
#include "nibbler.hpp"

Game::Game() :
  _dynGuiManager() {}

bool Game::init() {
	try {
		_dynGuiManager.loadGui(0);
		_dynGuiManager.nibblerGui->init();
	}
	catch(const std::exception& e) {
		logErr(e.what());
		return false;
	}
	return true;
}

Game::Game(Game const &src) {
	*this = src;
}

Game::~Game() {
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
			nibblerGui->init();

			nibblerGui->input.loadGuiID = NO_GUI_LOADED;
		}

		nibblerGui->draw();

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
