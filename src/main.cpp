#include <vector>
#include <tuple>
#include <string>
#include <iostream>
#include <cmath>
#include "types/model.h"
#include "graph.h"
#include "marching_cubes.h"
#include "fractal_flames.h"

using namespace std;

int main(int argc, char **argv) {
	vector<int> dimensions = {100, 100, 100}; //read from args
	vector<float> weights{0.5, 0.5, 0.5, 0.5}; //read from args
	vector<int> sdim{-1, -1, -1};
	string path = "model"; //read from args
	int iterations = 1000;
	int res = 20;
	int threads = 10;
	string device = "opencl";
	string function = "fractal";

	Model model;

	try{
		for(int i = 0; i < argc; i++) {
			string arg;
			arg = string(argv[i]);
			if (arg == "-o") {
				path = string(argv[i+1]);
			} else if (arg == "-i") {
				iterations = stoi(string(argv[i+1]));
			} else if (arg == "-t") {
				threads = stoi(string(argv[i+1]));
			} else if (arg == "-r") {
				res = stoi(string(argv[i+1]));
			} else if (arg == "-dim") {
				dimensions[0] = stoi(string(argv[i+1]));
				dimensions[1] = stoi(string(argv[i+2]));
				dimensions[2] = stoi(string(argv[i+3]));
			} else if (arg == "-weights") {
				weights[0] = stof(string(argv[i+1]));
				weights[1] = stof(string(argv[i+2]));
				weights[2] = stof(string(argv[i+3]));
				weights[3] = stof(string(argv[i+4]));
			} else if(arg == "-f")
				function = string(argv[i + 1]);
			else if (arg == "-sdim") {
				sdim[0] = stoi(string(argv[i+1]));
				sdim[1] = stoi(string(argv[i+2]));
				sdim[2] = stoi(string(argv[i+3]));
			} else if(arg == "-device")
				device = string(argv[i + 1]);
		}

	} catch (std::exception &e) {
		cout << "Caught Exception: " << e.what() << endl;
		cout << "Command Usage:\n"
			<< "-o [file]										Defines path to output to\n"
			<< "-i [int]										Defines the number of fractal flame iterations to run per kernel\n"
			<< "-t [int]										Defines the number of fractal flame threads to run\n"
			<< "-r [int]										Defines the model resolution\n"
			<< "-dim [int] [int] [int]							Defines xyz dimensions of fractal model\n"
			<< "-weights [float] [float] [float] [float]		Defines weights in the range [0, 1] for fractal functions\n"
			<< "-f [string]										Defines the function to create a model of\n"
			<< "-sdim [int] [int] [int]							Defines the dimensions of the marching cubes space";

		exit(0);
	}

	if(function == "fractal") {
		auto points = FractalFlames::fractal(iterations, threads, dimensions, weights);
		model.vertices = get<0>(points);
		model.colors = get<1>(points);
	} else if(function == "sincos")
		model = Graph::create(res, dimensions[0], dimensions[1], dimensions[2], [](float x, float z) { return 2 + sin(x) + cos(z); });
	else if(function == "sin")
		model = Graph::create(res, dimensions[0], dimensions[1], dimensions[2], [](float x, float z) { return 1 + sin(x); });
	else if(function == "cos")
		model = Graph::create(res, dimensions[0], dimensions[1], dimensions[2], [](float x, float z) { return 1 + cos(z); });

	printf("Initial\n\tVertices: %zu\n\tColors: %zu\n", model.vertices.size(), model.colors.size());

	if(device == "opencl")
		MarchingCubes::marchCL(res, model, sdim);
	else if(device == "cpu")
		MarchingCubes::march(res, model, sdim);

	printf("March\n\tVertices: %zu\n\tFaces: %zu\n", model.vertices.size(), model.faces.size());

	model.write(path);
}
