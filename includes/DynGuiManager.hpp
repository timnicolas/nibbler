#ifndef DYNGUIMANAGER_HPP_
#define DYNGUIMANAGER_HPP_

#define NO_GUI_LOADED 255

#include <dlfcn.h>
#include <map>
#include <utility>
#include <stdexcept>

#include "Logging.hpp"
#include "ANibblerGui.hpp"
// #include "NibblerSDL.hpp"
// #include "NibblerSFML.hpp"

class DynGuiManager {
	public:
		DynGuiManager();
		virtual ~DynGuiManager();
		DynGuiManager(DynGuiManager const &src);
		DynGuiManager &operator=(DynGuiManager const &rhs);

		void	loadGui(uint8_t id);

		class DynGuiManagerException : public std::runtime_error {
			public:
				DynGuiManagerException();
				explicit DynGuiManagerException(const char* what_arg);
		};

		ANibblerGui	*nibblerGui;

	private:
		void	_quitGui();

		uint8_t		_currentGuiID;
		void		*_hndl;
		static	std::map<uint8_t, std::pair< std::string const, std::string const > > const	_guiNames;
};

#endif  // DYNGUIMANAGER_HPP_
