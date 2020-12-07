struct vertex {
	float x,y,z;
};

struct color {
	float r,g,b;
};

struct color getcolor(struct vertex tempv) {
	struct color out;
	float r = 0.0;
	float g = 0.0;
	float b = 0.0;

	float hue = ( (int)tempv.z * 360 ) % 360;
	float x = 1 - abs( ((int)(hue/60) % 2) - 1 );

	if (0 <= hue < 60) {
		r = 1; g = x; b = 0;
	} else if (60 <= hue < 120) {
		r = x; g = 1; b = 0;
	} else if (120 <= hue < 180) {
		r = 0; g = 1; b = x;
	} else if (180 <= hue < 240) {
		r = 0; g = x; b = 1;
	} else if (240 <= hue < 300) {
		r = x; g = 0; b = 1;
	} else if (300 <= hue < 360) {
		r = 1; g = 0; b = x;
	}
	
	out.r = r; out.g = g; out.b = b;
	return out;
}

float rand(ulong *seed) {
	seed = ( *seed * 0x5DEEDE66DL + 0xBL) & ((1L << 48) - 1);
	return abs( sin( (float)((int)seed % 360) ) ); 
}

__kernel void fflame(
    const int iterations,
    const __global float *dimensions,
    const __global float *weights,
    const __global uint *fweights,
    __global float *vertices,
    __global float *colors
) {

	ulong seed = (ulong)get_global_id(0);
	int minmem = (get_global_id(0) - 1) * iterations * 3;
	if (minmem < 0) { minmem = 0; }

	struct vertex tempv;
	struct color tempc;
	tempv.x = 2.0 * rand(&seed) - 1.0;
	tempv.y = 2.0 * rand(&seed) - 1.0;
	tempv.z = 2.0 * rand(&seed) - 1.0;
	float weight = rand(&seed);

	int ii = 0;
	for (int i = 0; i < iterations; i++) {
		int f_index = fweights[(int)(rand(&seed) * 100)];

		weight = (weight + weights[f_index]) * 0.5;

		//Here's where things start to get messy
		// Start of function declarations
		
		if (f_index == 0) {
			float theta = atan2( tempv.y, tempv.x );
			float phi = atan2( sqrt(tempv.x*tempv.x + tempv.y*tempv.y), tempv.z );
			float temp = ( cos(theta/2) * (sqrt(2.0) + cos(phi)) + ( sin(theta/2) * sin(phi) * cos(phi) ) );

			tempv.x = cos(theta) * temp;
			tempv.y = sin(theta) * temp;
			tempv.z = -1.0 * sin(theta/2) * ( sqrt(2.0) + cos(phi) ) + ( cos(theta/2) * sin(phi) * cos(phi) );
		} else if (f_index == 1) {
			float denom = sqrt( tempv.x*tempv.x + tempv.y*tempv.y + tempv.z*tempv.z ) + 1.0;

			tempv.x = (2.0 * tempv.y)/denom;
			tempv.y = (2.0 * tempv.x)/denom;
			tempv.z = (2.0 * tempv.z)/denom;
		} else if (f_index == 2) {
			float theta = atan2( tempv.y, tempv.x );
			float phi = atan2( sqrt(tempv.x*tempv.x + tempv.y*tempv.y), tempv.z );
			float r = sqrt( tempv.x*tempv.x + tempv.y*tempv.y + tempv.z*tempv.z);

			tempv.x = sin(theta) * cos(r);
			tempv.y = cos(theta) * sin(r);
			tempv.z = sin(phi) * cos(r);
		} else if (f_index == 3) {
			float theta = atan2( tempv.y, tempv.x );
			float phi = atan2( sqrt(tempv.x*tempv.x + tempv.y*tempv.y), tempv.z );
			float r = sqrt( tempv.x*tempv.x + tempv.y*tempv.y + tempv.z*tempv.z);

			tempv.x = r * sin(theta+r);
			tempv.y = r * cos(theta-r);
			tempv.z = r * sin(phi+r);
		}

		// End of function declarations

		tempc = getcolor(tempv);
		vertices[minmem + ii] = (tempv.x + 1.0) * (0.5 * dimensions[0]);
		vertices[minmem + ii + 1] = (tempv.y + 1.0) * (0.5 * dimensions[1]);
		vertices[minmem + ii + 2] = (tempv.z + 1.0) * (0.5 * dimensions[2]);

		colors[minmem + ii] = tempc.r;
		colors[minmem + ii + 1] = tempc.g;
		colors[minmem + ii + 2] = tempc.b;
		ii += 3;
	}

}
