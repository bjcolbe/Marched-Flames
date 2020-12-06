#include "clut.h"

void Clut::initColors() {
	vector<float> color(3,0);
	color[0] = 176.0/255.0;
	color[1] = 196.0/255.0;
	color[2] = 0.0;
	colors.push_back(color);

	color[0] = 213.0/255.0;
	color[1] = 222.0/255.0;
	color[2] = 46.0/255.0;
	colors.push_back(color);

	color[0] = 121.0/255.0;
	color[1] = 90.0/255.0;
	color[2] = 45.0/255.0;
	colors.push_back(color);

	color[0] = 82.0/255.0;
	color[1] = 50.0/255.0;
	color[2] = 19.0/255.0;
	colors.push_back(color);

	color[0] = 195.0/255.0;
	color[1] = 188.0/255.0;
	color[2] = 149.0/255.0;
	colors.push_back(color);

	color[0] = 211.0/255.0;
	color[1] = 204.0/255.0;
	color[2] = 165.0/255.0;
	colors.push_back(color);
}

vector<float> Clut::lookup(float value) {
	vector<float> out = {0.0,0.0,0.0};
	if (value > 1.0 || value < 0.0) { return out; }

	int lowi = value * (colors.size() - 1);
	int highi = lowi + 1;
	if (highi >= colors.size()) { highi = colors.size() - 1; }

	double weight = (value * (colors.size() - 1)) - (double)lowi;
	for (int i = 0; i < out.size(); i++) {
		out[i] = colors[lowi][i] * (1.0 - weight) + colors[highi][i] * (weight);
	}

	return out;
}
