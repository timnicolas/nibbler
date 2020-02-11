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
		logErr("don't use NibblerSDL copy operator");
	}
	return *this;
}

bool NibblerSDL::_init() {
	logInfo("loading SDL");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        logErr("while loading SDL: " << SDL_GetError());
        SDL_Quit();
		return false;
    }

	_win = SDL_CreateWindow((_gameInfo->title + " SDL").c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		_gameInfo->width, _gameInfo->height, SDL_WINDOW_SHOWN);
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

		if (_event->key.type == SDL_KEYDOWN) {
			if (_event->key.keysym.sym == SDLK_ESCAPE)
				input.quit = true;

			else if (_event->key.keysym.sym == SDLK_SPACE)
				input.paused = !input.paused;
			else if (_event->key.keysym.sym == SDLK_r)
				input.restart = true;

			// move player 1
			else if (_event->key.keysym.sym == SDLK_UP)
				input.direction[0] = Direction::MOVE_UP;
			else if (_event->key.keysym.sym == SDLK_DOWN)
				input.direction[0] = Direction::MOVE_DOWN;
			else if (_event->key.keysym.sym == SDLK_LEFT)
				input.direction[0] = Direction::MOVE_LEFT;
			else if (_event->key.keysym.sym == SDLK_RIGHT)
				input.direction[0] = Direction::MOVE_RIGHT;

			// move player 2
			if (_gameInfo->nbPlayers >= 2 && _gameInfo->isIA[1] == false) {
				if (_event->key.keysym.sym == SDLK_w)
					input.direction[1] = Direction::MOVE_UP;
				else if (_event->key.keysym.sym == SDLK_s)
					input.direction[1] = Direction::MOVE_DOWN;
				else if (_event->key.keysym.sym == SDLK_a)
					input.direction[1] = Direction::MOVE_LEFT;
				else if (_event->key.keysym.sym == SDLK_d)
					input.direction[1] = Direction::MOVE_RIGHT;
			}

			if (_event->key.keysym.sym == SDLK_1)
				input.loadGuiID = 0;
			else if (_event->key.keysym.sym == SDLK_2)
				input.loadGuiID = 1;
			else if (_event->key.keysym.sym == SDLK_3)
				input.loadGuiID = 2;
		}
	}
}

bool NibblerSDL::draw() {
	// clear screen
	SDL_FillRect(_surface, NULL, 0x000000);

	// set the size of the square
	float startX = BORDER_SIZE;
	float startY = BORDER_SIZE;
	float size = _gameInfo->height - (2 * BORDER_SIZE);
	float step = size / _gameInfo->boardSize;

	// border
	SDL_Rect rect = {
		static_cast<int>(startX - BORDER_SIZE),
		static_cast<int>(startY - BORDER_SIZE),
		static_cast<int>(size + (2 * BORDER_SIZE)),
		static_cast<int>(size + (2 * BORDER_SIZE)),
	};
	SDL_FillRect(_surface, &rect, BORDER_COLOR);


	// draw board
	for (int i = 0; i < _gameInfo->boardSize; i++) {
		for (int j = 0; j < _gameInfo->boardSize; j++) {
			SDL_Rect rect = {
				static_cast<int>(startX + step * i),
				static_cast<int>(startY + step * j),
				static_cast<int>(step + 0.5),
				static_cast<int>(step + 0.5),
			};
			uint32_t color = ((i + j) & 1) ? SQUARE_COLOR_1 : SQUARE_COLOR_2;
			SDL_FillRect(_surface, &rect, color);
		}
	}
	// draw snakes
	for (int id = 0; id < _gameInfo->nbPlayers; id++) {
		int		i = 0;
		float	max = (_gameInfo->snakes[id].size() == 1) ? 1 : _gameInfo->snakes[id].size() - 1;
		for (auto it = _gameInfo->snakes[id].begin(); it != _gameInfo->snakes[id].end(); it++) {
			SDL_Rect rect = {
				static_cast<int>(startX + step * it->x),
				static_cast<int>(startY + step * it->y),
				static_cast<int>(step + 0.5),
				static_cast<int>(step + 0.5),
			};
			uint32_t	color = mixColor(getColor(id, 1), getColor(id, 2), i / max);
			SDL_FillRect(_surface, &rect, color);
			i++;
		}
	}
	// draw food
	for (auto it = _gameInfo->food.begin(); it != _gameInfo->food.end(); it++) {
		SDL_Rect rect = {
			static_cast<int>(startX + step * it->x),
			static_cast<int>(startY + step * it->y),
			static_cast<int>(step + 0.5),
			static_cast<int>(step + 0.5),
		};
		SDL_FillRect(_surface, &rect, FOOD_COLOR);
	}

	// render on screen
	SDL_UpdateWindowSurface(_win);
	return true;
}

extern "C" {
	ANibblerGui *makeNibblerSDL() {
		return new NibblerSDL();
	}
}
