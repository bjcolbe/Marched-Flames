#include "types/color.h"

Color Color::White = Color(1, 1, 1);
Color Color::Black = Color(0, 0, 0);

Color::Color(float r, float g, float b) : r(r), g(g), b(b) {}
Color::Color() : r(0), g(0), b(0) {}

int Color::getRGB() const {
	auto r = (int)(this->r * 255.0f) & 0xff;
	auto g = (int)(this->g * 255.0f) & 0xff;
	auto b = (int)(this->b * 255.0f) & 0xff;

	return (r << 16) | (g << 8) | b;
}

Color Color::operator+(const Color &other) {
	return Color(r + other.r, g + other.g, b + other.b);
}

Color Color::operator/(const float value) {
	return Color(r / value, g / value, b / value);
}

template<typename T>
T lerp(T a, T b, float t) {
	return a + (b - a) * t;
}

Color Color::lerp(Color a, Color b, float t) {
	return Color(
		::lerp(a.r, b.r, t),
		::lerp(a.g, b.g, t),
		::lerp(a.b, b.b, t)
	);
}