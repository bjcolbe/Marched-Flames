#pragma once

#include <vector>
#include <string>
#include <tuple>
#include "types/vertex.h"
#include "types/color.h"

struct Model {
	std::vector<std::tuple<int, int, int>> faces;
	std::vector<Vertex> vertices;
	std::vector<Color> colors;

	void scale(float value);

	/**
	 * Creates a file for the model
	 * @param path Path to write the model to
	 * @param format File format for the model
	 */
	void write(std::string path, std::string format = "obj");
};