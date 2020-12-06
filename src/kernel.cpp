#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

#include "utils/kernel.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int errorCode;

Kernel::Kernel(string path) {
	stringstream stream;
	stream << ifstream(path).rdbuf();
	this->source = stream.str();
}

Kernel::~Kernel() {
	validate(clReleaseContext(context));
	validate(clReleaseCommandQueue(commands));
	validate(clReleaseProgram(program));
	validate(clReleaseKernel(kernel));
}

void Kernel::compile(string name) {
	validate(clGetPlatformIDs(1, &this->platform, NULL));
	validate(clGetDeviceIDs(this->platform, CL_DEVICE_TYPE_GPU /*CL_DEVICE_TYPE_ALL*/, 1, &this->device, NULL))

	auto properties = new cl_context_properties[3];
	properties[0] = CL_CONTEXT_PLATFORM;
	properties[1] = (cl_context_properties)this->platform;
	properties[2] = 0;

	this->context = clCreateContext(properties, 1, &this->device, NULL, NULL, &errorCode);
	validate(errorCode);

	this->commands = clCreateCommandQueue(this->context, this->device, CL_QUEUE_PROFILING_ENABLE, &errorCode);
	validate(errorCode);

	auto source = this->source.c_str();
	this->program = clCreateProgramWithSource(this->context, 1, (const char**)&source, NULL, &errorCode);
	validate(errorCode);

	if(clBuildProgram(this->program, 0, NULL, NULL, NULL, NULL) != CL_SUCCESS) {
		cout << "OpenCL compile error" << endl;
		char buffer[4096];
		validate(clGetProgramBuildInfo(this->program, this->device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, NULL));
		cout << buffer << endl;
		exit(1);
	}

	this->kernel = clCreateKernel(this->program, name.c_str(), &errorCode);
	validate(errorCode);
}

float Kernel::run(int dimensions, size_t *globalSize, size_t *localSize) {
	cl_event event;
	validate(clEnqueueNDRangeKernel(this->commands, kernel, dimensions, NULL, globalSize, localSize, 0, NULL, &event));

	validate(clWaitForEvents(1, &event));
	validate(clFinish(commands));

	cl_ulong startTime, endTime;
	validate(clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &startTime, NULL));
	validate(clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &endTime, NULL));

	return (float)(endTime - startTime) / 1e6f;
}

void Kernel::removeArg(int count) {
	nextArg -= count;
}