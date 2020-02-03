#pragma once

#ifndef DEBUG
	#define DEBUG false
#endif
#define DEBUG_FPS_LOW	DEBUG & true

#include "DynGuiManager.hpp"
#include "SettingsJson.hpp"

void						initLogs();
void						initSettings();
std::chrono::milliseconds	getMs();

extern SettingsJson s;
