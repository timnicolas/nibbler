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

	settings.addi("width")
		.setMin(5)
		.setMax(20)
		.setValue(12);
	settings.addi("height")
		.setMin(5)
		.setMax(20)
		.setValue(20);

	settings.adds("name")
		.setValue("nibbler");

	std::cout << settings.gets("name") << " "
	<< settings.geti("width") << "*"<< settings.geti("height") << std::endl;

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
