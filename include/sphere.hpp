#pragma once

#include "hittable.hpp"
#include "vec3.hpp"

class [[nodiscard]] sphere : public hittable {
public:
	sphere(const point3 &center, double radius) : center_(center), radius_(std::fmax(0, radius)) {}

	[[nodiscard]] bool hit(const ray &r, interval ray_t, hit_record &rec) const override
	{
		vec3 oc = center_ - r.origin();
		auto a = r.direction().length_squared();
		auto h = dot(r.direction(), oc);
		auto c = oc.length_squared() - (radius_ * radius_);
		auto discriminant = (h*h) - (a*c);

		if (discriminant < 0) return false;

		auto sqrtd = std::sqrt(discriminant);
		auto root = (h - sqrtd) / a;

		if (!ray_t.surrounds(root))
		{
			root = (h + sqrtd) / a;
			if (!ray_t.surrounds(root)) 
				return false;
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		vec3 outward_normal = (rec.p - center_) / radius_;
		rec.normal = (rec.p - center_) / radius_;

		return true;
	}


private:
	point3 center_;
	double radius_;
};
