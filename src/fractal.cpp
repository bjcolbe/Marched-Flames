#include "fractal.h"

Vertex Fractal::select(int index, Vertex in) {
	switch ( (int)(index % (funcNum-1)) ) {
		case 0:
			return func1(in);
		case 1:
			return func2(in);
		case 2:
			return func3(in);
		case 3:
			return func4(in);
	}
	return Vertex(0,0,0);
}

//Klein bottle
Vertex Fractal::func1(Vertex in) {
	Vertex out;
	float theta = atan2(in.y, in.x);
	float phi = atan2(sqrt((in.x*in.x) + (in.y*in.y)), in.z);

	float temp = (cos(theta/2)*(sqrt(2)+cos(phi))+(sin(theta/2)*sin(phi)*cos(phi)));
	out.x = cos(theta) * temp;
	out.y = sin(theta) * temp;
	out.z = -1 * sin(theta/2) * (sqrt(2)+cos(phi)) + cos(theta/2)*sin(phi)*cos(phi);
	return out;
}

//Fisheye
Vertex Fractal::func2(Vertex in) {
	Vertex out;
	float denom = sqrt( in.x*in.x + in.y*in.y + in.z*in.z ) + 1;
	out.x = (2*in.y)/denom;
	out.y = (2*in.x)/denom;
	out.z = (2*in.z)/denom;
	return out;
}

//Diamond
Vertex Fractal::func3(Vertex in) {
	Vertex out;
	float theta = atan2(in.y, in.x);
	float phi = atan2(sqrt((in.x*in.x) + (in.y*in.y)), in.z);
	float r = sqrt( in.x*in.x + in.y*in.y + in.z*in.z );
	out.x = sin(theta)*cos(r);
	out.y = cos(theta)*sin(r);
	out.z = sin(phi)*cos(r);
	return out;
}

//Handkerchief
Vertex Fractal::func4(Vertex in) {
	Vertex out;
	float theta = atan2(in.y, in.x);
	float phi = atan2(sqrt((in.x*in.x) + (in.y*in.y)), in.z);
	float r = sqrt( in.x*in.x + in.y*in.y + in.z*in.z );
	out.x = r * sin(theta+r);
	out.y = r * cos(theta-r);
	out.z = r * sin(phi+r);
	return out;
}
