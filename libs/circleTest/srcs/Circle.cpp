#include "Circle.hpp"

Circle::Circle() {
}

Circle::~Circle() {
}

Circle::Circle(Circle const &src) {
	*this = src;
}

Circle &Circle::operator=(Circle const &rhs) {
	if (this != &rhs)
		;
	return *this;
}

void Circle::draw() const {
	std::cout << "  ###  " << std::endl;
	std::cout << " #   # " << std::endl;
	std::cout << "#     #" << std::endl;
	std::cout << "#     #" << std::endl;
	std::cout << " #   # " << std::endl;
	std::cout << "  ###  " << std::endl;
}

extern "C" {
	Circle *makeCircle() {
		return new Circle();
	}
}
