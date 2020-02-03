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
	Game	game;

	SettingsJson settings;

	settings.addu("width")
		.setMin(5)
		.setMax(20)
		.setValue(12);
	settings.addu("height")
		.setMin(5)
		.setMax(20)
		.setValue(20);

	settings.adds("name")
		.setValue("nibbler");

    settings.addj("test1").addu("okok").setValue(0);

    settings.addj("global");
    settings.j("global").addi("fps")
		.setMax(40)
        .setValue(30);
    settings.j("global").adds("test");

    settings.j("global").addj("testJson");
	settings.j("global").j("testJson").addi("le int");

	try {
		if (settings.loadFile("assets/settings.json") == false) {
			logErr("warnings when loading settings");
		}
	}
	catch(SettingsJson::SettingsException const & e) {
		logErr(e.what());
	}

	std::cout << "\"settings\": " << settings;

	// srand(time(NULL));
	// initLogs();  // init logs functions

	// if (game.init(WIDTH, HEIGHT, BOARD_SIZE) == false)
	// 	return EXIT_FAILURE;

	// try {
	// 	game.run();
	// }
	// catch(std::exception const & e) {
	// 	logErr(e.what());
	// 	return EXIT_FAILURE;
	// }

	return EXIT_SUCCESS;
}
