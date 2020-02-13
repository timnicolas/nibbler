#pragma once

#define NO_GUI_LOADED 255

#include <unistd.h>
#include <iostream>
#include <deque>
#include <vector>

#define SNAKE_1_COLOR_1 0x024fd6  // #024fd6
#define SNAKE_1_COLOR_2 0x4C90FF  // #4C90FF
#define SNAKE_2_COLOR_1 0x02d64f  // #02d64f
#define SNAKE_2_COLOR_2 0x4CFF90  // #4CFF90
#define SNAKE_3_COLOR_1 0xd64f02  // #d64f02
#define SNAKE_3_COLOR_2 0xFF904C  // #FF904C
#define SNAKE_4_COLOR_1 0x4f02d6  // #4f02d6
#define SNAKE_4_COLOR_2 0x904CFF  // #904CFF
#define SNAKE_5_COLOR_1 0xd6024f  // #d6024f
#define SNAKE_5_COLOR_2 0xFF4C90  // #FF4C90
#define SNAKE_6_COLOR_1 0x4fd602  // #4fd602
#define SNAKE_6_COLOR_2 0x90FF4C  // #90FF4C

#define SQUARE_COLOR_1		0x323D4D  // #323D4D
#define SQUARE_COLOR_2		0x27313D  // #27313D
#define FOOD_COLOR			0xFF0000  // #CC0000
#define BONUS_COLOR			0xCCCC00  // #CCCC00
#define WALL_COLOR			0x1C1C1C  // #1C1C1C
#define BORDER_COLOR		0xAAAAAA  // #AAAAAA
#define TEXT_COLOR			0xAAAAAA  // #AAAAAA
#define TEXT_WIN_COLOR		0x00FF00  // #00FF00
#define TEXT_GAMEOVER_COLOR	0xFF0000  // #FF0000

#define GET_R(color) ((color >> 16) & 0xFF)
#define GET_G(color) ((color >>  8) & 0xFF)
#define GET_B(color) ((color >>  0) & 0xFF)

#define BORDER_SIZE		5

#define HEIGHT_RATIO	0.7  // ratio of height from width

namespace Direction {
	enum Enum {
		MOVE_UP = 0,
		MOVE_DOWN = 1,
		MOVE_LEFT = 2,
		MOVE_RIGHT = 3,
	};
}

struct Vec2 {  // the Vec2 is a std::deque of struct Vec2
	int	x;
	int	y;

	Vec2();
	Vec2(int x_, int y_);
	bool operator==(Vec2 const & other) const;
};

struct GameInfo {
	// snake informations
	std::vector<std::deque<Vec2>>	snakes;
	std::vector<Direction::Enum>	direction;
	std::vector<uint32_t>			scores;
	std::vector<bool>				isIA;
	std::vector<uint16_t>			nbBonus;

	std::deque<Vec2>				food;
	std::deque<Vec2>				bonus;
	struct Wall {
		Vec2	pos;
		int		life;
	};
	std::deque<Wall>				wall;
	std::string	title;
	uint16_t	realWidth;
	uint16_t	realHeight;
	uint16_t	width;
	uint16_t	height;
	uint8_t		boardSize;
	uint8_t		minBoardSize;
	uint8_t		maxBoardSize;
	struct Rules {
		bool	canExitBorder;
	};
	Rules rules;


	bool		paused;
	bool		win;
	bool		gameOver;
	uint32_t	bestScore;
	int			nbPlayers;
	int			winnerID;

	std::string	font;

	explicit GameInfo(int nbPlayers_);
	void restart();
};

class ANibblerGui {
	public:
		ANibblerGui();
		virtual ~ANibblerGui();
		ANibblerGui(ANibblerGui const &src);
		ANibblerGui &operator=(ANibblerGui const &rhs);

		virtual	bool	init(GameInfo *gameInfo);
		virtual void	updateInput() = 0;
		virtual	bool	draw() = 0;

		struct Input {
			bool							quit;
			bool							paused;
			bool							restart;
			std::vector<Direction::Enum>	direction;
			std::vector<bool>				usingBonus;
			uint8_t							loadGuiID;

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
uint32_t getColor(int id, int colorNb);

typedef ANibblerGui *(*nibblerGuiCreator)();
