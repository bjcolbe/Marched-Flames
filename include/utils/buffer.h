#pragma once

#include <CL/cl.h>
#include "kernel.h"
#include "validator.h"

#include <iostream>

/**
 * Buffer type
 */
enum BufferType {
	ReadOnly,
	WriteOnly,
	ReadWrite
};

/**
 * Creates a buffer between host and device memory
 */
template<typename T>
class Buffer {
	public:
		/**
		 * @param kernel Kernel to create buffer for
		 * @param type Memory type
		 * @param array Host array
		 * @param length Host array length
		 */
		Buffer(Kernel &kernel, BufferType type, T *array, int length) : kernel(kernel) {
			this->array = array;
			this->length = length;

			cl_mem_flags flags;

			switch(type) {
				case ReadOnly:
					flags = CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR;
					break;
				case WriteOnly:
					flags = CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR;
					break;
				case ReadWrite:
					flags = CL_MEM_READ_WRITE;
					break;
			}

			int errorCode;
			data = clCreateBuffer(kernel.context, flags, sizeof(T) * length, array, &errorCode);
			validate(errorCode);
		}

		~Buffer() {
			validate(clReleaseMemObject(data));
		}

		/**
		 * Write host memory to device
		 */
		void write() {
			cl_event event;
			validate(clEnqueueWriteBuffer(kernel.commands, data, CL_TRUE, 0, sizeof(T) * length, array, 0, NULL, &event));
			validate(clWaitForEvents(1, &event));
		}

		/**
		 * Read device memory to host
		 */
		void read() {
			cl_event event;
			validate(clEnqueueReadBuffer(kernel.commands, data, CL_TRUE, 0, sizeof(T) * length, array, 0, NULL, &event));
			validate(clWaitForEvents(1, &event));
		}
	public:
		cl_mem data;
	private:
		Kernel &kernel;
		T *array;
		int length;
};