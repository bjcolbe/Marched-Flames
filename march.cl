/**
 * @param vertices Input vertices packed as [[x y z], 8]
 * @param colors Input colors packed as [[r g b], 8]
 * @param outVertices Output faces packed as [[[x1 y1 z1] [x2 y2 z2] [x3 y3 z3]], 12]
 * @param outColors Ouput colors packed as [[r g b], 3 * 12]
 * @param faces Outputs whether the face exists at the index
 */
kernel void polygonize(
	constant int *EdgeTable,
	constant int *TriangleTable,
	constant int *Association,
	const float isolevel,
	const global float *vertices,
	const global float *colors,
	global float *outVertices,
	global float *outColors,
	const int cubeIndex
) {
	int idx = get_global_id(0);

	if(EdgeTable[cubeIndex] & (1 << idx)) {
		int j = idx % 8;
		int k = Association[idx];

		float x1 = vertices[j * 3], y1 = vertices[j * 3 + 1], z1 = vertices[j * 3 + 2];
		float x2 = vertices[k * 3], y2 = vertices[k * 3 + 1], z2 = vertices[k * 3 + 2];

		float r1 = colors[j * 3], g1 = colors[j * 3 + 1], b1 = colors[j * 3 + 2];
		float r2 = colors[k * 3], g2 = colors[k * 3 + 1], b2 = colors[k * 3 + 2];

		bool active1 = r1 > 0 || g1 > 0 || b1 > 0;
		bool active2 = r2 > 0 || g2 > 0 || b2 > 0;

		float v1 = active1 ? 0.0f : 1.0f;
		float v2 = active2 ? 0.0f : 1.0f;
		float s = (isolevel - v1) / (v2 - v1);
		outVertices[idx * 3] = x1 + s * (x2 - x1);
		outVertices[idx * 3 + 1] = y1 + s * (y2 - y1);
		outVertices[idx * 3 + 2] = z1 + s * (z2 - z1);

		float t = active1 && active2 ? 0.5f : (active2 ? 1.0f : 0.0f);
		outColors[idx * 3] = mix(r1, r2, t);
		outColors[idx * 3 + 1] = mix(g1, g2, t);
		outColors[idx * 3 + 2] = mix(b1, b2, t);
	}
}