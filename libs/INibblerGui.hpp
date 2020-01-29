#ifndef INIBBLERGUI_HPP_
#define INIBBLERGUI_HPP_

namespace Input {
	enum eInput {
		NOTHING,
		QUIT,
		MOVE_UP,
		MOVE_DOWN,
		MOVE_LEFT,
		MOVE_RIGHT,
		LOAD_GUI_1,
		LOAD_GUI_2,
		LOAD_GUI_3,
	};
}

class INibblerGui {
	public:
		virtual ~INibblerGui() {}
		virtual	bool			init() = 0;
		virtual Input::eInput	getInput() const = 0;
		virtual	bool			draw() const = 0;

	private:
};

#endif  // INIBBLERGUI_HPP_
