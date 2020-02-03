#pragma once

#include "DynGuiManager.hpp"

class Game {
	public:
		Game();
		Game(Game const &src);
		virtual ~Game();

		Game &operator=(Game const &rhs);

		bool	init(uint16_t width, uint16_t height, uint8_t boardSize);
		void	run();
		void	restart();

		class GameException : public std::runtime_error {
			public:
				GameException();
				explicit GameException(const char* what_arg);
		};

	private:
		DynGuiManager		_dynGuiManager;
		GameInfo *			_gameInfo;
		std::deque<Vec2>	_snake;
		Vec2				_lastDeletedSnake;
		std::deque<Vec2>	_food;
		uint32_t			_speedMs;
		uint8_t				_needExtend;

		void				_move(Direction::Enum direction);
		void				_updateFood();
		void				_update();
};
