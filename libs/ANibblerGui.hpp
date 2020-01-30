#ifndef ANIBBLERGUI_HPP_
#define ANIBBLERGUI_HPP_

#define NO_GUI_LOADED 255

#include <unistd.h>
#include <iostream>

#define TITLE	"nibbler"
#define WIDTH	800
#define HEIGHT	600

class ANibblerGui {
	public:
		ANibblerGui();
		virtual ~ANibblerGui();
		ANibblerGui(ANibblerGui const &src);
		ANibblerGui &operator=(ANibblerGui const &rhs);

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

			Input();
			Input(Input const &src);
			Input &operator=(Input const &rhs);
		};

		Input input;
};

typedef ANibblerGui *(*nibblerGuiCreator)();

#endif  // ANIBBLERGUI_HPP_
