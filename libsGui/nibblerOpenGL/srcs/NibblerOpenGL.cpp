#include "NibblerOpenGL.hpp"
#include "Logging.hpp"

NibblerOpenGL::NibblerOpenGL() :
  _win(nullptr),
  _event(new SDL_Event()),
  _context(0) {
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
	SDL_GL_DeleteContext(_context);
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


    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

	_win = SDL_CreateWindow((_gameInfo->title + " OpenGL").c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		_gameInfo->width, _gameInfo->height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (_win == nullptr) {
        logErr("while loading OpenGL: " << SDL_GetError());
		return false;
	}

	_context = SDL_GL_CreateContext(_win);
    if (_context == 0) {
        logErr("while loading OpenGL: " << SDL_GetError());
        return false;
    }

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        logErr("while loading OpenGL: failed to init glad");
        return false;
	}

	try {
		_cubeShader = new Shader(CUBE_VS_PATH, CUBE_FS_PATH);
	}
	catch (Shader::ShaderError & e) {
        logErr("while loading OpenGL: " << e.what());
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
    glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, _gameInfo->width, _gameInfo->height);
    glClearColor(0.11373f, 0.17647f, 0.27059f, 1.0f);

	// std::cout << "draw\n";

    SDL_GL_SwapWindow(_win);
	return true;
}

extern "C" {
	ANibblerGui *makeNibblerOpenGL() {
		return new NibblerOpenGL();
	}
}
