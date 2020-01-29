#pragma once

#include <iostream>
#include "INibblerGui.hpp"

class NibblerSDL : public INibblerGui {
	public:
		NibblerSDL();
		virtual ~NibblerSDL();
		NibblerSDL(NibblerSDL const &src);
		NibblerSDL &operator=(NibblerSDL const &rhs);

		virtual bool	init();
		virtual bool	draw() const;
	private:
};

typedef NibblerSDL *(*makerNibblerSDL)();
