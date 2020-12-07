#include "types/model.h"

#include <ostream>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

void Model::scale(float value) {
	for(auto i = (int)vertices.size() - 1; i >= 0; i--)
		vertices[i] *= value;
}

void Model::write(string path, string format) {
	auto name = path.substr(0, path.find_last_of("\\/"));

	if(format == "obj") {
		ofstream mesh(path + ".obj");
		mesh << fixed << setprecision(4);

		mesh << "mtllib " << name << ".mtl" << endl;
		mesh << endl;

		mesh << "#Vertices" << endl;
		for(auto vertex : vertices)
			mesh << "v " << vertex.x << " " << vertex.z << " " << vertex.y << endl;

		mesh << endl;

		mesh << "#Faces" << endl;
		for(auto i = 0u; i < faces.size(); i++) {
			auto face = faces[i];
			mesh << "usemtl mat" << i << endl;
			mesh << "f " << get<0>(face) << " " << get<1>(face) << " " << get<2>(face) << endl;
		}

		mesh.close();

		ofstream material(path + ".mtl");
		material << fixed << setprecision(4);

		for(auto i = 0u; i < faces.size(); i++) {
			material << "newmtl mat" << i << endl;

			auto c = (colors[i * 3] + colors[i * 3 + 1] + colors[i * 3 + 2]) / 3;
			material << "Kd " << c.r << " " << c.g << " " << c.b << endl;
			material << "Ks " << 0.5 << " " << 0.5 << " " << 0.5 << endl;
		}

		material.close();
	} else if(format == "ply") {
		ofstream file(path + ".ply");
		file << fixed << setprecision(4);

		file << "ply" << endl;
		file << "format ascii 1.0" << endl;
		file << "element vertex 36" << endl;
		file << "property float x" << endl;
		file << "property float y" << endl;
		file << "property float z" << endl;
		file << "property float intensity" << endl;
		file << "property uchar diffuse_red" << endl;
		file << "property uchar diffuse_green" << endl;
		file << "property uchar diffuse_blue" << endl;
		file << "end_header" << endl;

		for(auto i = 0u; i < vertices.size(); i++) {
			auto v = vertices[i];
			auto c = colors[i];

			file << v.x << " " << v.y << " " << v.z;
			file << " " << 1;
			file << " " << c.r << " " << c.g << " " << c.b;
			file << endl;
		}
	} else
		cout << "Unrecognized file format '" << format << "'" << endl;
}
