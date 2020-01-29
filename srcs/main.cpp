#include <dlfcn.h>
#include <iostream>

#include "nibbler.hpp"
#include "Logging.hpp"
#include "NibblerSDL.hpp"

int main(int ac, char const **av) {
	(void)ac;
	(void)av;
	void			*hndl;
	makerNibblerSDL	pMaker;

	initLogs();  // init logs functions

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

	ANibblerGui	*nibblerGui = pMaker();
	nibblerGui->init();

	gameLoop(nibblerGui);

	delete nibblerGui;
	dlclose(hndl);

	return EXIT_SUCCESS;
}
