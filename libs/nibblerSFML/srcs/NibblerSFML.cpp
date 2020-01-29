#include "NibblerSFML.hpp"

NibblerSFML::NibblerSFML() {}

NibblerSFML::~NibblerSFML() {
	std::cout << "[INFO]: exit SFML" << std::endl;
}

NibblerSFML::NibblerSFML(NibblerSFML const &src) {
	*this = src;
}

NibblerSFML &NibblerSFML::operator=(NibblerSFML const &rhs) {
	(void)rhs;
	// if (this != &rhs)
	// 	;
	return *this;
}

bool NibblerSFML::init() {
	std::cout << "[INFO]: loading SFML" << std::endl;

    return true;
}

void NibblerSFML::updateInput() {
}

bool NibblerSFML::draw() const {
	return true;
}

extern "C" {
	NibblerSFML *makeNibblerSDL() {
		return new NibblerSFML();
	}
}
