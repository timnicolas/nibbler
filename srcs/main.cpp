#include <dlfcn.h>
#include <iostream>

#include "nibbler.hpp"
#include "Logging.hpp"
#include "Game.hpp"

int main(int ac, char const **av) {
	(void)ac;
	(void)av;
	Game	game;

	initLogs();  // init logs functions

	if (game.init() == false)
		return EXIT_FAILURE;

	try {
		game.run();
	}
	catch(const std::exception& e) {
		logErr(e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
