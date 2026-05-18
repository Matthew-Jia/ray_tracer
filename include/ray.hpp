#pragma once

#include "vec3.hpp"

class [[nodiscard]] ray {
public:

	ray() {}
	ray(const point3& orig, const vec3 &dir) : orig_(orig), dir_(dir) {}

	const point3& origin() const noexcept { return orig_; }
	const vec3& direction() const noexcept { return dir_; }

	point3 at(const double t) const { return orig_ + t*dir_; }

private:
	point3 orig_;
	vec3 dir_;
};
