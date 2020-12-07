#include "utils/rand.h"

#include <random>

using namespace std;

float Rand::range(float lower, float upper) {
	static default_random_engine engine;
	return uniform_real_distribution<>(lower, upper)(engine);
}