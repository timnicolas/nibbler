#include <stdlib.h>
#include <time.h>
#include <dlfcn.h>
#include <iostream>

#include "nibbler.hpp"
#include "Logging.hpp"
#include "SettingsJson.hpp"
#include "Game.hpp"

int main(int ac, char const **av) {
	(void)ac;
	(void)av;
	initLogs();  // init logs functions
	initSettings("assets/settings.json");
	initUserData(s.s("userDataFilename"));

	s.j("screen").u("height") = s.j("screen").u("width") * 0.6;

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
