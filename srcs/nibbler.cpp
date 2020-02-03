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

void	initUserData(std::string const & filename) {
	userData.addu("highScore", 0);

	try {
		if (userData.loadFile(filename) == false) {
			// warning when loading settings
		}
	}
	catch(SettingsJson::SettingsException const & e) {
		// file doesn't exist, create it
		logDebug("the file " << filename << " doesn't exist for now");
	}
}

void	initSettings(std::string const & filename) {
	s.addj("screen");
		s.j("screen").addu("width", 1200).setMin(400).setMax(4000);
		s.j("screen").addu("height", 800).setMin(400).setMax(4000);
		s.j("screen").addu("fps", 30).setMin(30).setMax(120);
	s.addu("boardSize", 20).setMin(5).setMax(50);
	s.addu("snakeSize", 4).setMin(1).setMax(40);
	s.addu("speedMs", 100).setMin(30).setMax(1000);
	s.addb("canExitBorder", false);

	try {
		if (s.loadFile(filename) == false) {
			// warning when loading settings
		}
	}
	catch(SettingsJson::SettingsException const & e) {
		logErr(e.what());
	}
}

std::chrono::milliseconds getMs() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch());
}
