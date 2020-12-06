#pragma once

struct Color {
	static Color White, Black;

	float r, g, b;
	Color(float r, float g, float b);
	Color();
	int getRGB() const;

	static Color lerp(Color a, Color b, float t);

	Color operator+(const Color&);
	Color operator/(const float);
};