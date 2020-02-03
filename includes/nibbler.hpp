#pragma once

#ifndef DEBUG
	#define DEBUG false
#endif
#define DEBUG_FPS_LOW	DEBUG & false

#include "DynGuiManager.hpp"
#include "SettingsJson.hpp"

void						initLogs();
void						initSettings(std::string const & filename);
void						initUserData(std::string const & filename);
std::chrono::milliseconds	getMs();

extern SettingsJson s;
extern SettingsJson userData;
