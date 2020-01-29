#include <dlfcn.h>
#include <iostream>

#include "nibbler.hpp"
#include "Logging.hpp"
#include "DynGuiManager.hpp"

int main(int ac, char const **av) {
	(void)ac;
	(void)av;
	uint8_t			guiId = 0;
	DynGuiManager	dynGuiManager;

	initLogs();  // init logs functions

	// load the defaut gui
	try {
		dynGuiManager.loadGui(guiId);
		dynGuiManager.nibblerGui->init();
	}
	catch(const std::exception& e) {
		logErr(e.what());
		return EXIT_FAILURE;
	}

	gameLoop(dynGuiManager);

	return EXIT_SUCCESS;
}
