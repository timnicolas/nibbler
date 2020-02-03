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

    settings.addj("test1").addu("okok");

    settings.addj("global");
    settings.j("global").addi("fps")
        .setValue(56);
    settings.j("global").adds("test");

    settings.j("global").addj("testJson");
	settings.j("global").j("testJson").addi("le int");

	std::cout << settings.s("name") << " "
	<< settings.u("width") << "*"<< settings.u("height") << " fps: "
    << settings.j("global").i("fps")
    << std::endl;

	std::cout << "\"settings.json\": " << settings;

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
