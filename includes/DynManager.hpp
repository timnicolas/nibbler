#pragma once

#include <dlfcn.h>
#include <vector>
#include <utility>
#include <stdexcept>

#include "Logging.hpp"

#define NO_DYN_LOADED 255


template<class T>
class DynManager {
	public:
		DynManager() : obj(nullptr), _currentID(NO_DYN_LOADED), _hndl(nullptr) {}
		virtual ~DynManager() {
			unload();
		}
		DynManager(DynManager const &src) { *this = src; }
		DynManager &operator=(DynManager const &rhs) {
			if (this != &rhs) {
				_currentID = rhs._currentID;
			}
			return *this;
		}

		class DynManagerException : public std::runtime_error {
			public:
				DynManagerException() : std::runtime_error("[DynManagerError]") {}
				explicit DynManagerException(const char* what_arg) :
					std::runtime_error(std::string(std::string("[DynGuiManagerError] ") + what_arg).c_str()) {}
		};

		T *		obj;

		typedef T * (*createPtr)();
		void		load(uint8_t id) {
			// quit current dyn before loading a new one
			unload();

			// load librairy
			_hndl = dlopen(_infos[id].first.c_str(), RTLD_LAZY);
			if (_hndl == NULL) {
				throw DynManagerException(dlerror());
			}

			// get the correct creator
			void *	mkr = dlsym(_hndl, _infos[id].second.c_str());
			if (mkr == NULL) {
				throw DynManagerException(dlerror());
			}

			// construct the gui
			obj = reinterpret_cast<createPtr>(mkr)();

			_currentID = id;
		}
		void		unload() {
			if (_currentID != NO_DYN_LOADED) {
				delete obj;
				dlclose(_hndl);

				_currentID = NO_DYN_LOADED;
			}
		}
		uint8_t		getCurrentID() const { return _currentID; }
		uint8_t		getNbDyn() const { return _infos.size(); }

		int		addDyn(std::string const & libFile, std::string const & creatorName) {
			_infos.push_back(std::pair<std::string const, std::string const>(libFile, creatorName));
			return _infos.size() - 1;  // return the ID
		}

	private:
		uint8_t		_currentID;
		void		*_hndl;
		std::vector<std::pair<std::string const, std::string const>> _infos;
};
