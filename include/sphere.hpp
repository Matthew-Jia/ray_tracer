#pragma once

#include "aabb.hpp"
#include "hittable.hpp"
#include "material.hpp"
#include "vec3.hpp"

#include <memory>

using std::shared_ptr;
using std::make_shared;

class [[nodiscard]] sphere : public hittable {
public:
	explicit sphere(const point3 &static_center, double radius, shared_ptr<material> mat)
    : center_{static_center, vec3{0, 0, 0}}, radius_{std::fmax(0, radius)}, mat_{mat}
  {
    vec3 rvec{radius, radius, radius};
    bbox_ = aabb{static_center - rvec, static_center + rvec};
  }

	explicit sphere(const point3 &center1, const point3 &center2, double radius, 
      shared_ptr<material> mat)
    : center_{center1, center2 - center1}, radius_{std::fmax(0, radius)}, mat_{mat}
  {
    vec3 rvec{radius, radius, radius};
    aabb box1{center_.at(0) - rvec, center_.at(0) + rvec};
    aabb box2{center_.at(1) - rvec, center_.at(1) + rvec};
    bbox_ = aabb{box1, box2};
  }


	[[nodiscard]] bool hit(const ray &r, interval ray_t, hit_record &rec) const override
	{
    point3 current_center = center_.at(r.time());
		vec3 oc = current_center - r.origin();
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
		vec3 outward_normal = (rec.p - current_center) / radius_;
    rec.set_face_normal(r, outward_normal);
		rec.mat = mat_;

		return true;
	}

  aabb bounding_box() const override { return bbox_; }

private:
	ray center_;
	double radius_;
	shared_ptr<material> mat_;
  aabb bbox_;
};
