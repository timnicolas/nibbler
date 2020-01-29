#ifndef ANIBBLERGUI_HPP_
#define ANIBBLERGUI_HPP_

#include <unistd.h>
#include <iostream>


#define TITLE	"nibbler"
#define WIDTH	800
#define HEIGHT	600

class ANibblerGui {
	public:
		virtual ~ANibblerGui() {}
		virtual	bool	init() = 0;
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

			Input() : quit(false), direction(MOVE_UP), loadGuiID(0) {}
		};

		Input input;
};

#endif  // ANIBBLERGUI_HPP_
