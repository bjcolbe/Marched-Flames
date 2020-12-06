#include "marching_cubes.h"

#include <limits>
#include <cmath>
#include <vector>
#include "utils/kernel.h"
#include "utils/buffer.h"

using namespace std;
using Point = MarchingCubes::Point;
using Cell = MarchingCubes::Cell;

void MarchingCubes::marchCL(float resolution, Model &model) {
	Space space(resolution, model);

	model.vertices.clear();
	model.colors.clear();

	Kernel kernel("march.cl");
	kernel.compile("polygonize");

	array<float, 3 * 8> ivertices, icolors;
	array<float, 3 * 12> overtices, ocolors;
	int cubeIndex;

	Buffer<int> inEdgeTable(kernel, BufferType::ReadOnly, MarchingCubes::EdgeTable, MarchingCubes::TableSize);
	Buffer<int> inTriangleTable(kernel, BufferType::ReadOnly, MarchingCubes::TriangleTable1D, MarchingCubes::TableSize * MarchingCubes::SubTableSize);
	Buffer<int> inAssociation(kernel, BufferType::ReadOnly, MarchingCubes::Association, 12);
	Buffer<float> inVertices(kernel, BufferType::ReadOnly, ivertices.data(), ivertices.size());
	Buffer<float> inColors(kernel, BufferType::ReadOnly, icolors.data(), icolors.size());
	Buffer<float> outVertices(kernel, BufferType::WriteOnly, overtices.data(), overtices.size());
	Buffer<float> outColors(kernel, BufferType::WriteOnly, ocolors.data(), ocolors.size());

	inEdgeTable.write();
	inTriangleTable.write();
	inAssociation.write();

	kernel.addArg(inEdgeTable.data);
	kernel.addArg(inTriangleTable.data);
	kernel.addArg(inAssociation.data);
	kernel.addArg(MarchingCubes::isolevel);
	kernel.addArg(inVertices.data);
	kernel.addArg(inColors.data);
	kernel.addArg(outVertices.data);
	kernel.addArg(outColors.data);
	kernel.addArg(cubeIndex);
	
	space.forEach([&](Cell cell) {
		cubeIndex = 0;

		for(auto i = 0u; i < cell.size(); i++) {
			auto p = cell[i];

			if(Space::isActive(p))
				cubeIndex |= 1 << i;

			auto v = get<0>(p);
			ivertices[i * 3] = v.x;
			ivertices[i * 3 + 1] = v.y;
			ivertices[i * 3 + 2] = v.z;

			auto c = get<1>(p);
			icolors[i * 3] = c.r;
			icolors[i * 3 + 1] = c.g;
			icolors[i * 3 + 2] = c.b;
		}

		if(MarchingCubes::EdgeTable[cubeIndex] == 0)
			return;

		inVertices.write();
		inColors.write();

		kernel.removeArg();
		kernel.addArg(cubeIndex);

		kernel.run(1, new size_t[1]{12}, new size_t[1]{1});

		outVertices.read();
		outColors.read();

		auto n = model.vertices.size();

		for(auto i = 0; TriangleTable[cubeIndex][i] != -1; i += 3) {
			for(auto j = 0; j < 3; j++) {
				auto index = MarchingCubes::TriangleTable[cubeIndex][i + j];

				model.vertices.push_back({
					overtices[index * 3],
					overtices[index * 3 + 1],
					overtices[index * 3 + 2]
				});

				model.colors.push_back({
					ocolors[index * 3],
					ocolors[index * 3 + 1],
					ocolors[index * 3 + 2]
				});
			}

			model.faces.push_back({
				n + 1 + i,
				n + 1 + i + 1,
				n + 1 + i + 2
			});
		}
	});
}