#include <vector>
#include <tuple>
#include <string>
#include "types/model.h"
#include "marching_cubes.h"
#include "fractal_flames.h"

using namespace std;

int main(int argc, char **argv) {

	tuple<int, int, int> dimensions{100, 100, 100}; //read from args
	vector<float> weights{1.1, 1.2, 1.3, 1.4}; //read from args
	string path = "model.obj"; //read from args
	int iterations = 1000000;

	try{
		for(int i = 0; i < argc; i++) {
			string arg == string(argv[i]);
			if (arg == "-o") {
				path = string(argv[i+1]);
			} else if (arg == "-i") {
				iterations = stoi(string(argv[i+1]));
			} else if (arg == "-dim") {
				get<0>(dimensions) = stoi(string(argv[i+1]));
				get<1>(dimensions) = stoi(string(argv[i+2]));
				get<2>(dimensions) = stoi(string(argv[i+3]));
			} else if (arg == "-weights") {
				weights[0] = stof(string(argv[i+1]));
				weights[1] = stof(string(argv[i+2]));
				weights[2] = stof(string(argv[i+3]));
				weights[3] = stof(string(argv[i+4]));
			} else {}
		}

	} catch (std::exception &e) {
		cout << "Caught Exception: " << e.what() << endl;\
		cout << "Command Usage:\n-o [file]						Defines path to output to\n"
			<< "-i [int]										Defines the number of iterations to run\n"
			<< "-dim [int] [int] [int]							Defines xyz dimensions of fractal model\n"
			<< "-weights [float] [float] [float] [float]		Defines weights for fractal functions" << endl;
		exit(0);
	}

	auto points = FractalFlames::fractal(iterations, dimensions, weights);
	auto vertices = get<0>(points);
	auto colors = get<1>(points);

	auto faces = MarchingCubes::march(vertices);

	Model model;
	model.vertices = vertices;
	model.vertexColors = colors;
	model.faces = faces;

	model.write(path);
}
