#pragma once

#include "vec3.hpp"

class [[nodiscard]] ray {
public:

  constexpr ray() = default;

	constexpr ray(const point3& orig, const vec3 &dir) noexcept 
    : ray{orig, dir, 0}  {}

	constexpr ray(const point3& orig, const vec3 &dir, double time) noexcept 
    : orig_{orig}, dir_{dir}, time_{time} {}

	constexpr const point3& origin() const noexcept { return orig_; }
	constexpr const vec3& direction() const noexcept { return dir_; }

	[[nodiscard]] constexpr double time() const noexcept { return time_; }

	constexpr point3 at(const double t) const noexcept { return orig_ + t*dir_; }

private:
	point3 orig_;
	vec3 dir_;
  double time_;
};
