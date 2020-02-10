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

		virtual bool	loadMusic(std::string const & name, std::string const & filename) = 0;
		virtual void	update() = 0;
		virtual bool	playMusic(std::string const & name, bool infinitePlay = true) = 0;
		virtual bool	pause(bool paused) = 0;
		virtual bool	restart() = 0;

	protected:
		virtual	bool	_init() = 0;
};

typedef ANibblerSound *(*nibblerSoundCreator)();
