#include <vector>

using namespace std;

struct Clut {
	void initColors();
	vector<vector<float>> colors;
	vector<float> lookup(float value);
};
