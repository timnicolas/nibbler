#ifndef CIRCLE_HPP_
#define CIRCLE_HPP_

#include <iostream>


class Circle {
	public:
		Circle();
		virtual ~Circle();
		Circle(Circle const &src);
		Circle &operator=(Circle const &rhs);

		virtual void draw() const;
	private:
};

typedef Circle *(*makerCircle)();

#endif  // CIRCLE_HPP_
