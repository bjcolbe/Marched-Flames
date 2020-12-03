#pragma once

#include <vector>
#include <string>
#include "types/vertex.h"
#include "types/color.h"

struct Model {
	std::vector<Vertex> vertices;
	std::vector<std::tuple<int, int, int>> faces;
	std::vector<Color> vertexColors;

	/**
	 * Creates a file for the model
	 * @param path Path to write the model to
	 */
	void write(std::string path);
};