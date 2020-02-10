#include "NibblerSoundSDL.hpp"
#include "Logging.hpp"

NibblerSoundSDL::NibblerSoundSDL() {
	// init logging
	#if DEBUG
		logging.setLoglevel(LOGDEBUG);
		logging.setPrintFileLine(LOGWARN, true);
		logging.setPrintFileLine(LOGERROR, true);
		logging.setPrintFileLine(LOGFATAL, true);
	#else
		logging.setLoglevel(LOGINFO);
	#endif
}

NibblerSoundSDL::~NibblerSoundSDL() {
	logInfo("exit SDL");
    // SDL_Quit();
}

NibblerSoundSDL::NibblerSoundSDL(NibblerSoundSDL const &src) {
	*this = src;
}

NibblerSoundSDL &NibblerSoundSDL::operator=(NibblerSoundSDL const &rhs) {
	if (this != &rhs) {
		logErr("don't use NibblerSoundSDL copy operator");
	}
	return *this;
}

bool NibblerSoundSDL::_init() {
	logInfo("loading SDL");

    return true;
}

extern "C" {
	ANibblerSound *makeNibblerSoundSDL() {
		return new NibblerSoundSDL();
	}
}
