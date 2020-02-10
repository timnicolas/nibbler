#include "ANibblerSound.hpp"
#include "Logging.hpp"

ANibblerSound::ANibblerSound() {}

ANibblerSound::~ANibblerSound() {
}

ANibblerSound::ANibblerSound(ANibblerSound const &src) {
	*this = src;
}

ANibblerSound &ANibblerSound::operator=(ANibblerSound const &rhs) {
	if (this != &rhs) {
		logWarn("don't copy ANibblerSound");
	}
	return *this;
}

bool ANibblerSound::init(int nbSoundChannels) {
	return _init(nbSoundChannels);
}
