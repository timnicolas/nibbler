#ifndef ANIBBLERGUI_HPP_
#define ANIBBLERGUI_HPP_

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

#define SQUARE_COLOR_1		0x323D4D
#define SQUARE_COLOR_2		0x27313D
#define FOOD_COLOR			0xFF0000
#define BORDER_COLOR		0xAAAAAA
#define TEXT_COLOR			0xAAAAAA
#define TEXT_WIN_COLOR		0x00FF00
#define TEXT_GAMEOVER_COLOR	0xFF0000

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
	uint8_t		minBoardSize;
	uint8_t		maxBoardSize;
	struct Rules {
		bool	canExitBorder;
	};
	Rules rules;

	std::vector<Direction::Enum>	direction;
	std::vector<uint32_t>			scores;
	std::vector<bool>				isIA;

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
		virtual	bool	draw(std::vector<std::deque<Vec2>> & snakes, std::deque<Vec2> & food) = 0;

		struct Input {
			bool							quit;
			bool							paused;
			bool							restart;
			std::vector<Direction::Enum>	direction;
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

#endif  // ANIBBLERGUI_HPP_
