#pragma once

#include "vec3.hpp"
#include "interval.hpp"

#include <iostream>

using color = vec3;

[[nodiscard]] inline double linear_to_gamma(double linear_component)
{
	assert((interval{0, 1}).contains(linear_component));

	if (linear_component == 0)
		return 0;

	return std::sqrt(linear_component);
}

inline void write_color(std::ostream &out, const color &pixel_color)
{
	auto r = linear_to_gamma(pixel_color.r);
	auto g = linear_to_gamma(pixel_color.g);
	auto b = linear_to_gamma(pixel_color.b);

	static const interval intensity(0.000, 0.999);
	int rbyte = int(256 * intensity.clamp(r));
	int gbyte = int(256 * intensity.clamp(g));
	int bbyte = int(256 * intensity.clamp(b));

	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
