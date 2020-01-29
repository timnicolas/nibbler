#include "NibblerSDL.hpp"

NibblerSDL::NibblerSDL() :
  _win(nullptr) {}

NibblerSDL::~NibblerSDL() {
	std::cout << "[INFO]: exit SDL" << std::endl;
	SDL_DestroyWindow(_win);
    SDL_Quit();
}

NibblerSDL::NibblerSDL(NibblerSDL const &src) {
	*this = src;
}

NibblerSDL &NibblerSDL::operator=(NibblerSDL const &rhs) {
	(void)rhs;
	// if (this != &rhs)
	// 	;
	return *this;
}

bool NibblerSDL::init() {
	std::cout << "[INFO]: loading SDL" << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "[ERROR]: while loading SDL" << SDL_GetError() << std::endl;
        SDL_Quit();
		return false;
    }

	_win = SDL_CreateWindow("Nibbler", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

	if (_win == nullptr) {
        std::cout << "[ERROR]: while loading SDL" << SDL_GetError() << std::endl;
		SDL_Quit();
		return false;
	}

    return true;
}

bool NibblerSDL::draw() const {
	std::cout << "  ###  " << std::endl;
	std::cout << " #   # " << std::endl;
	std::cout << "#     #" << std::endl;
	std::cout << "#     #" << std::endl;
	std::cout << " #   # " << std::endl;
	std::cout << "  ###  " << std::endl;
	return true;
}

extern "C" {
	NibblerSDL *makeNibblerSDL() {
		return new NibblerSDL();
	}
}
