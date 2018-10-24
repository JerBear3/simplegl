#include "../gl.hpp"

Color& Color::set(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
	
	return *this;
}
