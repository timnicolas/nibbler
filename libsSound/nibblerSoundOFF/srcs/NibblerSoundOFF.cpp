#include "NibblerSoundOFF.hpp"
#include "Logging.hpp"

NibblerSoundOFF::NibblerSoundOFF() {
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

NibblerSoundOFF::~NibblerSoundOFF() {
}

NibblerSoundOFF::NibblerSoundOFF(NibblerSoundOFF const &src) {
	*this = src;
}

NibblerSoundOFF &NibblerSoundOFF::operator=(NibblerSoundOFF const &rhs) {
	if (this != &rhs) {
		logErr("don't use NibblerSoundOFF copy operator");
	}
	return *this;
}

bool NibblerSoundOFF::_init() {
    return true;
}

bool	NibblerSoundOFF::loadMusic(std::string const & name, std::string const & filename) {
	(void)name;
	(void)filename;
	return true;
}

void	NibblerSoundOFF::update() {
}

bool	NibblerSoundOFF::playMusic(std::string const & name, bool infinitePlay) {
	(void)name;
	(void)infinitePlay;
	return true;
}

bool	NibblerSoundOFF::pause(bool infinitePlay) {
	(void)infinitePlay;
	return true;
}

bool	NibblerSoundOFF::restart() {
	return true;
}

extern "C" {
	ANibblerSound *makeNibblerSoundOFF() {
		return new NibblerSoundOFF();
	}
}
