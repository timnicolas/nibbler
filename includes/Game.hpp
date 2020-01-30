#pragma once

#include "DynGuiManager.hpp"

class Game {
	public:
		Game();
		Game(Game const &src);
		virtual ~Game();

		Game &operator=(Game const &rhs);

		bool	init(std::string title, uint16_t width, uint16_t height, uint8_t boardSize);
		void	run();

		class GameException : public std::runtime_error {
			public:
				GameException();
				explicit GameException(const char* what_arg);
		};
	protected:
	private:
		DynGuiManager		_dynGuiManager;
		GameInfo *			_gameInfo;
		std::deque<Snake>	_snake;
		uint32_t			_speedMs;

		void				_move(ANibblerGui::Input::eDirection direction);
};
