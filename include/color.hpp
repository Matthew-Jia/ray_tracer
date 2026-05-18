#pragma once

#include "vec3.hpp"
#include "interval.hpp"

#include <iostream>

using color = vec3;

inline void write_color(std::ostream &out, const color &pixel_color)
{
	static const interval intensity(0.000, 0.999);
	int rbyte = int(256 * intensity.clamp(pixel_color.r));
	int gbyte = int(256 * intensity.clamp(pixel_color.g));
	int bbyte = int(256 * intensity.clamp(pixel_color.b));

	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
