#pragma once

#include "vec3.hpp"

class [[nodiscard]] ray {
public:

	explicit ray() {}
	explicit ray(const point3& orig, const vec3 &dir) : ray{orig, dir, 0}  {}
	explicit ray(const point3& orig, const vec3 &dir, double time) : orig_{orig}, dir_{dir}, time_{time} {}

	const point3& origin() const noexcept { return orig_; }
	const vec3& direction() const noexcept { return dir_; }
	[[nodiscard]] double time() const noexcept { return time_; }

	point3 at(const double t) const { return orig_ + t*dir_; }

private:
	point3 orig_;
	vec3 dir_;
  double time_;
};
