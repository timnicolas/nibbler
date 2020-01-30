#pragma once

#include "DynGuiManager.hpp"

class Game {
	public:
		Game();
		Game(Game const &src);
		virtual ~Game();

		Game &operator=(Game const &rhs);

		bool	init();
		void	run();
	protected:
	private:
		DynGuiManager	_dynGuiManager;
};
