#pragma once

#include <limits>
#include <numbers>
#include <random>

// Constants

inline constexpr double infinity = std::numeric_limits<double>::infinity();
inline constexpr double pi = std::numbers::pi;

[[nodiscard]] inline constexpr double random_double() noexcept
{
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator(42);
	return distribution(generator);
}

[[nodiscard]] inline constexpr double random_double(double min, double max) noexcept { return min + (max-min)*random_double(); }
[[nodiscard]] inline constexpr int random_int(int min, int max) noexcept { return (int)random_double(min, max + 1); }
[[nodiscard]] inline constexpr double degrees_to_radians(double degrees) noexcept { return degrees * pi / 180.0; }

