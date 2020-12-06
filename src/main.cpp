#include <vector>
#include <tuple>
#include <string>
#include <iostream>
#include "types/model.h"
#include "marching_cubes.h"
#include "fractal_flames.h"

using namespace std;

int main(int argc, char **argv) {

	vector<int> dimensions = {100, 100, 100}; //read from args
	vector<float> weights{1.1, 1.2, 1.3, 1.4}; //read from args
	string path = "model"; //read from args
	int iterations = 5;
	int res = 1;

	try{
		for(int i = 0; i < argc; i++) {
			string arg;
			arg = string(argv[i]);
			if (arg == "-o") {
				path = string(argv[i+1]);
			} else if (arg == "-i") {
				iterations = stoi(string(argv[i+1]));
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
			} else {}
		}

	} catch (std::exception &e) {
		cout << "Caught Exception: " << e.what() << endl;\
		cout << "Command Usage:\n-o [file]						Defines path to output to\n"
			<< "-i [int]										Defines the number of iterations to run\n"
			<< "-r [int]										Defines the model resolution\n"
			<< "-dim [int] [int] [int]							Defines xyz dimensions of fractal model\n"
			<< "-weights [float] [float] [float] [float]		Defines weights for fractal functions" << endl;
		exit(0);
	}

	auto points = FractalFlames::cpufractal(iterations, dimensions, weights);
	cout << "Frac done" << endl;
	auto vertices = get<0>(points);
	auto colors = get<1>(points);
	cout << "Frac really done" << endl;

	Model model;
	model.vertices = vertices;
	model.colors = colors;
	MarchingCubes::march(res, model);
	model.write(path);
}