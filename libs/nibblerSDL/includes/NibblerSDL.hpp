#pragma once

#include <iostream>


class NibblerSDL {
	public:
		NibblerSDL();
		virtual ~NibblerSDL();
		NibblerSDL(NibblerSDL const &src);
		NibblerSDL &operator=(NibblerSDL const &rhs);

		virtual void draw() const;
	private:
};

typedef NibblerSDL *(*makerNibblerSDL)();
