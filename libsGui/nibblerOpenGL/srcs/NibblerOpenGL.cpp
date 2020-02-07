#include "NibblerOpenGL.hpp"
#include "Logging.hpp"
#include "debug.hpp"
#include "Material.hpp"

std::chrono::milliseconds getMs() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
}

const float	NibblerOpenGL::_cubeVertices[] = {
	// positions			// normals
	0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,  // 0l
	-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,  // 0l
	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,  // 0l

	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,  // 0r
	0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,  // 0r
	0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,  // 0r


	0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,  // 1l
	0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,  // 1l
	0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,  // 1l

	0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,  // 1r
	0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,  // 1r
	0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,  // 1r


	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,  // 2l
	0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,  // 2l
	0.5f, -0.5f, -0.5f,		0.0f, 0.0f, -1.0f,  // 2l

	0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,  // 2r
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,  // 2r
	-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,  // 2r


	-0.5f, 0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,  // 3l
	-0.5f, 0.5f, -0.5f,		-1.0f, 0.0f, 0.0f,  // 3l
	-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,  // 3l

	-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,  // 3r
	-0.5f, -0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,  // 3r
	-0.5f, 0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,  // 3r


	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,  // 4l
	0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,  // 4l
	0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,  // 4l

	0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,  // 4r
	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,  // 4r
	-0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,  // 4r


	-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,  // 5l
	0.5f, -0.5f, -0.5f,		0.0f, -1.0f, 0.0f,  // 5l
	0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,  // 5l

	0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,  // 5r
	-0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,  // 5r
	-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f   // 5r
};

NibblerOpenGL::NibblerOpenGL() :
  _win(nullptr),
  _event(new SDL_Event()),
  _context(0),
  _cubeShader(nullptr),
  _cam(nullptr),
  _textRender(nullptr),
  _skybox(nullptr),
  _lastLoopMs(0) {
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
	SDL_ShowCursor(SDL_ENABLE);
	SDL_SetRelativeMouseMode(SDL_FALSE);
	glDeleteBuffers(1, &_cubeShaderVBO);
	glDeleteVertexArrays(1, &_cubeShaderVAO);
	delete _cubeShader;
	delete _textRender;
	delete _skybox;
	delete _cam;
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
		logErr("don't use NibblerOpenGL copy operator");
	}
	return *this;
}

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);

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

	glEnable(GL_MULTISAMPLE);  // anti aliasing
	glEnable(GL_CULL_FACE);  // face culling
	glEnable(GL_BLEND);  // enable blending (used in textRender)
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	try {
		_cubeShader = new Shader(CUBE_VS_PATH, CUBE_FS_PATH);
		_textRender = new TextRender(_gameInfo->width, _gameInfo->height);
		_textBasicHeight = _gameInfo->width / 40;
		_textRender->loadFont("basicFont", _gameInfo->font, _textBasicHeight);
		_textTitleHeight = _gameInfo->width / 10;
		_textRender->loadFont("titleFont", _gameInfo->font, _textTitleHeight);
		_skybox = new Skybox;
	}
	catch (Shader::ShaderError & e) {
        logErr("while loading OpenGL: " << e.what());
		return false;
	}
	catch (TextRender::TextRenderError & e) {
        logErr("while loading OpenGL: " << e.what());
		return false;
	}

	glm::vec3 camPos;
	camPos.x = mapFloat(_gameInfo->boardSize, _gameInfo->minBoardSize, _gameInfo->maxBoardSize, -4, 16);
	camPos.y = mapFloat(_gameInfo->boardSize, _gameInfo->minBoardSize, _gameInfo->maxBoardSize, 15, 42);
	camPos.z = mapFloat(_gameInfo->boardSize, _gameInfo->minBoardSize, _gameInfo->maxBoardSize, 18, 80);
	float yaw = mapFloat(_gameInfo->boardSize, _gameInfo->minBoardSize, _gameInfo->maxBoardSize, -58, -80);
	float pitch = mapFloat(_gameInfo->boardSize, _gameInfo->minBoardSize, _gameInfo->maxBoardSize, -40, -40);
	_cam = new Camera(camPos, glm::vec3(0, 1, 0), yaw, pitch);

	float angle = _cam->zoom;
	float ratio = static_cast<float>(_gameInfo->width) / _gameInfo->height;
	float nearD = 0.1f;
	float farD = 400;
	_projection = glm::perspective(glm::radians(angle), ratio, nearD, farD);

	glGenVertexArrays(1, &_cubeShaderVAO);
    glGenBuffers(1, &(_cubeShaderVBO));

    glBindBuffer(GL_ARRAY_BUFFER, _cubeShaderVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(NibblerOpenGL::_cubeVertices), NibblerOpenGL::_cubeVertices, GL_STATIC_DRAW);

    glBindVertexArray(_cubeShaderVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, SIZE_LINE * sizeof(float),
		reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, SIZE_LINE * sizeof(float),
		reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

	_skybox->getShader().use();
	_skybox->getShader().setMat4("projection", _projection);
	_skybox->getShader().unuse();
	_cubeShader->use();
	_cubeShader->setMat4("projection", _projection);
	// set cube material
	Material material;
	_cubeShader->setVec3("material.specular", material.specular);
	_cubeShader->setFloat("material.shininess", material.shininess);

	// set direction light
	_cubeShader->setVec3("dirLight.direction", -0.2f, -0.8f, 0.6f);
	_cubeShader->setVec3("dirLight.ambient", 0.4f, 0.4f, 0.4f);
	_cubeShader->setVec3("dirLight.diffuse", 0.8f, 0.8f, 0.8f);
	_cubeShader->setVec3("dirLight.specular", 0.1f, 0.1f, 0.1f);

	// set point light
	_cubeShader->setBool("pointLight.enabled", false);

	_lastLoopMs = getMs().count();

    return true;
}

void NibblerOpenGL::updateInput() {
	uint64_t time = getMs().count();
	float dtTime = (time - _lastLoopMs) / 1000.0;
	_lastLoopMs = time;
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

			else if (_event->key.keysym.sym == SDLK_UP)
				input.direction[0] = Direction::MOVE_UP;
			else if (_event->key.keysym.sym == SDLK_DOWN)
				input.direction[0] = Direction::MOVE_DOWN;
			else if (_event->key.keysym.sym == SDLK_LEFT)
				input.direction[0] = Direction::MOVE_LEFT;
			else if (_event->key.keysym.sym == SDLK_RIGHT)
				input.direction[0] = Direction::MOVE_RIGHT;

			// move player 2
			if (_gameInfo->nbPlayers >= 2) {
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

		if (_event->type == SDL_MOUSEMOTION) {
			_cam->processMouseMovement(_event->motion.xrel, -_event->motion.yrel);
		}
	}

	const Uint8 * keystates = SDL_GetKeyboardState(NULL);

	if (_gameInfo->nbPlayers == 1) {  // move camera only on singlePlayer
		bool isRun = false;
		if (keystates[SDL_SCANCODE_LSHIFT])
			isRun = true;
		if (keystates[SDL_SCANCODE_W])
			_cam->processKeyboard(CamMovement::Forward, dtTime, isRun);
		if (keystates[SDL_SCANCODE_S])
			_cam->processKeyboard(CamMovement::Backward, dtTime, isRun);
		if (keystates[SDL_SCANCODE_A])
			_cam->processKeyboard(CamMovement::Left, dtTime, isRun);
		if (keystates[SDL_SCANCODE_D])
			_cam->processKeyboard(CamMovement::Right, dtTime, isRun);
		if (keystates[SDL_SCANCODE_E])
			_cam->processKeyboard(CamMovement::Up, dtTime, isRun);
		if (keystates[SDL_SCANCODE_Q])
			_cam->processKeyboard(CamMovement::Down, dtTime, isRun);
	}
}

bool NibblerOpenGL::draw(std::vector<std::deque<Vec2>> & snakes, std::deque<Vec2> & food) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, _gameInfo->width, _gameInfo->height);
    glClearColor(0.11373f, 0.17647f, 0.27059f, 1.0f);

	CAMERA_MAT4 view = _cam->getViewMatrix();

	_cubeShader->use();
	_cubeShader->setMat4("view", view);
	_cubeShader->setVec3("viewPos", _cam->pos);
	glBindVertexArray(_cubeShaderVAO);
	_cubeShader->unuse();

	CAMERA_MAT4	skyView = view;
	skyView[3][0] = 0;  // remove translation for the skybox
	skyView[3][1] = 0;
	skyView[3][2] = 0;
	_skybox->getShader().use();
	_skybox->getShader().setMat4("view", skyView);
	_skybox->getShader().unuse();

	_cubeShader->use();
	glm::mat4 model(1.0);
	glm::vec3 pos = glm::vec3(0.0, 0.0, 0.0);

	// draw board
	for (int i = 0; i < _gameInfo->boardSize; i++) {
		for (int j = 0; j < _gameInfo->boardSize; j++) {
			pos.x = i;
			pos.z = j;
			uint32_t color = ((i + j) & 1) ? SQUARE_COLOR_1 : SQUARE_COLOR_2;
			model = glm::translate(glm::mat4(1.0), pos);
			_cubeShader->setVec4("color", TO_OPENGL_COLOR(color));
			_cubeShader->setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
	// draw snakes
	pos.y = 1;
	for (int id = 0; id < _gameInfo->nbPlayers; id++) {
		int		i = 0;
		float	max = (snakes[id].size() == 1) ? 1 : snakes[id].size() - 1;
		for (auto it = snakes[id].begin(); it != snakes[id].end(); it++) {
			pos.x = it->x;
			pos.z = it->y;
			uint32_t	color = mixColor(getColor(id, 1), getColor(id, 2), i / max);
			model = glm::translate(glm::mat4(1.0), pos);
			_cubeShader->setVec4("color", TO_OPENGL_COLOR(color));
			_cubeShader->setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			i++;
		}
	}
	// draw food
	pos.y = 1;
	for (auto it = food.begin(); it != food.end(); it++) {
		pos.x = it->x;
		pos.z = it->y;
		model = glm::translate(glm::mat4(1.0), pos);
		_cubeShader->setVec4("color", TO_OPENGL_COLOR(FOOD_COLOR));
		_cubeShader->setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(_cubeVertices) / (sizeof(float) * SIZE_LINE));
	}
	_cubeShader->unuse();

	_skybox->draw(0.3);

	// text
    {
		int x = 20;
		int y = _gameInfo->height - _textBasicHeight - 10;
		int lineSz = _textBasicHeight * 1.2;
		std::string text;
		if (_gameInfo->nbPlayers == 1) {
			text = "Score: " + std::to_string(_gameInfo->scores[0]);
			_textRender->write("basicFont", text, x, y, 1, TO_OPENGL_COLOR(0xFFFFFF));
			y -= lineSz;
		}
		else {
			for (int id = 0; id < _gameInfo->nbPlayers; id++) {
				text = "Score ";
				if (_gameInfo->isIA[id])
					text += "[IA] ";
				text += std::to_string(id + 1) + " : " + std::to_string(_gameInfo->scores[id]);
				_textRender->write("basicFont", text, x, y, 1, TO_OPENGL_COLOR(getColor(id, 1)));
				y -= lineSz;
			}
		}
		text = "Best: " + std::to_string(_gameInfo->bestScore);
		_textRender->write("basicFont", text, x, y, 1, TO_OPENGL_COLOR(0xFFFFFF));

		y = 10;
		_textRender->write("basicFont", "r: restart", x, y, 1, TO_OPENGL_COLOR(0xFFFFFF));
		y += lineSz;
		_textRender->write("basicFont", "arrow: turn", x, y, 1, TO_OPENGL_COLOR(0xFFFFFF));
		y += lineSz;
		_textRender->write("basicFont", "space: pause", x, y, 1, TO_OPENGL_COLOR(0xFFFFFF));
		y += lineSz;
		_textRender->write("basicFont", "[wasd]: move camera", x, y, 1, TO_OPENGL_COLOR(0xFFFFFF));
		y += lineSz;
		_textRender->write("basicFont", "[ed]: move camera (up-down)", x, y, 1, TO_OPENGL_COLOR(0xFFFFFF));
	}

	if (_gameInfo->win || _gameInfo->gameOver || _gameInfo->paused) {
		std::string	text;
		uint32_t	color = TEXT_COLOR;

		if (_gameInfo->win) {
			if (_gameInfo->nbPlayers == 1)
				text = "You win !";
			else
				text = ((_gameInfo->isIA[_gameInfo->winnerID]) ? "IA " : "Player ")
					+ std::to_string(_gameInfo->winnerID + 1) + " win !";
			color = TEXT_WIN_COLOR;
		}
		else if (_gameInfo->gameOver) {
			text = "Game over";
			color = TEXT_GAMEOVER_COLOR;
		}
		else if (_gameInfo->paused) {
			text = "Pause";
		}
		float textX = _gameInfo->width / 2 - _textRender->strWidth("titleFont", text) / 2;
		float textY = _gameInfo->height / 2 - _textTitleHeight / 2;
		_textRender->write("titleFont", text, textX, textY, 1, TO_OPENGL_COLOR(color));
	}

    SDL_GL_SwapWindow(_win);
	checkError();
	return true;
}

extern "C" {
	ANibblerGui *makeNibblerOpenGL() {
		return new NibblerOpenGL();
	}
}
