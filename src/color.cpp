#include "types/color.h"

Color::Color(float r, float g, float b) : r(r), g(g), b(b) {}

int Color::getRGB() const {
	auto r = (int)(this->r * 255.0f) & 0xff;
	auto g = (int)(this->g * 255.0f) & 0xff;
	auto b = (int)(this->b * 255.0f) & 0xff;

	return (r << 16) | (g << 8) | b;
}