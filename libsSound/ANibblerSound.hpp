#pragma once

#define NO_GUI_LOADED 255

#include <unistd.h>
#include <iostream>
#include <deque>
#include <vector>

class ANibblerSound {
	public:
		ANibblerSound();
		virtual ~ANibblerSound();
		ANibblerSound(ANibblerSound const &src);
		ANibblerSound &operator=(ANibblerSound const &rhs);

		virtual	bool	init();

	protected:
		virtual	bool	_init() = 0;
};

typedef ANibblerSound *(*nibblerSoundCreator)();
