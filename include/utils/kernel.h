#pragma once

#include <string>
#include <CL/cl.h>
#include "validator.h"

class Kernel {
	public:
		/** Kernel source code */
		std::string source;

		/**
		 * @param path Path to kernel source
		 */
		Kernel(std::string path);

		~Kernel();

		/**
		 * Compiles the specified kernel from the source
		 * @param name Kernel name
		 */
		void compile(std::string name);

		/**
		 * Runs the kernel with the specified problem size
		 * @param dimension Dimensions of the problem (1D, 2D, etc.)
		 * @param globalSize Global size in each dimension
		 * @param localSize Local size in each dimension
		 * @return Execution time in milliseconds
		 */
		float run(int dimension, size_t *globalSize, size_t *localSize);

		/**
		 * Add the next argument to the kernel function
		 * @param arg Next kernel parameter value
		 */
		template<typename T>
		void addArg(T &arg) {
			validate(clSetKernelArg(this->kernel, nextArg++, sizeof(T), (void*)&arg));
		}

		void removeArg(int count = 1);
	protected:
		int nextArg = 0;
	public:
		cl_platform_id platform;
		cl_device_id device;
		cl_command_queue commands;
		cl_context context;
		cl_program program;
		cl_kernel kernel;
};