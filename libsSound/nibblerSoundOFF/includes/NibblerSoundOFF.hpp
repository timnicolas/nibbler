
#pragma once

#include "ANibblerSound.hpp"

class NibblerSoundOFF : public ANibblerSound {
	public:
		NibblerSoundOFF();
		virtual ~NibblerSoundOFF();
		NibblerSoundOFF(NibblerSoundOFF const &src);
		NibblerSoundOFF &operator=(NibblerSoundOFF const &rhs);

		virtual bool	loadMusic(std::string const & name, std::string const & filename);
		virtual void	update();
		virtual bool	playMusic(std::string const & name, bool infinitePlay = true);
		virtual bool	pause(bool paused);
		virtual bool	restart();

	private:
		virtual bool	_init();
};
