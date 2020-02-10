
#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <map>
#include <iostream>
#include "ANibblerSound.hpp"

class NibblerSoundSDL : public ANibblerSound {
	public:
		NibblerSoundSDL();
		virtual ~NibblerSoundSDL();
		NibblerSoundSDL(NibblerSoundSDL const &src);
		NibblerSoundSDL &operator=(NibblerSoundSDL const &rhs);

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

		struct Music {
			std::string	filename;
			Mix_Music *	music;

			Music() : music(nullptr) {}
		};

		std::map<std::string, Music>	_music;
		std::string						_actMusic;

		struct Sound {
			std::string	filename;
			Mix_Chunk *	sound;

			Sound() : sound(nullptr) {}
		};

		std::map<std::string, Sound>	_sound;
};
