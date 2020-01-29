#include <dlfcn.h>
#include <iostream>

#include "nibbler.hpp"
#include "Logging.hpp"
#include "NibblerSDL.hpp"
#include "NibblerSFML.hpp"

int main(int ac, char const **av) {
	(void)ac;
	(void)av;
	void *			hndl = nullptr;
	ANibblerGui	*	nibblerGui = nullptr;

	initLogs();  // init logs functions

	int lib = 2;
	if (lib == 1) {
		makerNibblerSDL		pMaker;
		// load librairy
		hndl = dlopen("libNibblerSDL.so", RTLD_LAZY);
		if (hndl == NULL) {
			logErr(dlerror());
			return EXIT_FAILURE;
		}

		void	*mkr = dlsym(hndl, "makeNibblerSDL");
		if (mkr == NULL) {
			logErr(dlerror());
			return EXIT_FAILURE;
		}
		pMaker = reinterpret_cast<makerNibblerSDL>(mkr);
		nibblerGui = pMaker();
	}
	else {
		makerNibblerSFML	pMaker;
		// load librairy
		hndl = dlopen("libNibblerSFML.so", RTLD_LAZY);
		if (hndl == NULL) {
			logErr(dlerror());
			return EXIT_FAILURE;
		}

		void	*mkr = dlsym(hndl, "makeNibblerSFML");
		if (mkr == NULL) {
			logErr(dlerror());
			return EXIT_FAILURE;
		}
		pMaker = reinterpret_cast<makerNibblerSFML>(mkr);
		nibblerGui = pMaker();
	}

	nibblerGui->init();

	gameLoop(nibblerGui);

	delete nibblerGui;
	dlclose(hndl);

	return EXIT_SUCCESS;
}
