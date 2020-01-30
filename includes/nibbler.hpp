#pragma once

#ifndef DEBUG
	#define DEBUG false
#endif
#define DEBUG_FPS_LOW	DEBUG & true
#define FPS 30

#define WIDTH		800
#define HEIGHT		600
#define BOARD_SIZE	20
#define START_SIZE	4
#define SPEED		100  // time to go to the next block

#include "DynGuiManager.hpp"

void						initLogs();
std::chrono::milliseconds	getMs();
