#pragma once

#include <functional>
#include "types/model.h"

namespace Graph {
	/**
	 * Creates a set of points from the specified function
	 * @param resolution Granularity of points to acquire
	 * @param width Width of function region
	 * @param height Maximum height of the function
	 * @param length Length of the function region
	 * @param function A function f(x,z) which returns y from the parameters x and z
	 * @return Model created from the function
	 */
	Model create(float resolution, float width, float height, float length, std::function<float(float, float)> function);
}