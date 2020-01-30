#ifndef ANIBBLERGUI_HPP_
#define ANIBBLERGUI_HPP_

#define NO_GUI_LOADED 255

#include <unistd.h>
#include <iostream>

struct GameInfo {
	std::string	title;
	uint16_t	width;
	uint16_t	height;
	uint8_t		boardSize;
};

class ANibblerGui {
	public:
		virtual ~ANibblerGui() {}
		virtual	bool	init(GameInfo *gameInfo) = 0;
		virtual void	updateInput() = 0;
		virtual	bool	draw() = 0;

		struct Input {
			bool		quit;
			enum eDirection {
				MOVE_UP,
				MOVE_DOWN,
				MOVE_LEFT,
				MOVE_RIGHT,
			};
			eDirection	direction;
			uint8_t		loadGuiID;

			Input() : quit(false), direction(MOVE_UP), loadGuiID(NO_GUI_LOADED) {}
		};

		Input input;


	protected:
		GameInfo *_gameInfo;
};

typedef ANibblerGui *(*nibblerGuiCreator)();

#endif  // ANIBBLERGUI_HPP_
