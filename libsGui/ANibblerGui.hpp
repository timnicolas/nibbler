#ifndef ANIBBLERGUI_HPP_
#define ANIBBLERGUI_HPP_

#define NO_GUI_LOADED 255

#include <unistd.h>
#include <iostream>
#include <deque>

#define SQUARE_COLOR_1	0x323D4D
#define SQUARE_COLOR_2	0x27313D
#define SNAKE_COLOR_1	0x024fd6
#define SNAKE_COLOR_2	0x4C90FF
#define FOOD_COLOR		0xFF0000
#define BORDER_COLOR	0xAAAAAA
#define TEXT_COLOR		0xAAAAAA

#define GET_R(color) ((color >> 16) & 0xFF)
#define GET_G(color) ((color >>  8) & 0xFF)
#define GET_B(color) ((color >>  0) & 0xFF)

#define BORDER_SIZE		5

namespace Direction {
	enum Enum {
		MOVE_UP,
		MOVE_DOWN,
		MOVE_LEFT,
		MOVE_RIGHT,
	};
}

struct GameInfo {
	std::string	title;
	uint16_t	width;
	uint16_t	height;
	uint8_t		boardSize;
	struct Rules {
		bool	canExitBorder;
	};
	Rules rules;

	Direction::Enum	direction;

	bool		paused;
	bool		win;
	bool		gameOver;
	uint32_t	bestScore;

	std::string	font;

	GameInfo();
	void restart();
};

struct Vec2 {  // the Vec2 is a std::deque of struct Vec2
	int	x;
	int	y;

	Vec2();
	Vec2(int x_, int y_);
	bool operator==(Vec2 const & other) const;
};

class ANibblerGui {
	public:
		ANibblerGui();
		virtual ~ANibblerGui();
		ANibblerGui(ANibblerGui const &src);
		ANibblerGui &operator=(ANibblerGui const &rhs);

		virtual	bool	init(GameInfo *gameInfo);
		virtual void	updateInput() = 0;
		virtual	bool	draw(std::deque<Vec2> & snake, std::deque<Vec2> & food) = 0;

		struct Input {
			bool		quit;
			bool		paused;
			bool		restart;
			Direction::Enum	direction;
			uint8_t		loadGuiID;

			Input();
			Input(Input const &src);
			Input &operator=(Input const &rhs);
			void reset();
		};

		Input input;


	protected:
		GameInfo *_gameInfo;

		virtual	bool	_init() = 0;
};

uint32_t mixColor(uint32_t c1, uint32_t c2, float factor);

typedef ANibblerGui *(*nibblerGuiCreator)();

#endif  // ANIBBLERGUI_HPP_
