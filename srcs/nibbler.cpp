#include <unistd.h>
#include <chrono>

#include "nibbler.hpp"
#include "Logging.hpp"
#include "ANibblerGui.hpp"

SettingsJson s;
SettingsJson userData;

void	initLogs() {
	// init logging
	#if DEBUG
		logging.setLoglevel(LOGDEBUG);
		logging.setPrintFileLine(LOGWARN, true);
		logging.setPrintFileLine(LOGERROR, true);
		logging.setPrintFileLine(LOGFATAL, true);
	#else
		logging.setLoglevel(LOGINFO);
	#endif
	logDebug("using debug mode");
}

bool	initSettings(std::string const & filename) {
	s.name("settings").description("main settings");
	s.add<SettingsJson>("screen");
		s.j("screen").add<std::string>("name", "nibbler").setDescription("name of the game");
		s.j("screen").add<uint64_t>("width", 1200).setMin(400).setMax(4000).setDescription("width of the screen");
		s.j("screen").add<uint64_t>("height", 800).setMin(400).setMax(4000).disableInFile(true)
			.setDescription("height of the screen /!\\ automatically calculed");
		s.j("screen").add<uint64_t>("fps", 30).setMin(30).setMax(120).setDescription("framerate");
	s.add<std::string>("userDataFilename", "assets/userData.json").disableInFile(true);
	s.add<std::string>("font", "assets/fonts/Pacifico.json").setDescription("main font ttf file");
	s.add<uint64_t>("boardSize", 20).setMin(8).setMax(50).setDescription("size of the snake board");
	s.add<uint64_t>("nbPlayers", 1).setMin(1).setMax(2).setDescription("number of players");
	s.add<SettingsJson>("ai");
		s.j("ai").add<uint64_t>("nbAI", 0).setMin(0).setMax(10).setDescription("number of IA on the game");
		s.j("ai").add<uint64_t>("strength", 10).setMin(1).setMax(100).setDescription("this is the strenght of the AI");
		s.j("ai").add<uint64_t>("changeDirProba", 10).setMin(1).setMax(100)
			.setDescription("probability to randomly change direction");
	s.add<uint64_t>("startGui", 0).setMin(0).setMax(2).setDescription("id of the startong GUI");
	s.add<uint64_t>("startSound", 1).setMin(0).setMax(1).setDescription("id of the starting sound (0 for OFF)");
	s.add<uint64_t>("snakeSize", 4).setMin(1).setMax(25).setDescription("starting size of the snake");
	s.add<uint64_t>("nbFood", 1).setMin(0).setMax(10).setDescription("number of food on the board");
	s.add<uint64_t>("speedMs", 100).setMin(30).setMax(1000).setDescription("starting speed of the snake");
	s.add<bool>("canExitBorder", false).setDescription("if true, the snakes cannot die in front of the borders");
	s.add<bool>("pauseOnStart", true).setDescription("if true, the game will start in pause mode");
	s.add<std::string>("masterMusic", "assets/music/masterMusic.wav");
	s.add<std::string>("soundWin", "assets/music/win.wav");
	s.add<std::string>("soundLoose", "assets/music/loose.wav");

	try {
		if (s.loadFile(filename) == false) {
			// warning when loading settings
			return false;
		}
	}
	catch(SettingsJson::SettingsException const & e) {
		logErr(e.what());
		return false;
	}
	return true;
}

bool	initUserData(std::string const & filename) {
	userData.name("userData").description("all informations about the player");
	userData.add<uint64_t>("highScore", 0);

	try {
		if (userData.loadFile(filename) == false) {
			// warning when loading settings
			return false;
		}
	}
	catch(SettingsJson::SettingsException const & e) {
		// file doesn't exist, create it at the end
		logDebug("the file " << filename << " doesn't exist for now");
		return false;
	}
	return true;
}

bool	saveUserData(std::string const & filename) {
	try {
		userData.saveToFile(filename);
	}
	catch(SettingsJson::SettingsException const & e) {
		logErr(e.what());
		return false;
	}
	return true;
}

std::chrono::milliseconds getMs() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
}
