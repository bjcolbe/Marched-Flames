#include "types/vertex.h"

#include <cmath>

using namespace std;

Vertex::Vertex(float x, float y, float z) : x(x), y(y), z(z) {}

float Vertex::sqrMagnitude() {
	return x * x + y * y + z * z;
}

float Vertex::magnitude() {
	return sqrt(sqrMagnitude());
}

Vertex Vertex::operator+(const Vertex &other) const {
	return Vertex(x + other.x, y + other.y, z + other.z);
}

Vertex Vertex::operator-(const Vertex &other) const {
	return Vertex(x - other.x, y - other.y, z - other.z);
}

Vertex Vertex::operator*(float value) const {
	return Vertex(x * value, y * value, z * value);
}

Vertex& Vertex::operator*=(float value) {
	x *= value;
	y *= value;
	z *= value;

	return *this;
}

Vertex operator*(float scalar, const Vertex &vector) {
	return Vertex(vector.x * scalar, vector.y * scalar, vector.z * scalar);
}

Vertex Vertex::operator/(float value) const {
	return Vertex(x / value, y / value, z / value);
}