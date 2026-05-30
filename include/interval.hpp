#pragma once

#include "common.hpp"

class [[nodiscard]] interval {
public:
	double min, max;

	constexpr interval() noexcept : min{+infinity}, max{-infinity} {}
	constexpr interval(double min, double max) noexcept : min{min}, max{max} {}
	constexpr interval(const interval &i1, const interval &i2) noexcept :
    min{std::fmin(i1.min, i2.min)}, max{std::fmax(i1.max, i2.max)} 
  {}

	[[nodiscard]] constexpr double size() const noexcept            { return max - min; }
	[[nodiscard]] constexpr bool contains(double x) const noexcept  { return min <= x && x <= max; }
	[[nodiscard]] constexpr bool surrounds(double x) const noexcept { return min < x && x < max; }

	[[nodiscard]] constexpr double clamp(double x) const noexcept 
	{
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}

  constexpr interval expand(double delta) const noexcept
  {
    auto padding = delta/2;
    return interval{min-padding, max+padding};
  }

  [[nodiscard]] static constexpr bool overlap(const interval &a, const interval &b) noexcept
  {
    auto min = std::fmax(a.min, b.min);
    auto max = std::fmin(a.max, b.max);
    return min < max;
  }
  
	static const interval empty, universe;
};

inline constexpr interval interval::empty{+infinity, -infinity};
inline constexpr interval interval::universe{-infinity, +infinity};

inline constexpr interval operator+(const interval &it, double displacement) noexcept
{
  return interval{it.min + displacement, it.max + displacement};
}

inline constexpr interval operator+(double displacement, const interval &it) noexcept
{
  return it + displacement;
}
