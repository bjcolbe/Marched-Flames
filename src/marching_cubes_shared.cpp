#include "marching_cubes.h"

#include <cstdio>
#include <ios>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <limits>
#include <iostream>

using namespace std;
using Space = MarchingCubes::Space;
using Point = MarchingCubes::Point;
using Cell = MarchingCubes::Cell;

int MarchingCubes::EdgeTable[TableSize];
int MarchingCubes::TriangleTable[TableSize][SubTableSize];
int MarchingCubes::TriangleTable1D[TableSize * SubTableSize];
int MarchingCubes::Association[12]{1, 2, 3, 0, 5, 6, 7, 4, 4, 5, 6, 7};
float MarchingCubes::isolevel = 0.5;
MarchingCubes::Initializer MarchingCubes::initializer;

MarchingCubes::Initializer::Initializer() {
	ifstream edges("edge.table");

	for(auto i = 0; i < TableSize; i++)
		edges >> hex >> EdgeTable[i];

	edges.close();

	ifstream triangles("tri.table");

	for(auto i = 0; i < TableSize; i++) {
		string line;
		getline(triangles, line);

		stringstream lineStream(line);

		for(auto j = 0; j < SubTableSize; j++) {
			string value;
			getline(lineStream, value, ',');

			TriangleTable[i][j] = stoi(value);
		}
	}

	triangles.close();

	for(auto j = 0; j < SubTableSize; j++)
		for(auto i = 0; i < TableSize; i++)
			TriangleTable1D[TableSize * j + i] = TriangleTable[i][j];
}

MarchingCubes::Space::Space(float resolution, Model &model) {
	if(model.vertices.size() != model.colors.size()) {
		cerr << "Model vertices " << model.colors.size() << " are not the same amount as vertex colors " << model.colors.size() << endl;
		return;
	}

	if(model.vertices.size() == 0)
		return;

	constexpr auto inf = numeric_limits<float>::infinity();
	minX = minY = minZ = inf;
	maxX = maxY = maxZ = -inf;

	for(auto v : model.vertices) {
		if(v.x < minX)
			minX = v.x;
		if(v.x > maxX)
			maxX = v.x;

		if(v.y < minY)
			minY = v.y;
		if(v.y > maxY)
			maxY = v.y;

		if(v.z < minZ)
			minZ = v.z;
		if(v.z > maxZ)
			maxZ = v.z;
	}

	averageDistance = 0.0f;

	for(auto i = (int)model.vertices.size() - 1; i > 0; i--) {
		auto localAverage = 0.0f;

		for(auto j = i - 1; j >= 0; j--) {
			auto v1 = model.vertices[i];
			auto v2 = model.vertices[j];
			// printf("\t|<%.1f, %.1f, %.1f> - <%.1f, %.1f, %.1f>| = %f\n", v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, (v1 - v2).magnitude());

			localAverage += (v1 - v2).magnitude();
		}

		averageDistance += localAverage / i;
	}

	averageDistance /= model.vertices.size();

	if(averageDistance == 0) {
		cerr << "Average distance between points is zero" << endl;
		return;
	}

	auto step = resolution / averageDistance;
	width = (int)ceil((maxX - minX) * step);
	height = (int)ceil((maxY - minY) * step);
	length = (int)ceil((maxZ - minZ) * step);

	printf("Space\n");
	printf("\tBounds: (%.2f, %.2f) (%.2f, %.2f) (%.2f, %.2f)\n", minX, maxX, minY, maxY, minZ, maxZ);
	printf("\tSize: %f, %f, %f\n", maxX - minX, maxY - minY, maxZ - minZ);
	printf("\tAverage Distance: %f\n", averageDistance);
	printf("\tDimensions: %i, %i, %i\n", width, height, length);

	for(auto j = 0; j < height; j++)
	for(auto k = 0; k < length; k++)
	for(auto i = 0; i < width; i++) {
		vertices.push_back({(float)i, (float)j, (float)k});
		colors.push_back({-1, -1, -1});
	}

	for(auto index = (int)model.vertices.size() - 1; index >= 0; index--) {
		auto v = model.vertices[index];
		auto c = model.colors[index];

		auto i = (int)round((width - 1) * (v.x - minX) / (maxX - minX));
		auto j = (int)round((height - 1) * (v.y - minY) / (maxY - minY));
		auto k = (int)round((length - 1) * (v.z - minZ) / (maxZ - minZ));

		colors[width * (length * j + k) + i] = c;
	}
}

Point Space::at(int i, int j, int k) {
	auto index = width * (length * j + k) + i;
	return {vertices[index], colors[index]};
}

void Space::forEach(function<void(Cell)> action) {
	for(auto i = 0; i < width - 1; i++)
		for(auto j = 0; j < height - 1; j++)
			for(auto k = 0; k < length - 1; k++) {
				// Cell cell{{
				// 	at(i, j, k), //0
				// 	at(i + 1, j, k), //1
				// 	at(i + 1, j, k + 1), //2
				// 	at(i, j, k + 1), //3
				// 	at(i, j + 1, k), //4
				// 	at(i + 1, j + 1, k), //5
				// 	at(i + 1, j + 1, k + 1), //6
				// 	at(i, j + 1, k + 1), //7
				// }};

				action({{
					at(i, j, k),
					at(i, j + 1, k),
					at(i, j + 1, k + 1),
					at(i, j, k + 1),
					at(i + 1, j, k),
					at(i + 1, j + 1, k),
					at(i + 1, j + 1, k + 1),
					at(i + 1, j, k + 1),
				}});
			}
}

bool Space::isActive(Point point) {
	auto color = get<1>(point);
	return color.r >= 0 || color.g >= 0 || color.b >= 0;
}