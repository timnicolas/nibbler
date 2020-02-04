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
	s.add<SettingsJson>("screen");
		s.j("screen").add<std::string>("name", "nibbler");
		s.j("screen").add<uint64_t>("width", 1200).setMin(400).setMax(4000);
		s.j("screen").add<uint64_t>("height", 800).setMin(400).setMax(4000);
		s.j("screen").add<uint64_t>("fps", 30).setMin(30).setMax(120);
	s.add<std::string>("userDataFilename", "assets/userData.json");
	s.add<uint64_t>("boardSize", 20).setMin(5).setMax(50);
	s.add<uint64_t>("snakeSize", 4).setMin(1).setMax(40);
	s.add<uint64_t>("speedMs", 100).setMin(30).setMax(1000);
	s.add<bool>("canExitBorder", false);

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
