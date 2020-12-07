#include <cmath>
#include "types/vertex.h"

struct Fractal {
	Vertex select(int index, Vertex in);
	Vertex func1(Vertex in);
	Vertex func2(Vertex in);
	Vertex func3(Vertex in);
	Vertex func4(Vertex in);
	int funcNum = 4;
};
