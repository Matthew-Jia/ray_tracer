#pragma once

#include "aabb.hpp"
#include "interval.hpp"
#include "ray.hpp"

#include <memory>

class material;

class [[nodiscard]] hit_record {
public:
	point3 p;
	vec3 normal;
  std::shared_ptr<material> mat;
	double t;
  double u;
  double v;
	bool front_face;

	void set_face_normal(const ray &r, const vec3 &outward_normal)
	{
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};


class hittable {
public:
	virtual ~hittable() = default; // makes destructor virtual
	[[nodiscard]] virtual bool hit(const ray &r, interval ray_t, hit_record &rec) const = 0;
  virtual aabb bounding_box() const = 0;
};
