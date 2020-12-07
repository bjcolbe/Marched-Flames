#pragma once

#include <array>
#include <tuple>
#include <vector>
#include <functional>
#include "types/color.h"
#include "types/model.h"
#include "types/vertex.h"

struct MarchingCubes {
	using Point = std::tuple<Vertex, Color>;
	using Cell = std::array<Point, 8>;
	using Triangle = std::array<Point, 3>;

	static const int TableSize = 256;
	static const int SubTableSize = 16;

	static int EdgeTable[TableSize];
	static int TriangleTable[TableSize][SubTableSize];
	static int TriangleTable1D[TableSize * SubTableSize];
	static int Association[12];
	static float isolevel;

	/**
	 * Uses marching cubes to find the faces of a point cloud
	 * @param model An incomplete model consisting of vertices and their colors (point cloud)
	 */
	static void march(float resolution, Model &model, std::vector<int> sdim = {-1, -1, -1});

	/**
	 * Uses parallel marching cubes via OpenCL to find the faces of a point cloud
	 * @param model An incomplete model consisting of vertices and their colors (point cloud)
	 */
	static void marchCL(float resolution, Model &model, std::vector<int> sdim = {-1, -1, -1});

	struct Space {
		int width, height, length;
		float minX, minY, minZ;
		float maxX, maxY, maxZ;
		float averageDistance;
		std::vector<Vertex> vertices;
		std::vector<Color> colors;
		Space(float, Model&, int dimX = -1, int dimY = -1, int dimZ = -1);
		Point at(int, int, int);
		void forEach(std::function<void(Cell)> action);
		static bool isActive(Point);
	};

	private:
		struct Initializer {
			Initializer();
		};

		static Initializer initializer;
};