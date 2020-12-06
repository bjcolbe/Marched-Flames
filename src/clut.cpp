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

Color Clut::lookup(float value) {
	Color C(0,0,0);
	if (value > 1.0 || value < 0.0) { return out; }

	int lowi = value * (colors.size() - 1);
	int highi = lowi + 1;
	if (highi >= colors.size()) { highi = colors.size() - 1; }

	double weight = (value * (colors.size() - 1)) - (double)lowi;
	C.r = colors[lowi][0] * (1.0 - weight) + colors[highi][0] * (weight);
	C.g = colors[lowi][1] * (1.0 - weight) + colors[highi][1] * (weight);
	C.b = colors[lowi][2] * (1.0 - weight) + colors[highi][2] * (weight);

	return C;
}
