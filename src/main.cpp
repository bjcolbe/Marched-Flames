#include <vector>
#include <tuple>
#include "types/model.h"
#include "marching_cubes.h"
#include "fractal_flames.h"

using namespace std;

int main(int argc, char **argv) {
	tuple<int, int, int> dimensions{100, 100, 100}; //read from args
	vector<float> weights{1.1, 1.2, 1.3, 1.4}; //read from args
	auto path = "model.obj"; //read from args

	auto points = FractalFlames::fractal(dimensions, weights);
	auto vertices = get<0>(points);
	auto colors = get<1>(points);

	auto faces = MarchingCubes::march(vertices);

	Model model;
	model.vertices = vertices;
	model.vertexColors = colors;
	model.faces = faces;

	model.write(path);
}