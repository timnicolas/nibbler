#include <stdlib.h>
#include <time.h>
#include <dlfcn.h>
#include <iostream>

#include "nibbler.hpp"
#include "Logging.hpp"
#include "SettingsJson.hpp"
#include "Game.hpp"

int start(int ac, char const **av) {
	(void)ac;
	(void)av;
	initLogs();  // init logs functions
	initSettings("assets/settings.json");
	initUserData(s.s("userDataFilename"));

	s.j("screen").u("height") = s.j("screen").u("width") * 0.8;

	if (s.u("snakeSize") > s.u("boardSize") / 2) {
		logWarn("max size for snake is " << s.u("boardSize") / 2);
		s.u("snakeSize") = s.u("boardSize") / 2;
	}
	if (s.u("nbPlayers") + s.u("nbIA") > s.u("boardSize")) {
		logWarn("max players & IA is " << s.u("boardSize"));
		s.u("nbIA") = s.u("boardSize") - s.u("nbPlayers");
	}
	#if DEBUG
		std::cout << s.toString(JsonOpt::VERBOSE | JsonOpt::COLOR);
		std::cout << userData.toString(JsonOpt::VERBOSE | JsonOpt::COLOR);
	#endif

	srand(time(NULL));
	Game	game;

	if (game.init() == false)
		return EXIT_FAILURE;

	try {
		game.run();
	}
	catch(std::exception const & e) {
		logErr(e.what());
		return EXIT_FAILURE;
	}

	saveUserData(s.s("userDataFilename"));
	return EXIT_SUCCESS;
}

int main(int ac, char const **av) {
	int ret = start(ac, av);

	return ret;
}
