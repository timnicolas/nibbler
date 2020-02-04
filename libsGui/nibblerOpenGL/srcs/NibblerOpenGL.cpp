#include "NibblerOpenGL.hpp"
#include "Logging.hpp"

NibblerOpenGL::NibblerOpenGL() :
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

NibblerOpenGL::~NibblerOpenGL() {
	logInfo("exit OpenGL");
	delete _event;
	SDL_DestroyWindow(_win);
    SDL_Quit();
}

NibblerOpenGL::NibblerOpenGL(NibblerOpenGL const &src) {
	*this = src;
}

NibblerOpenGL &NibblerOpenGL::operator=(NibblerOpenGL const &rhs) {
	if (this != &rhs) {
		_win = rhs._win;
		_surface = rhs._surface;
		_event = rhs._event;
	}
	return *this;
}

bool NibblerOpenGL::_init() {
	logInfo("loading OpenGL");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        logErr("while loading OpenGL: " << SDL_GetError());
        SDL_Quit();
		return false;
    }

	_win = SDL_CreateWindow((_gameInfo->title + " OpenGL").c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		_gameInfo->width, _gameInfo->height, SDL_WINDOW_SHOWN);
	if (_win == nullptr) {
        logErr("while loading OpenGL: " << SDL_GetError());
		SDL_Quit();
		return false;
	}

	_surface = SDL_GetWindowSurface(_win);
	if (_surface == nullptr) {
        logErr("while loading OpenGL: " << SDL_GetError());
		SDL_Quit();
		return false;
	}

    return true;
}

void NibblerOpenGL::updateInput() {
	while (SDL_PollEvent(_event)) {
		if (_event->window.event == SDL_WINDOWEVENT_CLOSE)
			input.quit = true;
		else if (_event->key.type == SDL_KEYDOWN && _event->key.keysym.sym == SDLK_ESCAPE)
			input.quit = true;

		else if (_event->key.type == SDL_KEYDOWN && _event->key.keysym.sym == SDLK_SPACE)
			input.paused = !input.paused;
		else if (_event->key.type == SDL_KEYDOWN && _event->key.keysym.sym == SDLK_r)
			input.restart = true;

		else if (_event->key.type == SDL_KEYDOWN && _event->key.keysym.sym == SDLK_UP)
			input.direction = Direction::MOVE_UP;
		else if (_event->key.type == SDL_KEYDOWN && _event->key.keysym.sym == SDLK_DOWN)
			input.direction = Direction::MOVE_DOWN;
		else if (_event->key.type == SDL_KEYDOWN && _event->key.keysym.sym == SDLK_LEFT)
			input.direction = Direction::MOVE_LEFT;
		else if (_event->key.type == SDL_KEYDOWN && _event->key.keysym.sym == SDLK_RIGHT)
			input.direction = Direction::MOVE_RIGHT;

		else if (_event->key.type == SDL_KEYDOWN && _event->key.keysym.sym == SDLK_1)
			input.loadGuiID = 0;
		else if (_event->key.type == SDL_KEYDOWN && _event->key.keysym.sym == SDLK_2)
			input.loadGuiID = 1;
		else if (_event->key.type == SDL_KEYDOWN && _event->key.keysym.sym == SDLK_3)
			input.loadGuiID = 2;
	}
}

bool NibblerOpenGL::draw(std::deque<Vec2> & snake, std::deque<Vec2> & food) {
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
	// draw snake
	int		i = 0;
	float	max = (snake.size() == 1) ? 1 : snake.size() - 1;
	for (auto it = snake.begin(); it != snake.end(); it++) {
		SDL_Rect rect = {
			static_cast<int>(startX + step * it->x),
			static_cast<int>(startY + step * it->y),
			static_cast<int>(step + 0.5),
			static_cast<int>(step + 0.5),
		};
		uint32_t	color = mixColor(SNAKE_COLOR_1, SNAKE_COLOR_2, i / max);
		SDL_FillRect(_surface, &rect, color);
		i++;
	}
	// draw food
	for (auto it = food.begin(); it != food.end(); it++) {
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
	ANibblerGui *makeNibblerOpenGL() {
		return new NibblerOpenGL();
	}
}
