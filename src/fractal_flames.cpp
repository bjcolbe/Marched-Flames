#include "fractal_flames.h"
#include "clut.h"
#include "fractal.h"
#include "CL/cl.h"
#include "utils/validator.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;
/*
char* getKernelFromFile(const char* filename, const char* preamble, size_t *sz) {
	FILE* fptr = NULL;
	size_t szSource, szPreamble;
	char* sourceString;

	fptr = fopen(filename, "r");
	szPreamble = strlen(preamble);

	fseek(fptr,0,SEEK_END);
	szSource = ftell(fptr);
	fseek(fptr,0,SEEK_SET);

	sourceString = (char*)calloc(szSource + szPreamble+1, sizeof(char));
	fclose(fptr);
	*sz = szSource * szPreamble;
	sourceString[szSource + szPreamble] = '\0';
	return sourceString;
}
*/
tuple<vector<Vertex>, vector<Color>> FractalFlames::cpufractal(int _iterations, vector<int> _dimensions, vector<float> _weights) {
	std::vector<Vertex> vVerts;
	std::vector<Color> vColors;

	Clut clut;
	clut.initColors();
	Fractal frac;

	float wsum = _weights[0]+_weights[1]+_weights[2]+_weights[3];
	int w1 = (_weights[0]/wsum) * 100 + 0.5;
	int w2 = (_weights[1]/wsum) * 100 + 0.5;
	int w3 = (_weights[2]/wsum) * 100 + 0.5;
	int w4 = (_weights[3]/wsum) * 100 + 0.5;

	int counts[4] = {0,0,0,0};
	vector<int> randoms(100,0);
	for (int i = 0; i < 100; i++) {
		if (counts[0] < w1) {
			randoms[i] = 0;
			counts[0]++;
		} else if (counts[1] < w2) {
			randoms[i] = 1;
			counts[1]++;
		} else if (counts[2] < w3) {
			randoms[i] = 2;
			counts[2]++;
		} else if (counts[3] < w4) {
			randoms[i] = 3;
			counts[3]++;
		}
	}

	int iterations = _iterations;

	Vertex V(0,0,0);
	Color C(0,0,0);

	V.x = 2.0 * drand48() - 1.0;
	V.y = 2.0 * drand48() - 1.0;
	V.z = 2.0 * drand48() - 1.0;
	float weight = drand48();

	for (int i = 0; i < iterations; i++) {
		int f_index = randoms[drand48() * 100];

		weight = (weight + _weights[f_index]) * 0.5;
		V = frac.select(f_index, V);

		V.x = (V.x + 1.0) * (0.5 * _dimensions[0]);
		V.y = (V.y + 1.0) * (0.5 * _dimensions[1]);
		V.z = (V.z + 1.0) * (0.5 * _dimensions[2]);

		C = clut.lookup(weight);

		vVerts.push_back(V);
		vColors.push_back(C);
	}

	std::tuple<std::vector<Vertex>, std::vector<Color>> tupOut (vVerts, vColors);
	return tupOut;
}

tuple<vector<Vertex>, vector<Color>> FractalFlames::fractal(int _iterations, int _threads, vector<int> _dimensions, vector<float> _weights) {

	//Set up a set of numbers to weight the functions
	vector<int> randoms(100,0);
	float wsum = _weights[0]+_weights[1]+_weights[2]+_weights[3];
	int w1 = (_weights[0]/wsum) * 100 + 0.5;
	int w2 = (_weights[1]/wsum) * 100 + 0.5;
	int w3 = (_weights[2]/wsum) * 100 + 0.5;
	int w4 = (_weights[3]/wsum) * 100 + 0.5;

	int counts[4] = {0,0,0,0};
	for (int i = 0; i < 100; i++) {
		if (counts[0] < w1) {
			randoms[i] = 0;
			counts[0]++;
		} else if (counts[1] < w2) {
			randoms[i] = 1;
			counts[1]++;
		} else if (counts[2] < w3) {
			randoms[i] = 2;
			counts[2]++;
		} else if (counts[3] < w4) {
			randoms[i] = 3;
			counts[3]++;
		}
	}

	//Set up prelim variables
	cl_platform_id platform_id;
	cl_uint num_of_platforms = 0;
	cl_uint num_of_devices = 0;
	cl_device_id device_id;
	cl_context_properties properties[3];
	cl_int err;
	cl_context context;
	cl_command_queue command_queue;
	cl_program program;
	cl_kernel kernel;
	cl_mem input1, input2, input3, output1, output2;
	size_t global[1], local[1];
	cl_event prof_event;

	//Populate input data
	cl_uint *dimensions, *fweights;
	cl_float *weights;
	cl_float *vertices, *colors;
	cl_uint iterations = _iterations;
	cl_uint threads = _threads;
	cl_uint vertTotal = threads * iterations;
	#define DATA_SIZE vertTotal * 3

	dimensions = (cl_uint *) malloc(sizeof(cl_uint) * 3);
	weights = (cl_float *) malloc(sizeof(cl_float) * 4);
	fweights = (cl_uint *) malloc(sizeof(cl_uint) * 100);
	vertices = (cl_float *) malloc(sizeof(cl_float) * DATA_SIZE);
	colors = (cl_float *) malloc(sizeof(cl_float) * DATA_SIZE);

	for (int i = 0; i < 3; i++) {
		dimensions[i] = _dimensions[i];
		weights[i] = _weights[i];
	}

	for (int i = 0; i < 100; i++) {
		fweights[i] = randoms[i];
	}

	for (cl_uint i = 0; i < vertTotal; i += 3) {
		for (int j = 0; j < 3; j++) {
			vertices[i+j] = 0;
			colors[i+j] = 0;
		}
	}

	//Set variable values
	if (clGetPlatformIDs(1, &platform_id, &num_of_platforms) != CL_SUCCESS) {
		std::cout << "Unable to get platform IDs" << std::endl;
		exit(0);
	}

	if (clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &num_of_devices) != CL_SUCCESS) {
		std::cout << "Unable to get device IDs" << std::endl;
		exit(0);
	}

	properties[0] = CL_CONTEXT_PLATFORM;
	properties[1] = (cl_context_properties) platform_id;
	properties[2] = 0;

	context = clCreateContext(properties, 1, &device_id, NULL, NULL, &err);
	command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &err);

	stringstream ss;
	ss << ifstream("fractal_flames.cl").rdbuf();
	auto sourcestr = ss.str();
	auto source = sourcestr.c_str();

	program = clCreateProgramWithSource(context, 1, (const char **) &source, NULL, &err);
	validate(err);

	//Compile the program
	if (clBuildProgram(program, 0, NULL, NULL, NULL, NULL) != CL_SUCCESS) {
		std::cout << "Error while compiling program" << std::endl;

		char buffer[4096];
		size_t length;
		clGetProgramBuildInfo(
			program,
			device_id,
			CL_PROGRAM_BUILD_LOG,
			sizeof(buffer),
			buffer,
			&length
		);

		std::cout << buffer << std::endl;
		exit(1);
	}

	kernel = clCreateKernel(program, "fflame", &err);
	validate(err);

	//Set up data buffers
	input1 = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_uint) * 3, NULL, &err);
	validate(err);
	input2 = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_float) * 3, NULL, &err);
	validate(err);
	input3 = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_uint) * 100, NULL, &err);
	validate(err);
	output1 = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_float) * DATA_SIZE, NULL, &err);
	validate(err);
	output2 = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_float) * DATA_SIZE, NULL, &err);
	validate(err);

	//Load data into input
	validate(clEnqueueWriteBuffer(command_queue, input1, CL_TRUE, 0, sizeof(cl_uint) * 3, dimensions, 0, NULL, NULL));
	validate(clEnqueueWriteBuffer(command_queue, input2, CL_TRUE, 0, sizeof(cl_float) * 3, weights, 0, NULL, NULL));
	validate(clEnqueueWriteBuffer(command_queue, input3, CL_TRUE, 0, sizeof(cl_uint) * 100, fweights, 0, NULL, NULL));

	//Set arguments for kernel
	validate(clSetKernelArg(kernel, 0, sizeof(int), &iterations));
	validate(clSetKernelArg(kernel, 1, sizeof(cl_mem), &input1));
	validate(clSetKernelArg(kernel, 2, sizeof(cl_mem), &input2));
	validate(clSetKernelArg(kernel, 3, sizeof(cl_mem), &input3));
	validate(clSetKernelArg(kernel, 4, sizeof(cl_mem), &output1));
	validate(clSetKernelArg(kernel, 5, sizeof(cl_mem), &output2));

	//Configure local/global data into chunks
	global[0] = threads;

	int local_num = threads / 10;
	if (local_num > 100) {
		local_num = 100;
	} else if (local_num < 1) {
		local_num = 1;
	}

	local[0] = local_num;

	//Queue up the kernel
	err = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, global, local, 0, NULL, &prof_event);
	validate(err);
	clFinish(command_queue);

	//Profiling Timing
	err = clWaitForEvents(1, &prof_event);
	cl_ulong start_time, end_time;
	double run_time;
	size_t return_bytes;
	err = clGetEventProfilingInfo(prof_event,CL_PROFILING_COMMAND_START,sizeof(cl_ulong),&start_time,&return_bytes);
	err = clGetEventProfilingInfo(prof_event,CL_PROFILING_COMMAND_END,sizeof(cl_ulong),&end_time,&return_bytes);
	run_time = (double)(end_time-start_time);

	//Get output out
	clEnqueueReadBuffer(command_queue, output1, CL_TRUE, 0, sizeof(cl_float) * DATA_SIZE, vertices, 0, NULL, NULL);
	clEnqueueReadBuffer(command_queue, output2, CL_TRUE, 0, sizeof(cl_float) * DATA_SIZE, colors, 0, NULL, NULL);

	//Print results
	// printMat(results);
	std::cout << "Fractal Flame segment ran for: " << run_time << " ms" << std::endl;

	//Cleanup
	clReleaseContext(context);
	clReleaseCommandQueue(command_queue);
	clReleaseProgram(program);
	validate(clReleaseKernel(kernel));
	clReleaseMemObject(input1);
	clReleaseMemObject(input2);
	clReleaseMemObject(output1);
	clReleaseMemObject(output2);

	//Assign the output into vector/tuple form
	std::vector<Vertex> vVerts;
	std::vector<Color> vColors;
	for (cl_uint i = 0; i < DATA_SIZE; i += 3) {
		Vertex vert( vertices[i], vertices[i+1], vertices[i+2] );
		Color col( colors[i], colors[i+1], colors[i+2] );
		vVerts.push_back(vert);
		vColors.push_back(col);
	}

	std::tuple<std::vector<Vertex>, std::vector<Color>> tupOut (vVerts, vColors);
	return tupOut;
}

