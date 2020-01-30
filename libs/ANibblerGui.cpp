#include "ANibblerGui.hpp"

ANibblerGui::ANibblerGui() {
}

ANibblerGui::~ANibblerGui() {
}

ANibblerGui::ANibblerGui(ANibblerGui const &src) {
	*this = src;
}

ANibblerGui &ANibblerGui::operator=(ANibblerGui const &rhs) {
	if (this != &rhs) {
		input = rhs.input;
	}
	return *this;
}

// -- Input --------------------------------------------------------------------


ANibblerGui::Input::Input()
: quit(false), direction(MOVE_UP), loadGuiID(NO_GUI_LOADED) {
}

ANibblerGui::Input::Input(ANibblerGui::Input const &src) {
	*this = src;
}

ANibblerGui::Input &ANibblerGui::Input::operator=(ANibblerGui::Input const &rhs) {
	if (this != &rhs) {
		direction = rhs.direction;
		loadGuiID = rhs.loadGuiID;
	}
	return *this;
}
