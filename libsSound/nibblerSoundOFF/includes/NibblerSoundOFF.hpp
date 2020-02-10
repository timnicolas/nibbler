
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

		virtual bool	loadSound(std::string const & name, std::string const & filename, int soundLevel);
		virtual bool	playSound(std::string const & name, int channel = 0);
		virtual bool	stopAllSounds();
		virtual bool	stopSound(int channel);

	private:
		virtual bool	_init(int nbSoundChannels);
};
