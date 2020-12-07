#include "marching_cubes.h"

#include <cmath>
#include <limits>
#include <vector>
#include <chrono>

using namespace std;
using Point = MarchingCubes::Point;
using Cell = MarchingCubes::Cell;
using Triangle = MarchingCubes::Triangle;
using Space = MarchingCubes::Space;

Vertex lerp(Vertex a, Vertex b, float t) {
	float v1 = t == 0.5 ? 0 : (t < 0.5 ? 0 : 1);
	float v2 = t == 0.5 ? 0 : (t > 0.5 ? 0 : 1);

	return a + (MarchingCubes::isolevel - v1) * (b - a) / (v2 - v1);
}

vector<Triangle> polygonize(Cell cell) {
	array<Vertex, 12> vertices;
	array<Color, 12> colors;
	vector<Triangle> triangles;
	auto cubeIndex = 0;

	for(auto i = 0u; i < cell.size(); i++)
		if(Space::isActive(cell[i]))
			cubeIndex |= 1 << i;

	if(MarchingCubes::EdgeTable[cubeIndex] == 0)
		return triangles;

	for(auto i = 0; i < 12; i++) {
		if(!(MarchingCubes::EdgeTable[cubeIndex] & (1 << i)))
			continue;

		auto j = i % 8;
		auto k = MarchingCubes::Association[i];

		auto a = cell[j], b = cell[k];
		auto t = Space::isActive(a) && Space::isActive(b) ? 0.5 : (Space::isActive(b) ? 1 : 0);

		vertices[i] = lerp(get<0>(a), get<0>(b), t);
		colors[i] = Color::lerp(get<1>(a), get<1>(b), t);
	}

	auto pointAt = [&](int i) -> Point {
		auto index = MarchingCubes::TriangleTable[cubeIndex][i];
		return {vertices[index], colors[index]};
	};

	for(auto i = 0; MarchingCubes::TriangleTable[cubeIndex][i] != -1; i += 3)
		triangles.push_back({
			pointAt(i),
			pointAt(i + 1),
			pointAt(i + 2)
		});

	return triangles;
}

void MarchingCubes::march(float resolution, Model &model, vector<int> sdim) {
	Space space(resolution, model, sdim[0], sdim[1], sdim[2]);

	model.vertices.clear();
	model.colors.clear();

	int count = 0;
	float totalMarchTime = 0;

	space.forEach([&](Cell cell) {
		auto start = chrono::high_resolution_clock::now();
		auto faces = polygonize(cell);

		auto end = chrono::high_resolution_clock::now();
		auto delta = chrono::duration<float, std::milli>(end - start).count();
		totalMarchTime += delta;
		count++;

		auto n = model.vertices.size();

		for(auto face : faces)
			for(auto point : face) {
				model.vertices.push_back(get<0>(point));
				model.colors.push_back(get<1>(point));
			}

		for(auto i = 0u; i < faces.size(); i++)
			model.faces.push_back({
				n + 1 + i * 3,
				n + 1 + i * 3 + 1,
				n + 1 + i * 3 + 2
			});
	});

	printf("March Time\n");
	printf("\tAverage Cell: %fs\n", totalMarchTime / count / 1000);
	printf("\tTotal March: %fs\n", totalMarchTime / 1000);
}