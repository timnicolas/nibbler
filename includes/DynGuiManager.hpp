#ifndef DYNGUIMANAGER_HPP_
#define DYNGUIMANAGER_HPP_

#define NB_GUI 3

#include <dlfcn.h>
#include <array>
#include <utility>
#include <stdexcept>

#include "Logging.hpp"
#include "ANibblerGui.hpp"

class DynGuiManager {
	public:
		DynGuiManager();
		virtual ~DynGuiManager();
		DynGuiManager(DynGuiManager const &src);
		DynGuiManager &operator=(DynGuiManager const &rhs);

		class DynGuiManagerException : public std::runtime_error {
			public:
				DynGuiManagerException();
				explicit DynGuiManagerException(const char* what_arg);
		};

		ANibblerGui	*nibblerGui;

		void		loadGui(uint8_t id);
		uint8_t		getCurrentGuiID() const;

	private:
		void	_quitGui();

		uint8_t		_currentGuiID;
		void		*_hndl;
		static	std::array<std::pair< std::string const, std::string const >, NB_GUI> const	_guiInfos;
};

#endif  // DYNGUIMANAGER_HPP_
