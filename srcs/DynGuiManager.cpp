#include "DynGuiManager.hpp"

// contain dynamic libs info
std::array<std::pair< std::string const, std::string const >, NB_GUI> const	DynGuiManager::_guiInfos = {{
	{"libNibblerSDL.so", "makeNibblerSDL"},
	{"libNibblerSFML.so", "makeNibblerSFML"}
}};

DynGuiManager::DynGuiManager()
: nibblerGui(nullptr),
  _currentGuiID(NO_GUI_LOADED),
  _hndl(nullptr) {
}

DynGuiManager::~DynGuiManager() {
	if (_currentGuiID == NO_GUI_LOADED) {
		_quitGui();
	}
}

DynGuiManager::DynGuiManager(DynGuiManager const &src) {
	*this = src;
}

DynGuiManager &DynGuiManager::operator=(DynGuiManager const &rhs) {
	if (this != &rhs) {
		_currentGuiID = rhs._currentGuiID;
	}
	return *this;
}

void	DynGuiManager::_quitGui() {
	delete nibblerGui;
	dlclose(_hndl);

	_currentGuiID = NO_GUI_LOADED;
}

void	DynGuiManager::loadGui(uint8_t id) {
	// quit current gui before loading a new one
	if (_currentGuiID != NO_GUI_LOADED) {
		_quitGui();
	}

	// load librairy
	_hndl = dlopen(_guiInfos[id].first.c_str(), RTLD_LAZY);
	if (_hndl == NULL) {
		throw DynGuiManagerException(dlerror());
	}

	// get the correct nibblerGuiCreator
	void	*mkr = dlsym(_hndl, _guiInfos[id].second.c_str());
	if (mkr == NULL) {
		throw DynGuiManagerException(dlerror());
	}

	// construct the gui
	nibblerGui = reinterpret_cast<nibblerGuiCreator>(mkr)();

	_currentGuiID = id;
}

// -- Exceptions errors --------------------------------------------------------
DynGuiManager::DynGuiManagerException::DynGuiManagerException()
: std::runtime_error("[DynGuiManagerError]") {}

DynGuiManager::DynGuiManagerException::DynGuiManagerException(const char* what_arg)
: std::runtime_error(std::string(std::string("[DynGuiManagerError] ") + what_arg).c_str()) {}
