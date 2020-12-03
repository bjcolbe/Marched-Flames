#pragma once

#include <vector>
#include <tuple>
#include "types/color.h"
#include "types/vertex.h"

namespace MarchingCubes {
	/**
	 * Uses the marching cubes algorithm to find the faces of a point cloud
	 * @param points Point cloud
	 * @return Mesh faces
	 */
	std::vector<std::tuple<int, int, int>> march(std::vector<Vertex> points);
}