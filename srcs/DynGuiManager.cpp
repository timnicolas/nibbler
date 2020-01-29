#include "DynGuiManager.hpp"

std::map<uint8_t, char const *> const	DynGuiManager::_guiNames = {
	{0, "libNibblerSDL.so"},
	{1, "libNibblerSFML.so"}
};

DynGuiManager::DynGuiManager()
: _currentGuiID(NO_GUI_LOADED),
  _hndl(nullptr),
  _nibblerGui(nullptr) {
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
	delete _nibblerGui;
	dlclose(_hndl);

	_currentGuiID = NO_GUI_LOADED;
}

void	DynGuiManager::loadGui(uint8_t id) {
	// quit current gui before loading a new one
	if (_currentGuiID != NO_GUI_LOADED) {
		_quitGui();
	}

	// load librairy
	_hndl = dlopen(_guiNames.at(id), RTLD_LAZY);
	if (_hndl == NULL) {
		throw DynGuiManagerException(dlerror());
	}

	// void	*mkr = dlsym(_hndl, "makeNibblerSDL");
	// if (mkr == NULL) {
	// 	throw DynGuiManagerException(dlerror());
	// }

	// makerNibblerSDL		pMaker;
	// pMaker = reinterpret_cast<makerNibblerSDL>(mkr);
	// nibblerGui = pMaker();

	_currentGuiID = id;
}

// -- Exceptions errors --------------------------------------------------------
DynGuiManager::DynGuiManagerException::DynGuiManagerException()
: std::runtime_error("[DynGuiManagerError]") {}

DynGuiManager::DynGuiManagerException::DynGuiManagerException(const char* what_arg)
: std::runtime_error(std::string(std::string("[DynGuiManagerError] ") + what_arg).c_str()) {}
