#pragma once

#include <limits>
#include <random>

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

[[nodiscard]] inline double degrees_to_radians(double degrees) { return degrees * pi / 180.0; }
[[nodiscard]] inline double random_double() 
{
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

[[nodiscard]] inline double random_double(double min, double max) { return min + (max-min)*random_double(); }

	

