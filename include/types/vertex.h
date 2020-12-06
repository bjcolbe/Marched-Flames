#pragma once

#include <ostream>

struct Vertex {
	float x, y, z;
	Vertex(float x = 0, float y = 0, float z = 0);

	float sqrMagnitude();
	float magnitude();

	Vertex operator+(const Vertex &other) const;
	Vertex operator-(const Vertex &other) const;
	Vertex operator*(float value) const;
	Vertex& operator*=(float value);
	friend Vertex operator*(float scalar, const Vertex &vector);
	Vertex operator/(float value) const;
};