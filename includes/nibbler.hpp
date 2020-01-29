#pragma once

#ifndef DEBUG
	#define DEBUG false
#endif
#define DEBUG_FPS_LOW	DEBUG & true
#define FPS 3

#include "ANibblerGui.hpp"

void	initLogs();
void	gameLoop(ANibblerGui * nibblerGui);
