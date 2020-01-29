#ifndef INIBBLERGUI_HPP_
#define INIBBLERGUI_HPP_

class INibblerGui {
	public:
		virtual	bool	init() = 0;
		virtual	bool	draw() const = 0;

	private:
};

#endif  // INIBBLERGUI_HPP_
