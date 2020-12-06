#include <vector>
#include "types/color.h"

using namespace std;

struct Clut {
	void initColors();
	vector<vector<float>> colors;
	Color lookup(float value);
};
