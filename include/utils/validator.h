#pragma once

#include <CL/cl.h>

#define validate(x) { Validator::validateGPU((x), __FILE__, __LINE__); }

namespace Validator {
	void validateGPU(cl_int, const char*, int);
}