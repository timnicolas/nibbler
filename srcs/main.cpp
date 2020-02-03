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
	initSettings();
	try {
		if (s.loadFile("assets/settings.json") == false) {
			// warning when loading settings
		}
	}
	catch(SettingsJson::SettingsException const & e) {
		logErr(e.what());
	}

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

	return EXIT_SUCCESS;
}
