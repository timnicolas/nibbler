#include "NibblerSDL.hpp"
#include "Logging.hpp"

NibblerSDL::NibblerSDL() :
  _win(nullptr),
  _event(new SDL_Event()) {
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

NibblerSDL::~NibblerSDL() {
	logInfo("exit SDL");
	delete _event;
	SDL_DestroyWindow(_win);
    SDL_Quit();
}

NibblerSDL::NibblerSDL(NibblerSDL const &src) {
	*this = src;
}

NibblerSDL &NibblerSDL::operator=(NibblerSDL const &rhs) {
	if (this != &rhs) {
		_win = rhs._win;
		_surface = rhs._surface;
		_event = rhs._event;
	}
	return *this;
}

bool NibblerSDL::init() {
	logInfo("loading SDL");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        logErr("while loading SDL: " << SDL_GetError());
        SDL_Quit();
		return false;
    }

	_win = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (_win == nullptr) {
        logErr("while loading SDL: " << SDL_GetError());
		SDL_Quit();
		return false;
	}

	_surface = SDL_GetWindowSurface(_win);
	if (_surface == nullptr) {
        logErr("while loading SDL: " << SDL_GetError());
		SDL_Quit();
		return false;
	}

    return true;
}

void NibblerSDL::updateInput() {
	while (SDL_PollEvent(_event)) {
		if (_event->window.event == SDL_WINDOWEVENT_CLOSE)
			input.quit = true;
		else if (_event->key.type == SDL_KEYDOWN && _event->key.keysym.sym == SDLK_ESCAPE)
			input.quit = true;

		else if (_event->key.type == SDL_KEYDOWN && _event->key.keysym.sym == SDLK_UP)
			input.direction = ANibblerGui::Input::MOVE_UP;
		else if (_event->key.type == SDL_KEYDOWN && _event->key.keysym.sym == SDLK_DOWN)
			input.direction = ANibblerGui::Input::MOVE_DOWN;
		else if (_event->key.type == SDL_KEYDOWN && _event->key.keysym.sym == SDLK_LEFT)
			input.direction = ANibblerGui::Input::MOVE_LEFT;
		else if (_event->key.type == SDL_KEYDOWN && _event->key.keysym.sym == SDLK_RIGHT)
			input.direction = ANibblerGui::Input::MOVE_RIGHT;

		else if (_event->key.type == SDL_KEYDOWN && _event->key.keysym.sym == SDLK_1)
			input.loadGuiID = 0;
		else if (_event->key.type == SDL_KEYDOWN && _event->key.keysym.sym == SDLK_2)
			input.loadGuiID = 1;
		else if (_event->key.type == SDL_KEYDOWN && _event->key.keysym.sym == SDLK_3)
			input.loadGuiID = 2;
	}
}

bool NibblerSDL::draw() {
	// clear screen
	SDL_FillRect(_surface, NULL, 0x000000);

	// draw rect on the screen
	SDL_Rect rect = {
		100,
		100,
		10 + 100 * input.direction,
		10 + 100 * (4 - input.direction),
	};
	SDL_FillRect(_surface, &rect, SDL_MapRGB(_surface->format, 255, 0, 0));
	SDL_UpdateWindowSurface(_win);
	return true;
}

extern "C" {
	ANibblerGui *makeNibblerSDL() {
		return new NibblerSDL();
	}
}
