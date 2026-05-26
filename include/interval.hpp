#pragma once

#include "common.hpp"

class [[nodiscard]] interval {
public:
	double min, max;

	explicit interval() : min{+infinity}, max{-infinity} {}
	explicit interval(double min, double max) : min{min}, max{max} {}
	explicit interval(const interval &i1, const interval &i2) :
    min{std::fmin(i1.min, i2.min)}, max{std::fmax(i1.max, i2.max)} 
  {}

	[[nodiscard]] double size() const { return max - min; }
	[[nodiscard]] bool contains(double x) const { return min <= x && x <= max; }
	[[nodiscard]] bool surrounds(double x) const { return min < x && x < max; }
	[[nodiscard]] double clamp(double x) const 
	{
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}

  [[nodiscard]] static bool overlap(const interval &a, const interval &b)
  {
    auto min = std::fmax(a.min, b.min);
    auto max = std::fmin(a.max, b.max);
    return min < max;
  }

	static const interval empty, universe;
};

const interval interval::empty		= interval{+infinity, -infinity};
const interval interval::universe = interval{-infinity, +infinity};
