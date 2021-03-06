#pragma once

#include <functional>
#include <vector>
#include <tuple>
#include "types/color.h"
#include "types/vertex.h"

namespace FractalFlames {
	/**
	 * Creates a 3d colored fractal using fractal flames
	 * @param dimensions Fractal dimensions
	 * @param weights Weights correlating to variations
	 * @return Vertex and color of each point in the fractal
	 */
	std::tuple<std::vector<Vertex>, std::vector<Color>> fractal(int _iterations, int _threads, std::vector<int> _dimensions, std::vector<float> _weights);
	std::tuple<std::vector<Vertex>, std::vector<Color>> cpufractal(int _iterations, std::vector<int> _dimensions, std::vector<float> _weights);
}
