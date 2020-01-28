#include <dlfcn.h>
#include <iostream>

#include "Logging.hpp"
#include "Circle.hpp"

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
	makerCircle	pMaker;

	initLogs();  // init logs functions

	// load librairy
	hndl = dlopen("libs/circleTest/libcircle.so", RTLD_LAZY);
	if (hndl == NULL) {
		std::cerr << "dlopen : " << dlerror() << std::endl;
		return EXIT_FAILURE;
	}

	// load makeCircle function
	void	*mkr = dlsym(hndl, "makeCircle");
	if (mkr == NULL) {
		std::cerr << "dlsym : " << dlerror() << std::endl;
		return EXIT_FAILURE;
	}
	pMaker = reinterpret_cast<makerCircle>(mkr);


	// create and use instance of class Circle
	Circle	*myCircle = pMaker();
	myCircle->draw();
	dlclose(hndl);

	return EXIT_SUCCESS;
}
