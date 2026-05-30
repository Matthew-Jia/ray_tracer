#pragma once

#include "vec3.hpp"
#include "interval.hpp"

#include <iostream>

using color = vec3;

[[nodiscard]] inline constexpr double linear_to_gamma(double linear_component) noexcept
{
  if (linear_component > 0)
    return std::sqrt(linear_component);

  return 0;
}

inline void write_color(std::ostream &out, const color &pixel_color) noexcept
{
	static const interval intensity(0.000, 0.999);

	auto r = linear_to_gamma(pixel_color.r);
	auto g = linear_to_gamma(pixel_color.g);
	auto b = linear_to_gamma(pixel_color.b);

	int rbyte = int(256 * intensity.clamp(r));
	int gbyte = int(256 * intensity.clamp(g));
	int bbyte = int(256 * intensity.clamp(b));

	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
