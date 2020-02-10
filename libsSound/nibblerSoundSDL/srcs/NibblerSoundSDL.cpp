#include "NibblerSoundSDL.hpp"
#include "Logging.hpp"

NibblerSoundSDL::NibblerSoundSDL() {
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

NibblerSoundSDL::~NibblerSoundSDL() {
	logInfo("exit sound SDL");
	for (auto it = _music.begin(); it != _music.end(); it++) {
		Mix_FreeMusic(it->second.music);
	}
	Mix_CloseAudio();
    SDL_Quit();
}

NibblerSoundSDL::NibblerSoundSDL(NibblerSoundSDL const &src) {
	*this = src;
}

NibblerSoundSDL &NibblerSoundSDL::operator=(NibblerSoundSDL const &rhs) {
	if (this != &rhs) {
		logErr("don't use NibblerSoundSDL copy operator");
	}
	return *this;
}

bool NibblerSoundSDL::_init() {
	logInfo("loading sound SDL");

	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		return false;

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) {
		logErr("while loading sound SDL: " << Mix_GetError());
	}

    return true;
}

bool	NibblerSoundSDL::loadMusic(std::string const & name, std::string const & filename) {
	Music newMusic;
	newMusic.filename = filename;
	newMusic.music = Mix_LoadMUS(filename.c_str());
	if (newMusic.music == nullptr) {
		logErr("while loading music " << filename << ": " << Mix_GetError());
		return false;
	}
	_music.insert(std::pair<std::string, Music>(name, newMusic));
	return true;
}

void	NibblerSoundSDL::update() {
}

bool	NibblerSoundSDL::playMusic(std::string const & name, bool infinitePlay) {
	int nb = (infinitePlay) ? -1 : 1;

	if (Mix_PlayMusic(_music[name].music, nb) < 0) {
		logErr("error: " << Mix_GetError());
		return false;
	}
	return true;
}

bool	NibblerSoundSDL::pause(bool infinitePlay) {
	if (infinitePlay)
		Mix_PauseMusic();
	else
		Mix_ResumeMusic();
	return true;
}

bool	NibblerSoundSDL::restart() {
	Mix_RewindMusic();
	Mix_ResumeMusic();
	return true;
}

extern "C" {
	ANibblerSound *makeNibblerSoundSDL() {
		return new NibblerSoundSDL();
	}
}
