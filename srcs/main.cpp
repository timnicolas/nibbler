#include <dlfcn.h>
#include <iostream>

#include "Logging.hpp"
#include "NibblerSDL.hpp"

void	initLogs() {
	// init logging
	#if DEBUG
		logging.setLoglevel(LOGDEBUG);
		logging.setPrintFileLine(LOGWARN, true);
		logging.setPrintFileLine(LOGERROR, true);
		logging.setPrintFileLine(LOGFATAL, true);
	#else
		logging.setLoglevel(LOGINFO);
	#endif
	logDebug("using debug mode");
}

int main(int ac, char const **av) {
	(void)ac;
	(void)av;
	void		*hndl;
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

	NibblerSDL	*nibblerSDL = pMaker();
	nibblerSDL->draw();
	delete nibblerSDL;
	dlclose(hndl);

	return EXIT_SUCCESS;
}
