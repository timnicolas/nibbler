#pragma once

#ifndef DEBUG
	#define DEBUG false
#endif
#define DEBUG_FPS_LOW	DEBUG & true
#define FPS 30

#define TITLE		"nibbler"
#define WIDTH		800
#define HEIGHT		600
#define BOARD_SIZE	20

#include "DynGuiManager.hpp"

void						initLogs();
std::chrono::milliseconds	getMs();
