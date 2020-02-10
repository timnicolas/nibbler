
#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "ANibblerSound.hpp"

class NibblerSoundSDL : public ANibblerSound {
	public:
		NibblerSoundSDL();
		virtual ~NibblerSoundSDL();
		NibblerSoundSDL(NibblerSoundSDL const &src);
		NibblerSoundSDL &operator=(NibblerSoundSDL const &rhs);

	private:
		virtual bool	_init();
};
