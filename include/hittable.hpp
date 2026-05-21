#pragma once

#include "interval.hpp"
#include "ray.hpp"

#include <memory>

using std::make_shared;
using std::shared_ptr;

class material;

class [[nodiscard]] hit_record {
public:
	point3 p;
	vec3 normal;
	shared_ptr<material> mat;
	double t;
	bool front_face;

	void set_face_normal(const ray &r, const vec3 &outward_normal)
	{
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};


class hittable {
public:
	virtual ~hittable() = default;
	[[nodiscard]] virtual bool hit(const ray &r, interval ray_t, hit_record &rec) const = 0;
};
